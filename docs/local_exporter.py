#!/usr/bin/env python

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

"""Copy 'index.template.html' to 'index.html' and get JS dependencies locally."""

import os
import platform
import shutil
import ssl
import sys

try:
    # For Python 3.0 and later
    from urllib.request import urlopen, HTTPError, URLError, Request
except ImportError:
    # Fall back to Python 2's urllib2
    from urllib2 import urlopen, HTTPError, URLError, Request


def download(url, target_file_path):
    """Download URL to file."""
    if not silent:
        print('# downloading %s' % url)

    # Prepare the target directory
    target_directory = os.path.dirname(target_file_path)
    if not os.path.exists(target_directory):
        os.makedirs(target_directory)

    # Sometimes Travis cannot get the file at the first trial
    nTrials = 3
    for i in range(nTrials):
        try:
            request = Request(url, headers={'User-Agent': 'Mozilla'})
            if platform.system() == 'Linux' and \
               hasattr(ssl, 'create_default_context'):
                # On Ubuntu 16.04 there are issues with the certificates.
                ctx = ssl.create_default_context()
                ctx.check_hostname = False
                ctx.verify_mode = ssl.CERT_NONE
                response = urlopen(request, timeout=5, context=ctx)
            else:
                response = urlopen(request, timeout=5)
            content = response.read()

            f = open(target_file_path, 'wb')
            f.write(content)
            os.chmod(target_file_path, 0o644)
            f.close()

            break
        except HTTPError as e:
            sys.stderr.write('HTTPError = ' + str(e.reason) + '\n')
        except URLError as e:
            sys.stderr.write('URLError = ' + str(e.reason) + '\n')
        if i == nTrials - 1:
            sys.exit('Cannot get url: ' + url)
    if i > 0:
        sys.stderr.write('# (number of trials: %d)' % (i + 1))


if __name__ == "__main__":
    silent = '--silent' in sys.argv
    script_directory = os.path.dirname(os.path.realpath(__file__)) + os.sep

    with open(script_directory + 'index.template.html', 'r') as file:
        content = file.read()

    dependencies = []
    for dependencies_file in ['dependencies.txt']:
        with open(script_directory + dependencies_file, 'r') as f:
            for line in f:
                line = line.replace('\n', '')
                if line and not line.startswith('#'):
                    dependencies.append(line)
    jsString = ''
    cssString = ''
    repositories = [
        'https://cyberbotics.com/',
        'https://cdnjs.cloudflare.com/ajax/libs/'
    ]
    for dependency in dependencies:
        if dependency.endswith('.css'):
            d = dependency
            for repo in repositories:
                d = d.replace(repo, '')
            cssString += '<link type="text/css" rel="stylesheet" href="dependencies/%s"/>' % (d)
        if dependency.endswith('.js'):
            d = dependency
            for repo in repositories:
                d = d.replace(repo, '')
            jsString += '<script src="dependencies/%s"></script>' % (d)

    content = content.replace('%{ JS }%', jsString)
    content = content.replace('%{ CSS }%', cssString)

    html_file_path = script_directory + 'index.html'
    with open(html_file_path, 'w') as file:
        file.write(content)
        os.chmod(html_file_path, 0o644)

    dependencyDirectory = script_directory + 'dependencies'
    if os.path.exists(dependencyDirectory):
        shutil.rmtree(dependencyDirectory)
    for dependency in dependencies:
        for repository in repositories:
            if dependency.startswith(repository):
                local_path = dependency[len(repository):]
        path = dependencyDirectory + os.sep + local_path.replace("/", os.sep)
        download(dependency, path)
