#!/usr/bin/env python
# -*- coding: utf-8 -*-

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

import fnmatch
import io
import os
import re
import sys

from io import open

# https://github.com/rcompton/ryancompton.net/blob/released/assets/praw_drugs/urlmarker.py
WEB_URL_REGEX = \
    r'(?i)\b((?:https?:(?:/{1,3}|[a-z0-9%])|[a-z0-9.\-]+[.](?:com|net|org|edu|gov|mil|aero|asia|biz|cat|coop|info|' \
    r'int|jobs|mobi|museum|name|post|pro|tel|travel|xxx|ac|ad|ae|af|ag|ai|al|am|an|ao|aq|ar|as|at|au|aw|ax|az|ba|' \
    r'bb|bd|be|bf|bg|bh|bi|bj|bm|bn|bo|br|bs|bt|bv|bw|by|bz|ca|cc|cd|cf|cg|ch|ci|ck|cl|cm|cn|co|cr|cs|cu|cv|cx|' \
    r'cy|cz|dd|de|dj|dk|dm|do|dz|ec|ee|eg|eh|er|es|et|eu|fi|fj|fk|fm|fo|fr|ga|gb|gd|ge|gf|gg|gh|gi|gl|gm|gn|gp|gq|' \
    r'gr|gs|gt|gu|gw|gy|hk|hm|hn|hr|ht|hu|id|ie|il|im|in|io|iq|ir|is|it|je|jm|jo|jp|ke|kg|kh|ki|km|kn|kp|kr|kw|ky|' \
    r'kz|la|lb|lc|li|lk|lr|ls|lt|lu|lv|ly|ma|mc|md|me|mg|mh|mk|ml|mm|mn|mo|mp|mq|mr|ms|mt|mu|mv|mw|mx|my|mz|na|nc|' \
    r'ne|nf|ng|ni|nl|no|np|nr|nu|nz|om|pa|pe|pf|pg|ph|pk|pl|pm|pn|pr|ps|pt|pw|py|qa|re|ro|rs|ru|rw|sa|sb|sc|sd|se|' \
    r'sg|sh|si|sj|Ja|sk|sl|sm|sn|so|sr|ss|st|su|sv|sx|sy|sz|tc|td|tf|tg|th|tj|tk|tl|tm|tn|to|tp|tr|tt|tv|tw|tz|ua|' \
    r'ug|uk|us|uy|uz|va|vc|ve|vg|vi|vn|vu|wf|ws|ye|yt|yu|za|zm|zw)/)(?:[^\s()<>{}\[\]]+|\([^\s()]*?\([^\s()]+\)' \
    r'[^\s()]*?\)|\([^\s]+?\))+(?:\([^\s()]*?\([^\s()]+\)[^\s()]*?\)|\([^\s]+?\)|[^\s`!()\[\]{};:\'".,<>?«»“”‘’])|' \
    r'(?:(?<!@)[a-z0-9]+(?:[.\-][a-z0-9]+)*[.](?:com|net|org|edu|gov|mil|aero|asia|biz|cat|coop|info|int|jobs|' \
    r'mobi|museum|name|post|pro|tel|travel|xxx|ac|ad|ae|af|ag|ai|al|am|an|ao|aq|ar|as|at|au|aw|ax|az|ba|bb|bd|be|' \
    r'bf|bg|bh|bi|bj|bm|bn|bo|br|bs|bt|bv|bw|by|bz|ca|cc|cd|cf|cg|ch|ci|ck|cl|cm|cn|co|cr|cs|cu|cv|cx|cy|cz|dd|de|' \
    r'dj|dk|dm|do|dz|ec|ee|eg|eh|er|es|et|eu|fi|fj|fk|fm|fo|fr|ga|gb|gd|ge|gf|gg|gh|gi|gl|gm|gn|gp|gq|gr|gs|gt|gu|' \
    r'gw|gy|hk|hm|hn|hr|ht|hu|id|ie|il|im|in|io|iq|ir|is|it|je|jm|jo|jp|ke|kg|kh|ki|km|kn|kp|kr|kw|ky|kz|la|lb|lc|' \
    r'li|lk|lr|ls|lt|lu|lv|ly|ma|mc|md|me|mg|mh|mk|ml|mm|mn|mo|mp|mq|mr|ms|mt|mu|mv|mw|mx|my|mz|na|nc|ne|nf|ng|ni|' \
    r'nl|no|np|nr|nu|nz|om|pa|pe|pf|pg|ph|pk|pl|pm|pn|pr|ps|pt|pw|py|qa|re|ro|rs|ru|rw|sa|sb|sc|sd|se|sg|sh|si|sj|' \
    r'Ja|sk|sl|sm|sn|so|sr|ss|st|su|sv|sx|sy|sz|tc|td|tf|tg|th|tj|tk|tl|tm|tn|to|tp|tr|tt|tv|tw|tz|ua|ug|uk|us|uy|' \
    r'uz|va|vc|ve|vg|vi|vn|vu|wf|ws|ye|yt|yu|za|zm|zw)\b/?(?!@)))'

DESCRIPTION_STATE = 0
FIELDS_STATE = 1
BODY_STATE = 2

fileList = []
upperCategories = {'projects': ['appearances']}

# look for all the PROTO files in the 'projects/objects' directory
for rootPath, dirNames, fileNames in os.walk(os.environ['WEBOTS_HOME'] + os.sep + 'projects' + os.sep + 'objects'):
    for fileName in fnmatch.filter(fileNames, '*.proto'):
        fileList.append(os.path.join(rootPath, fileName))
# look for all the PROTO files in the 'projects/appearances' directory
for rootPath, dirNames, fileNames in os.walk(os.environ['WEBOTS_HOME'] + os.sep + 'projects' + os.sep + 'appearances'):
    for fileName in fnmatch.filter(fileNames, '*.proto'):
        fileList.append(os.path.join(rootPath, fileName))
fileList = sorted(fileList)

# create the 'appearances' page
with open('appearances.md', 'w', encoding='utf-8') as file:
    file.write(u'# Appearances\n')
    file.write(u'This chapter describes the list of available appearance PROTO nodes based on the '
               '[PBRAppearance](../reference/pbrappearance.md) node.\n\n')

# make sure that if a PROTO has the same name than the title it appears first
prioritaryProtoList = []
for proto in fileList:
    protoName = os.path.basename(proto).split('.')[0]
    categoryName = os.path.basename(os.path.dirname(os.path.dirname(proto)))
    upperCategoryName = os.path.basename(os.path.dirname(os.path.dirname(os.path.dirname(proto))))
    if protoName.lower() == categoryName and upperCategoryName == 'objects':
        prioritaryProtoList.append(proto)
        fileList.remove(proto)

# get list of base nodes
baseNodeList = []
for rootPath, dirNames, fileNames in os.walk(os.path.join(os.environ['WEBOTS_HOME'], 'resources', 'nodes')):
    for fileName in fnmatch.filter(fileNames, '*.wrl'):
        baseNodeList.append(os.path.splitext(fileName)[0])

# loop through all PROTO files
for proto in prioritaryProtoList + fileList:
    protoName = os.path.basename(proto).split('.')[0]
    category = os.path.basename(os.path.dirname(os.path.dirname(proto)))
    upperCategory = os.path.basename(os.path.dirname(os.path.dirname(os.path.dirname(proto))))
    if upperCategory == 'objects':
        upperCategory = category
    upperCategoryName = upperCategory.replace('_', '-')
    description = ''
    license = ''
    licenseUrl = ''
    fields = u''
    state = 0
    describedField = []
    fieldEnumeration = {}
    skipProto = False
    # parse the PROTO file
    with io.open(proto, 'r', newline='') as file:
        content = file.read()
        # header
        matches = re.finditer(r'^#.*\n', content, re.MULTILINE)
        for i, match in enumerate(matches):
            line = match.group()
            if line.startswith('#VRML_SIM'):
                continue
            elif line.startswith('# license:'):
                license = line.replace('# license:', '').strip()
            elif line.startswith('# license url:'):
                licenseUrl = line.replace('# license url:', '').strip()
            elif line.startswith('# tags:'):
                if 'deprecated' in line or 'hidden' in line:
                    skipProto = True
                    break
            else:
                newLine = line.replace('#', '').replace('_', '\\_').strip()
                urls = re.findall(WEB_URL_REGEX, newLine)
                for url in urls:
                    newLine = newLine.replace(url, '[%s](%s)' % (url, url))
                description += newLine + '\n'
        if skipProto:
            imagePath = 'images/objects/%s/%s/model.png' % (category, protoName)
            if upperCategory == 'projects':
                imagePath = 'images/%s/%s.png' % (category, protoName)
            if os.path.exists(imagePath):
                os.remove(imagePath)
            thumbnailPath = imagePath.replace('.png', '.thumbnail.png')
            if os.path.exists(thumbnailPath):
                os.remove(thumbnailPath)
            continue
        # fields
        matches = re.finditer(r'\[\n((.*\n)*)\]', content, re.MULTILINE)
        for i, match in enumerate(matches):
            fieldsDefinition = match.group(1)
            break  # only first match is interesting
        # remove enumerations
        matches = re.finditer(r'.*ield\s+([^ ]*?)(\{(?:[^\[\n]*\,?\s?)(?<!(\{))\})\s+([^ ]*)\s+([^#\n]*)(#?)(.*)',
                              fieldsDefinition, re.MULTILINE)
        for i, match in enumerate(matches):
            fieldEnumeration[match.group(4)] = match.group(2)[1:-1].split(',')  # keep the list of possibilities
            if '\n' in match.group():
                string = ' ' * match.group().index(match.group(2))
                fieldsDefinition = fieldsDefinition.replace(string + match.group(4), match.group(4))
                fieldsDefinition = fieldsDefinition.replace(match.group(2) + '\n', '')
            if len(match.group(2)) < 40:
                fieldsDefinition = fieldsDefinition.replace(match.group(2), ' ' * len(match.group(2)))
            else:
                fieldsDefinition = fieldsDefinition.replace(match.group(2), '')
        # count minimum space number between field type and name
        matches = re.finditer(r'.*ield\s+([^ ]*?)(\s+)([^ ]*)\s+([^#\n]*)(#?)(.*)',
                              fieldsDefinition, re.MULTILINE)
        minSpaces = 2000
        for i, match in enumerate(matches):
            spaces = match.group(2)
            if len(spaces) < minSpaces:
                minSpaces = len(spaces)
        spacesToRemove = max(minSpaces - 2, 0)
        # create the final cleaned PROTO header
        matches = re.finditer(r'^\s*(.*?ield)\s+([^ \{]*)(\s+)([^ ]*)\s+([^#\n]*)(#?)(.*)((\n*(    |  \]).*)*)',
                              fieldsDefinition, re.MULTILINE)
        for i, match in enumerate(matches):
            if match.group(1) not in ['hiddenField', 'deprecatedField']:
                fieldType = match.group(2)
                spaces = match.group(3)
                fieldName = match.group(4)
                fieldDefaultValue = match.group(5)
                fieldComment = match.group(7).strip()
                # skip 'Is `NodeType.fieldName`.' descriptions
                if fieldComment and not re.match(r'Is\s`([a-zA-Z]*).([a-zA-Z]*)`.', fieldComment):
                    # add link to base nodes:
                    for baseNode in baseNodeList:
                        link = ' [' + baseNode + '](../reference/' + baseNode.lower() + '.md)'
                        fieldComment = fieldComment.replace(' ' + baseNode, link)
                    describedField.append([fieldType, fieldName, fieldComment])
                # remove the comment
                fieldString = match.group()
                fieldString = re.sub(r'\s*(#.*)', '', match.group(), 0, re.MULTILINE)
                # remove intial '*field' string
                fieldString = re.sub(r'^\s*.*field\s', '  ', fieldString, 0, re.MULTILINE)
                # remove unwanted spaces between field type and field name (if needed)
                if spacesToRemove > 0:
                    fieldString = fieldString.replace(fieldType + ' ' * spacesToRemove, fieldType)
                fields += fieldString + '\n'

    baseType = ''
    # use the cache file to get the baseType
    cacheFile = proto.replace(os.path.basename(proto), '.' + os.path.basename(proto)).replace('.proto', '.cache')
    if os.path.isfile(cacheFile):
        with io.open(cacheFile, 'r', newline='') as file:
            for line in file.readlines():
                match = re.match(r'baseType:\s*([a-zA-Z]*)', line)
                if match:
                    baseType = match.group(1)
                    break
    else:
        sys.stderr.write('Could not find cache file: "%s"\n' % cacheFile)

    # add documentation for this PROTO file
    mode = 'a'
    filename = 'object-' + upperCategoryName + '.md'
    if upperCategory == 'projects':
        filename = 'appearances.md'
    elif upperCategory not in upperCategories:
        mode = 'w'
    with open(filename, mode, encoding='utf-8') as file:
        if upperCategory not in upperCategories and not upperCategory == category and not upperCategory == 'projects':
            file.write(u'# %s\n\n' % upperCategory.replace('_', ' ').title())
        headerPrefix = u'#'
        if not upperCategory == category and not upperCategory == 'projects':
            headerPrefix = u'##'

        if upperCategory not in upperCategories or category not in upperCategories[upperCategory]:
            file.write(headerPrefix + u' %s\n\n' % category.replace('_', ' ').title())
        if protoName not in [upperCategory.replace('_', ' ').title(), category.replace('_', ' ').title()]:
            file.write(headerPrefix + u'# %s\n\n' % protoName)
        else:
            # Avoid writing twice the same title at different level
            file.write(headerPrefix + u'# %s PROTO\n\n' % protoName)

        file.write(description + '\n')

        imagePath = 'images/objects/%s/%s/model.png' % (category, protoName)
        if upperCategory == 'projects':  # appearances
            imagePath = 'images/%s/%s.png' % (category, protoName)
        thumbnailPath = imagePath.replace('.png', '.thumbnail.png')
        if os.path.isfile(thumbnailPath):
            imagePath = thumbnailPath
        if os.path.isfile(imagePath):
            file.write(u'%figure\n\n')
            file.write(u'![%s](%s)\n\n' % (protoName, imagePath))
            file.write(u'%end\n\n')
        else:
            # maybe multiple images
            if os.path.exists(os.path.dirname(imagePath)):
                availableImages = [os.path.dirname(imagePath) + '/' + f for f in os.listdir(os.path.dirname(imagePath))]
                regex = imagePath.replace('.png', '_..png')
                files = []
                for image in availableImages:
                    if re.match(regex, image):
                        thumbnailPath = image.replace('.png', '.thumbnail.png')
                        if os.path.isfile(thumbnailPath):
                            image = thumbnailPath
                        files.append(image)
                if files:
                    files.sort()  # alphabetically ordered
                    file.write(u'%figure\n\n')
                    file.write(u'|     |     |\n')
                    file.write(u'|:---:|:---:|\n')
                    for i in range(len(files)):
                        image = os.path.basename(files[i]).replace('.thumbnail.png', '.png')
                        if i % 2 == 0:
                            file.write(u'| ![%s](%s) |' % (image, files[i]))
                        else:
                            file.write(u'![%s](%s) |\n' % (image, files[i]))
                    if not len(files) % 2 == 0:
                        file.write(u' |\n')
                    file.write(u'\n%end\n\n')
                else:
                    sys.stderr.write('Please add a "%s" file.\n' % imagePath)
            else:
                sys.stderr.write('Please add a "%s" file.\n' % imagePath)

        if baseType:
            file.write(u'Derived from [%s](../reference/%s.md).\n\n' % (baseType, baseType.lower()))

        file.write(u'```\n')
        file.write(u'%s {\n' % protoName)
        file.write(fields)
        file.write(u'}\n')
        file.write(u'```\n\n')
        location = proto.replace(os.environ['WEBOTS_HOME'], '').replace(os.sep, '/')
        file.write(u'> **File location**: "[WEBOTS\\_HOME%s](https://github.com/cyberbotics/webots/tree/released%s)"\n\n' %
                   (location.replace('_', '\\_'), location))
        if license:
            file.write(u'> **License**: %s\n' % license)
            if licenseUrl:
                file.write(u'[More information.](%s)\n' % licenseUrl)
            file.write(u'\n')
        else:
            sys.stderr.write('Please add a license to "%s"\n' % proto)

        if describedField:
            file.write(headerPrefix + u'## %s Field Summary\n\n' % protoName)
            for fieldType, fieldName, fieldDescription in describedField:
                file.write(u'- `%s`: %s' % (fieldName, fieldDescription))
                isMFField = fieldType.startswith('MF')
                if fieldName in fieldEnumeration:
                    values = fieldEnumeration[fieldName]
                    if isMFField:
                        file.write(u' This field accepts a list of ')
                    else:
                        if len(values) > 1:
                            file.write(u' This field accepts the following values: ')
                        else:
                            file.write(u' This field accepts the following value: ')
                    for i in range(len(values)):
                        value = values[i].split('{')[0]  # In case of node keep only the type
                        if i == len(values) - 1:
                            if isMFField:
                                file.write(u'`%s` %s.' % (value.strip(), fieldType.replace('MF', '').lower() + 's'))
                            else:
                                file.write(u'`%s`.' % value.strip())
                        elif i == len(values) - 2:
                            file.write(u'`%s` and ' % value.strip())
                        else:
                            file.write(u'`%s`, ' % value.strip())
                file.write(u'\n\n')

    if upperCategory not in upperCategories:
        upperCategories[upperCategory] = []
        upperCategories[upperCategory].append(category)
    elif category not in upperCategories[upperCategory]:
        upperCategories[upperCategory].append(category)

# write the menu in 'object.md'
del upperCategories['projects']
upperCategoriesList = sorted(upperCategories.keys())
categoriesList = []
with open('objects.md', 'w', encoding='utf-8') as file:
    file.write(u'# Objects\n\n')
    file.write(u'## Sections\n\n')
    for upperCategory in upperCategoriesList:
        categories = sorted(upperCategories[upperCategory])
        if not upperCategory == categories[0]:
            file.write(u'- [%s](object-%s.md)\n' % (upperCategory.replace('_', ' ').title(), upperCategory.replace('_', '-')))
        for category in categories:
            categoriesList.append(category)
            if upperCategory == category:
                file.write(u'- [%s](object-%s.md)\n' % (category.replace('_', ' ').title(), category.replace('_', '-')))
            else:
                file.write(u'  - [%s](object-%s.md#%s)\n' % (category.replace('_', ' ').title(),
                                                             upperCategory.replace('_', '-'), category.replace('_', '-')))
    file.write(u'\n')

# print the updated part of 'menu.md'
categoriesList = sorted(categoriesList)
print("Please update the 'Objects' part in 'menu.md' with:")
for category in upperCategoriesList:
    print('    - [%s](object-%s.md)' % (category.replace('_', ' ').title(), category.replace('_', '-')))
