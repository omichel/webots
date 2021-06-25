#!/usr/bin/env python3

# Copyright 1996-2021 Cyberbotics Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Author Simon Steinmann (https://github.com/Simon-Steinmann)
#


# Automatic PROTO to multi-file PROTO trimesh extractor
#
# INSTRUCTIONS:
# Call the script with the --input=<path> argument
#
# 1. if <path> ends in <filename>.proto:
#   - creates a new folder in the same directory named "multifile_<filename>"
#   - creates a new <filename>.proto inside this folder with all trimeshes
#     replaced by proto files, placed in a subfolder.
#   - the mesh proto files in the subfolder have the same header as the
#     original file, with the additional 'hidden' tag added.
#
# 2. If <path> does not end in ".proto" -> assumes directory
#   - duplicates our chosen <path> and appends "_multiProto_0" to the name
#   - the original <path> remains unchanged. If executed multiple times, the
#     trailing number for the <path> duplicates increases.
#   - searches for every .proto file in the duplicated <path> recursively and:
#       - ignores .proto file with either no header or a 'hidden' tag
#       - replaces the file with a version, where meshes are extracted and put
#         into a "<protoname>/" folder.
#       - the mesh proto files in the subfolder have the same header as the
#         original file, with the additional 'hidden' tag added.
#
#

import os
import argparse
import shutil
import sys
import time

# modules needed for multithreading
import multiprocessing
import signal


def mutliprocess_initializer():
    """Ignore CTRL+C in the worker process."""
    signal.signal(signal.SIGINT, signal.SIG_IGN)


class proto2mesh:
    def __init__(self):
        print('Proto Batch Editor by Simon Steinmann', flush=True)
        self.disableMeshOptimization = False
        self.disableFileCreation = False

    def convert(self, inFile, outFile, headerInsert, baseNodeInsert, baseNodeTypes, abortKeywords, verbose=True):
        self.robotName = os.path.splitext(os.path.basename(inFile))[0]
        newPath = os.path.dirname(outFile)
        print('Editing ' + outFile, flush=True)
        os.makedirs(newPath, exist_ok=True)
        # make a directory to store meshes with the same name as the proto
        os.makedirs(outFile.replace('.proto', ''), exist_ok=True)
        # The input PROTO file we are converting
        self.f = open(inFile)
        self.protoFileString = ''
        # The new proto file, with meshes extracted
        self.lineNumber = 0  # current line in the source proto file. For debug purposes.
        lines = []
        # Standard spacings that are used if the proto header is empty or has no comments.
        # The indices are ["field", fieldType, fieldName, Data, '#']
        headerColumnIndices = [2, 8, 19, 34, 55]
        headerIsEdited = False
        checkedBaseNode = False
        while True:
            line = self.f.readline()
            self.lineNumber += 1
            ln = line.split()
            # termination condition:
            eof = 0
            while ln == []:
                lines.append(line)
                line = self.f.readline()
                self.lineNumber += 1
                ln = line.split()
                eof += 1
                if eof > 10:
                    self.f.close()
                    self.cleanup(inFile)
                    if not self.disableFileCreation:
                        lines.insert(-13, baseNodeInsert)
                        self.pf = open(outFile, 'w', newline='\n')
                        self.pf.write(''.join(lines[:-10]))
                        self.pf.close()
                    return 'success'

            # check if we have the correct Basenode Type
            if headerIsEdited:
                if not checkedBaseNode:
                    if len(ln) == 2 and ln[1] == '{':
                        checkedBaseNode = True
                        if not ln[0] in baseNodeTypes:
                            raise ValueError('Error: Wrong BaseNode Type. {} is not in {}'.format(ln[0], baseNodeTypes))
            if not headerIsEdited:
                # Detect the spacings of arguments in the header
                if 'field' in ln:
                    for i in range(min(len(ln), 4)):
                        headerColumnIndices[i] = line.index(ln[i])
                    if '#' in ln:
                        headerColumnIndices[4] = line.index('#')
                if line == ']\n':
                    # Insert our insertString at the end of the PROTO header, using the spacings extracted above
                    headerIsEdited = True
                    insertSplit = headerInsert.split()
                    insertCommentIndex = insertSplit.index('#')
                    insertString = ''
                    insertString += ' ' * headerColumnIndices[0] + insertSplit[0]
                    insertString += ' ' * (headerColumnIndices[1] - len(insertString)) + insertSplit[1]
                    insertString += ' ' * (headerColumnIndices[2] - len(insertString)) + insertSplit[2]
                    insertString += ' ' * max(1, (headerColumnIndices[3] - len(insertString))) + insertSplit[3]
                    insertString += ' ' + ' '.join(insertSplit[4:insertCommentIndex])
                    insertString += ' ' * (headerColumnIndices[4] - len(insertString)
                                           ) + ' '.join(insertSplit[insertCommentIndex:])
                    lines.append(insertString + '\n')

            # Write the whole line from input to output file, without changes
            if any(item in ln for item in abortKeywords):
                raise ValueError('Error: {} contains at least one abort Keyword {}!'.format(ln, abortKeywords))
            lines.append(line)

    def cleanup(self, inFile, outFile=None):
        if inFile.endswith('proto_temp'):
            os.remove(inFile)
        if outFile is not None:
            os.remove(outFile)

    def convert_all(self, pool, sourcePath, outPath, headerInsert, baseNodeInsert, baseNodeTypes, abortKeywords, verbose):
        if outPath is None:
            outPath = sourcePath
        outPath = self.create_outputDir(sourcePath, outPath)
        print(outPath)
        if not self.disableFileCreation:
            os.makedirs(outPath, exist_ok=True)
        # Find all the proto files, and store their filePaths
        os.chdir(sourcePath)
        # Walk the tree.
        self.protoFiles = []  # List of the full filepaths.
        failedConvertions = []
        for root, directories, files in os.walk('./'):
            for filename in files:
                # Join the two strings in order to form the full filepath.
                if filename.endswith('.proto'):
                    filepath = os.path.join(root, filename)
                    filepath = filepath[1:]
                    self.protoFiles.append(filepath)
        for proto in self.protoFiles:
            inFile = outPath + proto
            outFile = outPath + proto
            # make a copy of our inFile, which will be read and later deleted
            shutil.copy(inFile, inFile + '_temp')
            inFile = inFile + '_temp'
            if pool is None:
                try:
                    self.convert(inFile, outFile, headerInsert, baseNodeInsert, baseNodeTypes, abortKeywords, verbose)
                except Exception as e:
                    failedConvertions.append(sourcePath + proto + ' - line: ' + str(self.lineNumber) + '\n' + str(e))
                    self.move_failed_conversions(sourcePath, outPath, proto, e)
            else:
                multiprocessing.active_children()
                r = pool.apply_async(self.convert, args=(inFile, outFile, verbose))
                failedConvertions.append([r, sourcePath + proto + '\n', sourcePath, outPath, proto])
        return failedConvertions

    def move_failed_conversions(self, sourcePath, outPath, proto, e):
        # create a _FAILED_CONVERSIONS folder inside our output path
        failedBasePath = os.path.join(outPath, '_FAILED_CONVERSIONS')
        outFile = os.path.abspath(outPath + proto)
        # delete all files, which have been created for the failed conversion
        try:
            self.f.close()
            os.remove(outFile)
        except:
            pass
        try:
            os.remove(outFile + '_temp')
        except:
            pass
        meshFolder = outFile.replace('.proto', '')
        if os.path.isdir(meshFolder):
            shutil.rmtree(meshFolder)
        # copy the original source .proto file with its directory tree
        # into the _FAILED_CONVERSIONS folder.
        os.makedirs(failedBasePath + proto, exist_ok=True)
        with open(failedBasePath + proto + "_ErrorLog.txt", "w") as text_file:
            text_file.write(str(e))
        shutil.copy(sourcePath + proto, failedBasePath + proto)

    def create_outputDir(self, sourcePath, outPath):
        # Create a new directory, where the converted files will be stored.
        os.makedirs(outPath, exist_ok=True)
        if sourcePath == outPath:
            newDirName = os.path.basename(sourcePath) + '_multiProto_0'
            newDirPath = os.path.join(os.path.dirname(sourcePath), newDirName)
        else:
            newDirPath = os.path.join(outPath, os.path.basename(sourcePath))
        n = 0
        while os.path.isdir(newDirPath):
            n += 1
            newDirPath = newDirPath[:-1] + str(n)
        shutil.copytree(sourcePath, newDirPath, ignore=shutil.ignore_patterns('*.png', '*.jpg', '*.xcf'))
        return newDirPath


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument(
        dest='input',
        default=None,
        help='Specifies the proto file, or a directory. Converts all .proto files, if it is a directory.',
    )
    parser.add_argument(
        '--output',
        dest='outPath',
        default=None,
        help='Specifies the output path. Only for directory conversions.',
    )
    parser.add_argument(
        '-m', '--multithreaded',
        dest='multithreaded',
        default=False,
        action='store_true',
        help='If set, enables multicore processing for directories with several PROTO files. May not work on Windows.',
    )
    parser.add_argument(
        '-v', '--verbose',
        dest='verbose',
        default=False,
        action='store_true',
        help='If set, detailed output of mesh conversion is shown in console.',
    )
    args = parser.parse_args()
    inPath = args.input
    outPath = args.outPath
    verbose = args.verbose
    multithreaded = args.multithreaded
    tStart = time.time()
    p2m = proto2mesh()
    headerInsert = "  field MFColor    recognitionColors []            # Is `Solid.recognitionColors`.\n"
    baseNodeInsert = "    recognitionColors IS recognitionColors\n"
    baseNodeTypes = ['Solid', 'Robot']
    abortKeywords = ['recognitionColors']
    if multithreaded:
        pool = multiprocessing.Pool(initializer=mutliprocess_initializer, maxtasksperchild=10)
    else:
        pool = None

    try:
        if inPath is not None:
            if os.path.splitext(inPath)[1] == '.proto':
                p2m.convert(inPath, outPath, headerInsert, baseNodeInsert, baseNodeTypes, abortKeywords, verbose=verbose)
                print('Conversion done. Duration: ', round(time.time() - tStart, 3), ' seconds')
            elif os.path.isdir(inPath):
                inPath = os.path.abspath(inPath)
                results = p2m.convert_all(pool, inPath, outPath, headerInsert, baseNodeInsert,
                                          baseNodeTypes, abortKeywords, verbose)
                if multithreaded:
                    pool.close()
                    pool.join()
                    print('\nFailed conversions. Check PROTO formatting:\n')
                    for i in range(len(results)):
                        r = results[i]
                        try:
                            r[0].get()
                        except Exception as e:
                            p2m.move_failed_conversions(r[2], r[3], r[4], e)
                            print('\n', r[1], e)
                else:
                    print('\nFailed conversions. Check PROTO formatting:\n')
                    for path in results:
                        print('\n' + path)
                print('Conversion done. Duration: ', round(time.time() - tStart, 3), ' seconds')
            else:
                sys.exit('Error: --input has to be a .proto file or directory!')
        else:
            sys.exit('Error: Mandatory argument <input> is missing! It should specify a .proto file or directory path.')
    except KeyboardInterrupt:
        print('----------KeyboardInterrupt-------------------------------------------------------')
        pool.terminate()
        pool.join()
        sys.exit("KeyboardInterrupt! Terminating running processes.")
