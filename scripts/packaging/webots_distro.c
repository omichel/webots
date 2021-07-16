/*
 * Copyright 1996-2021 Cyberbotics Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// This tool is used to generate the various installation files:
// a .deb file which is a shell script for creating a deb and a tarball package
// a .iss file for InnoSetup
// a .mac file, which is a shell script for creating the webots directory

#include <ctype.h>  // tolower
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifdef __APPLE__
#include <CommonCrypto/CommonDigest.h>
#elif defined(__linux__)
#include <openssl/md5.h>
#else  // _WIN32
#include <windows.h>
#include "openssl/md5.h"
#endif

#ifdef __APPLE__
#define MD5_CTX CC_MD5_CTX
#define MD5_Init(a) CC_MD5_Init(a)
#define MD5_Update(a, b, c) CC_MD5_Update(a, b, c)
#define MD5_Final(a, b) CC_MD5_Final(a, b)
#endif

#ifdef _WIN32
#define DIR_SEP '\\'
#else
#define DIR_SEP '/'
#endif

#define SetForegroundColorToRed() printf("\033[22;31;1m")
#define SetForegroundColorToDefault() printf("\033[22;30;0m")

// added the bool type
#ifndef bool
#define bool char
#endif

#ifndef true
// clang-format off
#define true ((bool)1)
// clang-format on
#endif

#ifndef false
// clang-format off
#define false ((bool)0)
// clang-format on
#endif

// globals

static FILE *fd;
static int mode;  // either DEB, ISS or MAC
#ifdef __x86_64__
static const char *arch = "amd64";
static const char *arch2 = "x86-64";
#else
static const char *arch = "i386";
static const char *arch2 = "i386";
#endif
static const char *webots_home;
static char application_name[32];
static char bundle_name[32];
static char application_name_lowercase_and_dashes[32];
static char distribution_path[256];

#define ISS 1   // Windows Inno Setup file format                           -> webots*.iss
#define MAC 2   // macOS shell script to create dmg file                    -> webots*.mac
#define DEB 3   // Linux shell script to create a deb and a tarball package -> webots.deb
#define SNAP 4  // Linux shell script to create a snap package              -> webots.snap

// ORable flags for file type defined in files.txt [linux,mac,windows,exe,dll,sig]
#define TYPE_LINUX 1
#define TYPE_MAC 2
#define TYPE_WINDOWS 4
#define TYPE_EXE 8
#define TYPE_DLL 16

#define UNIX2DOS(s, d)  \
  {                     \
    int _i = 0;         \
    do                  \
      if (s[_i] == '/') \
        d[_i] = '\\';   \
      else              \
        d[_i] = s[_i];  \
    while (s[_i++]);    \
  }

static int year = 0;
#define BUFFER_SIZE 2048

static bool dir_exists(const char *path) {
  struct stat info;

  if (stat(path, &info) != 0)
    return false;
  else if (info.st_mode & S_IFDIR)
    return true;
  else
    return false;
}

// source: http://coding.debuntu.org/c-implementing-str_replace-replace-all-occurrences-substring
// Note: str_replace() creates a new string with malloc which should be freed.
char *str_replace(const char *string, const char *substr, const char *replacement) {
  char *tok = NULL;
  char *newstr = NULL;
  char *oldstr = NULL;
  char *head = NULL;

  if (substr == NULL || replacement == NULL)
    return strdup(string);
  newstr = strdup(string);
  head = newstr;
  while ((tok = strstr(head, substr))) {
    oldstr = newstr;
    newstr = malloc(strlen(oldstr) - strlen(substr) + strlen(replacement) + 1);
    if (newstr == NULL) {
      free(oldstr);
      return NULL;
    }
    memcpy(newstr, oldstr, tok - oldstr);
    memcpy(newstr + (tok - oldstr), replacement, strlen(replacement));
    memcpy(newstr + (tok - oldstr) + strlen(replacement), tok + strlen(substr),
           strlen(oldstr) - strlen(substr) - (tok - oldstr));
    memset(newstr + strlen(oldstr) - strlen(substr) + strlen(replacement), 0, 1);
    head = newstr + (tok - oldstr) + strlen(replacement);
    free(oldstr);
  }
  return newstr;
}

static void test_file(const char *file) {
  char file1[BUFFER_SIZE];
  int i;
  if (file[0] == '/' || file[0] == '$')
    return;  // ignore absolute file names
  for (i = 0; file[i]; i++)
    if (file[i] == '*')
      return;  // ignore wildcard filenames
#ifndef _WIN32
  char *f = str_replace(file, "\\ ", " ");
  sprintf(file1, "../../%s", f);
  free(f);
#else
  sprintf(file1, "../../%s", file);
#endif
  if (access(file1, F_OK) == 0)
    return;
  SetForegroundColorToRed();
  printf("Missing file: %s\n", file);
  SetForegroundColorToDefault();
  exit(-1);
}

static void test_dir(const char *dir) {
  char dir1[BUFFER_SIZE];
  if (strcmp(dir, "util") == 0)
    return;  // this one is created and copied from elsewhere
  sprintf(dir1, "../../%s", dir);
  if (access(dir1, F_OK) == 0)
    return;
  SetForegroundColorToRed();
  printf("Missing dir: %s\n", dir);
  SetForegroundColorToDefault();
  exit(-1);
}

static void copy_file(const char *file) {
  char file2[BUFFER_SIZE];
  char dest[BUFFER_SIZE];
  char dest2[BUFFER_SIZE];
  int i, l, k;

  test_file(file);
  l = strlen(file);
  sprintf(dest, "%s", file);
  k = 0;
  for (i = l - 1; i > 0; i--)
    if (dest[i] == '/') {
      if (k == 0)
        dest[i] = '\0';
      k++;
    }
  if (k == 0)
    dest[0] = '\0';
  if (mode != ISS) {  // protect the $ character (mainly for java nested classes)
    k = 0;
    for (i = 0; i < l; i++) {
      if (file[i] == '$')
        file2[k++] = '\\';
      file2[k++] = file[i];
    }
    file2[k] = '\0';
    k = 0;
    for (i = 0; i < ((int)strlen(dest)); i++) {
      if (dest[i] == '$')
        dest2[k++] = '\\';
      dest2[k++] = dest[i];
    }
    dest2[k] = '\0';
  }
#ifndef _WIN32
  // protect parentheses in unix systems
  char *protected_filename1 = str_replace(file2, "(", "\\(");
  char *protected_filename2 = str_replace(protected_filename1, ")", "\\)");
  free(protected_filename1);
#endif
  switch (mode) {
    case ISS:
      UNIX2DOS(file, file2);
      UNIX2DOS(dest, dest2);
      l = strlen(file2);
      for (i = l - 1; file2[i] != '\\'; i--)
        ;
      fprintf(fd, "Source: \"%s\"; DestDir: \"{app}\\%s\"", file2, dest2);
      if (file2[i + 1] == '.')
        fprintf(fd, "; Attribs: hidden");
      if ((file2[l - 4] == '.' && file2[l - 3] == 'j' && file2[l - 2] == 'p' && file2[l - 1] == 'g') ||
          (file2[l - 4] == '.' && file2[l - 3] == 'p' && file2[l - 2] == 'n' && file2[l - 1] == 'g'))
        fprintf(fd, "; Flags: nocompression");
      fprintf(fd, "\n");
      break;
#ifndef _WIN32
    case MAC:
      fprintf(fd, "cp -a $WEBOTS_HOME/%s \"%s/%s/%s/\"\n", protected_filename2, distribution_path, bundle_name, dest2);
      break;
    case DEB:
      fprintf(fd, "cp -a $WEBOTS_HOME/%s %s/debian/usr/local/%s/%s\n", protected_filename2, distribution_path,
              application_name_lowercase_and_dashes, dest2);
      break;
    case SNAP:
      fprintf(fd, "cp -a $WEBOTS_HOME/%s $DESTDIR/usr/share/%s/%s\n", protected_filename2,
              application_name_lowercase_and_dashes, dest2);
      break;
#endif
    default:
      break;
  }
#ifndef _WIN32
  free(protected_filename2);
#endif
}

static bool compute_md5_of_file(const char *file_name, unsigned char *out) {
  FILE *file;
  file = fopen(file_name, "r");
  if (file == NULL)
    return false;

  unsigned char buffer[8192];
  unsigned char bufferUnix[8192];

  MD5_CTX mc;
  MD5_Init(&mc);

  size_t len = fread(buffer, 1, sizeof(buffer), file);
  while (len > 0) {
    // convert all line endings to Unix '\n'
    int i = 0;
    int lenUnix = 0;
    while (i < len) {
      if (buffer[i] != '\r') {
        // copy char
        bufferUnix[lenUnix] = buffer[i];
        ++lenUnix;
      }
      ++i;
    }

    MD5_Update(&mc, bufferUnix, lenUnix);
    len = fread(buffer, 1, sizeof(buffer), file);
  }

  fclose(file);
  MD5_Final(out, &mc);
  return true;
}

static void make_dir(const char *directory) {
  char directory2[BUFFER_SIZE];
  test_dir(directory);
  switch (mode) {
    case MAC:
      fprintf(fd, "mkdir \"%s/%s/%s\"\n", distribution_path, bundle_name, directory);
      break;
    case ISS:
      UNIX2DOS(directory, directory2);
      fprintf(fd, "Name: \"{app}\\%s\"\n", directory2);
      break;
    case DEB:
      fprintf(fd, "mkdir %s/debian/usr/local/%s/%s\n", distribution_path, application_name_lowercase_and_dashes, directory);
      break;
    case SNAP:
      fprintf(fd, "mkdir $DESTDIR/usr/share/%s/%s\n", application_name_lowercase_and_dashes, directory);
    default:
      break;
  }
}

// this function works only for simple wildcard match,
// like "/home/user/*.class" or "/home/user/file-*.c"
static int wildcard_match(const char *str, const char *pattern) {
  enum State {
    Exact,     // exact match
    Any,       // ?
    AnyRepeat  // *
  };
  const char *s = str;
  const char *p = pattern;
  const char *q = 0;
  int state = 0;
  bool match = true;
  while (match && *p) {
    if (*p == '*') {
      state = AnyRepeat;
      q = p + 1;
    } else if (*p == '?')
      state = Any;
    else
      state = Exact;
    if (*s == 0)
      break;
    switch (state) {
      case Exact:
        match = *s == *p;
        s++;
        p++;
        break;
      case Any:
        match = true;
        s++;
        p++;
        break;
      case AnyRepeat:
        match = true;
        s++;
        if (*s == *q)
          p++;
        break;
    }
  }
  if (state == AnyRepeat)
    return (*s == *q);
  else if (state == Any)
    return (*s == *p);
  else
    return match && (*s == *p);
}

static char **expand_wildcard_filename(const char *big_buffer, int *n) {
  DIR *dd;
  int i;
  struct dirent *d;
  char **r = NULL;
  *n = 0;

  int l = strlen(big_buffer);
  char *directory_name = (char *)malloc(l + 1);
  strcpy(directory_name, big_buffer);
  char *pattern = NULL;
  for (i = l - 1; i >= 0; i--)
    if (directory_name[i] == '/') {
      directory_name[i] = '\0';
      pattern = &directory_name[i + 1];
      break;
    }
  dd = opendir(directory_name);
  while ((d = readdir(dd))) {
    if (d->d_name[0] == '.')
      continue;  // skip hidden files, . and .. directories
    if (wildcard_match(d->d_name, pattern)) {
      (*n)++;
      r = (char **)realloc(r, (*n) * sizeof(char *));
      l = strlen(directory_name) + 1 + strlen(d->d_name);
      r[(*n) - 1] = (char *)malloc(l + 1);
      snprintf(r[(*n) - 1], l + 1, "%s/%s", directory_name, d->d_name);
    }
  }
  closedir(dd);
  free(directory_name);
  return r;
}

static void add_ros_dependencies(const char *path) {
  fprintf(fd, "mkdir -p %s/projects/default/controllers/ros/lib/ros\n", path);
#ifdef WEBOTS_UBUNTU_16_04
  fprintf(fd, "cp /opt/ros/kinetic/lib/libroscpp.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/kinetic/lib/librosconsole.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/kinetic/lib/libroscpp_serialization.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/kinetic/lib/librostime.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/kinetic/lib/libxmlrpcpp.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/kinetic/lib/libcpp_common.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/kinetic/lib/librosconsole_log4cxx.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/kinetic/lib/librosconsole_backend_interface.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/kinetic/lib/libroscpp.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/kinetic/lib/libroscpp.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/kinetic/lib/libroscpp.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libboost_system.so.1.58.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.58.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libboost_chrono.so.1.58.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.58.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/liblog4cxx.so.10 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.58.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libconsole_bridge.so.0.2 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libapr-1.so.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libaprutil-1.so.0 %s/projects/default/controllers/ros/lib/ros\n", path);
#elif defined(WEBOTS_UBUNTU_18_04)
  fprintf(fd, "cp /opt/ros/melodic/lib/libroscpp.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/melodic/lib/librosconsole.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/melodic/lib/libroscpp_serialization.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/melodic/lib/librostime.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/melodic/lib/libxmlrpcpp.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/melodic/lib/libcpp_common.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/melodic/lib/librosconsole_log4cxx.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/melodic/lib/librosconsole_backend_interface.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libboost_system.so.1.65.1 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.65.1 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libboost_chrono.so.1.65.1 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.65.1 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.65.1 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/liblog4cxx.so.10 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libconsole_bridge.so.0.4 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libapr-1.so.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libaprutil-1.so.0 %s/projects/default/controllers/ros/lib/ros\n", path);
#elif defined(WEBOTS_UBUNTU_20_04)
  fprintf(fd, "cp /opt/ros/noetic/lib/libroscpp.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/noetic/lib/librosconsole.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/noetic/lib/libroscpp_serialization.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/noetic/lib/librostime.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/noetic/lib/libxmlrpcpp.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/noetic/lib/libcpp_common.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/noetic/lib/librosconsole_log4cxx.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /opt/ros/noetic/lib/librosconsole_backend_interface.so %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /lib/x86_64-linux-gnu/libboost_thread.so.1.71.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /lib/x86_64-linux-gnu/libboost_chrono.so.1.71.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /lib/x86_64-linux-gnu/libboost_filesystem.so.1.71.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /lib/x86_64-linux-gnu/liblog4cxx.so.10 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /lib/x86_64-linux-gnu/libboost_regex.so.1.71.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /lib/x86_64-linux-gnu/libconsole_bridge.so.0.4 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /lib/x86_64-linux-gnu/libapr-1.so.0 %s/projects/default/controllers/ros/lib/ros\n", path);
  fprintf(fd, "cp /lib/x86_64-linux-gnu/libaprutil-1.so.0 %s/projects/default/controllers/ros/lib/ros\n", path);
#endif
}

static void create_file(const char *name, int m) {
  char filename[256];
  char version[64];          // for example "R2018a revision 1"
  char package_version[64];  // for example "R2018a-rev1"
  char buffer[BUFFER_SIZE];
  char big_buffer[2 * BUFFER_SIZE];
  FILE *projects_fd;
  FILE *data_fd;
  int i, j, l, type;

  snprintf(application_name, 32, "%s", name);
#ifdef __APPLE__
  snprintf(bundle_name, 32, "%s.app", name);
#endif
  snprintf(application_name_lowercase_and_dashes, 32, "%s", name);
  l = strlen(name);
  for (i = 0; i < l; i++) {
    application_name_lowercase_and_dashes[i] = tolower(application_name_lowercase_and_dashes[i]);
    if (application_name_lowercase_and_dashes[i] == ' ')
      application_name_lowercase_and_dashes[i] = '-';
  }
  mode = m;
  switch (mode) {
    case ISS:
      snprintf(filename, 256, "%s.iss", application_name_lowercase_and_dashes);
      break;
    case MAC:
      snprintf(filename, 256, "%s.mac", application_name_lowercase_and_dashes);
      break;
    case DEB:
      snprintf(filename, 256, "%s.deb", application_name_lowercase_and_dashes);
      break;
    case SNAP:
      snprintf(filename, 256, "%s.snap", application_name_lowercase_and_dashes);
      break;
    default:
      break;
  }
  fd = fopen(filename, "w+");
  if (fd == NULL) {
    fprintf(stderr, "could not open file: %s\n", filename);
    exit(-1);
  }
  data_fd = fopen("webots_version.txt", "r");
  if (data_fd == NULL) {
    fprintf(stderr, "could not open file: webots_version.txt\n");
    exit(-1);
  }
  fgets(version, 64, data_fd);
  l = strlen(version);
  if (version[l - 1] == '\n')
    version[l - 1] = '\0';  // remove final new line character
  printf("# creating %s version %s", filename, version);
  fflush(stdout);
  fclose(data_fd);
  char major[16], revision_string[16];
  int revision_number;
  int matches = sscanf(version, "%15s %15s %d", major, revision_string, &revision_number);
  if (matches > 1)
    sprintf(package_version, "%s-rev%d", major, revision_number);
  else
    sprintf(package_version, "%s", major);
  switch (mode) {
    case MAC:
      fprintf(fd, "#!/bin/bash\n");
      fprintf(fd, "# run this script to build %s-%s.dmg in \"%s/\"\n\n", application_name_lowercase_and_dashes, package_version,
              distribution_path);
      fprintf(fd, "rm -rf \"%s/%s/Contents\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -rf \"%s/%s/webots\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -rf \"%s/%s/docs\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -rf \"%s/%s/bin\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -rf \"%s/%s/bin/qt\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -rf \"%s/%s/include\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -rf \"%s/%s/lib\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -rf \"%s/%s/projects\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -rf \"%s/%s/resources\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -rf \"%s/%s/transfer\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -rf \"%s/%s/util\"\n", distribution_path, bundle_name);
      fprintf(fd, "rm -f \"%s/%s-%s.dmg\"\n\n", distribution_path, application_name_lowercase_and_dashes, package_version);
      fprintf(fd, "mkdir -p \"%s/%s\"\n\n", distribution_path, bundle_name);
      break;
    case ISS:
      fprintf(fd,
              "[Setup]\n"
              "SourceDir=..\\..\n"
              "AppId=Webots\n"
              "AppName=%s\n"
              "AppVersion=%s\n"
              "AppVerName=%s %s\n"
              "AppCopyright=Copyright (c) %d Cyberbotics, Ltd.\n"
              "AppPublisher=Cyberbotics, Ltd.\n"
              "AppPublisherURL=https://www.cyberbotics.com\n"
              "ChangesEnvironment=yes\n"  // tells Windows Explorer to reload environment variables (e.g., WEBOTS_HOME)
              "Compression=lzma2/fast\n"
              "DefaultDirName={autopf}\\%s\n"
              "DefaultGroupName=Cyberbotics\n"
              "UninstallDisplayIcon={app}\\msys64\\mingw64\\bin\\webots-bin.exe\n"
              "PrivilegesRequired=admin\n"
              "UsePreviousPrivileges=no\n"
              "PrivilegesRequiredOverridesAllowed=dialog commandline\n",
              application_name, version, application_name, version, year, application_name);
      fprintf(fd, "OutputBaseFileName=%s-%s_setup\n", application_name_lowercase_and_dashes, package_version);
      fprintf(fd,
              "OutputDir=%s\n"
              "ChangesAssociations=yes\n"
              "DisableStartupPrompt=yes\n"
              "ArchitecturesInstallIn64BitMode=x64\n"
              "ArchitecturesAllowed=x64\n"
              "UsePreviousAppDir=yes\n"
              "\n[Dirs]\n",
              distribution_path);
      break;
    case DEB:
      fprintf(fd, "#!/bin/bash\n");
      fprintf(fd, "# run this auto-generated script to install %s in \"$HOME/develop/%s\"\n\n", application_name,
              application_name_lowercase_and_dashes);
      fprintf(fd, "rm -rf %s/debian # cleanup\n", distribution_path);
      fprintf(fd, "rm -f %s/%s-%s_*.deb\n\n", distribution_path, application_name_lowercase_and_dashes, package_version);
      fprintf(fd, "mkdir %s/debian\n", distribution_path);
      fprintf(fd, "mkdir %s/debian/usr\n", distribution_path);
      fprintf(fd, "mkdir %s/debian/usr/local\n", distribution_path);
      fprintf(fd, "mkdir %s/debian/usr/local/%s\n", distribution_path, application_name_lowercase_and_dashes);
      break;
    case SNAP:
      fprintf(fd, "#!/bin/bash\n");
      fprintf(fd, "# run this auto-generated script to install the %s snap in \"$DESTDIR\"\n\n",
              application_name_lowercase_and_dashes);
      fprintf(fd, "mkdir -p $DESTDIR\n");
      fprintf(fd, "mkdir -p $DESTDIR/lib\n");
      fprintf(fd, "mkdir -p $DESTDIR/lib/x86_64-linux-gnu\n");
      fprintf(fd, "mkdir -p $DESTDIR/usr\n");
      fprintf(fd, "mkdir -p $DESTDIR/usr/share\n");
      fprintf(fd, "mkdir -p $DESTDIR/usr/bin\n");
      fprintf(fd, "mkdir -p $DESTDIR/usr/lib\n");
      fprintf(fd, "mkdir -p $DESTDIR/usr/lib/x86_64-linux-gnu\n");
      fprintf(fd, "mkdir $DESTDIR/usr/share/%s\n", application_name_lowercase_and_dashes);
    default:
      break;
  }
  // creating all the subdirectories listed in files.txt
  projects_fd = fopen("files.txt", "r");
  if (projects_fd == NULL) {
    fprintf(stderr, "Cannot open files.txt file!\n");
    exit(-1);
  }
  while (fgets(buffer, BUFFER_SIZE, projects_fd)) {
    l = strlen(buffer) - 2;  // skipping the final '\n'
    for (i = 0; i < l; i++)
      if (buffer[i] == ' ' && buffer[i + 1] == '#') {
        l = i - 1;
        break;
      }  // skip comment
    type = 0;
    while (buffer[l] == ' ' || buffer[l] == '\t')
      l--;
    if (buffer[0] == '#' || l < 1)
      continue;
    if (buffer[l] == ']') {
      while (buffer[l] != '[' && l > 0)
        l--;
      if (l == 0) {
        fprintf(stderr, "missing opening bracket in files.txt: [\n");
        exit(-1);
      }
      i = l - 1;
      l++;
      do {
        switch (buffer[l++]) {
          case 'l':
            type |= TYPE_LINUX;
            break;
          case 'm':
            type |= TYPE_MAC;
            break;
          case 'w':
            type |= TYPE_WINDOWS;
            break;
          case 'e':  // exe
          case 'd':  // dll
            break;
          default:
            fprintf(stderr, "unknown option: %s\n", &buffer[l - 1]);
            break;
        }
        for (;;)
          if (buffer[l] == ',') {
            l++;
            break;
          } else if (buffer[l] == ']')
            break;
          else
            l++;
      } while (buffer[l] != ']');
      if ((type & (TYPE_LINUX | TYPE_MAC | TYPE_WINDOWS)) == 0)
        type |= (TYPE_LINUX | TYPE_MAC | TYPE_WINDOWS);
      l = i;
    } else {
      l++;
      type = TYPE_LINUX | TYPE_MAC | TYPE_WINDOWS;
    }
    buffer[l--] = '\0';
    if (buffer[l] == '/') {
      buffer[l] = '\0';
      if (((type & TYPE_LINUX) && (mode == DEB || mode == SNAP)) || ((type & TYPE_WINDOWS) && (mode == ISS)) ||
          ((type & TYPE_MAC) && (mode == MAC)))
        make_dir(buffer);
    }
  }
  rewind(projects_fd);
  if (mode == ISS) {
    FILE *f = fopen("msys64_folders.iss", "r");
    while (fgets(buffer, sizeof(buffer), f) != NULL)
      fprintf(fd, "%s", buffer);
    fclose(f);
    fprintf(fd, "\n[Files]\n");
  }
  // copying all the remaining necessary files listed in files.txt
  while (fgets(buffer, BUFFER_SIZE, projects_fd)) {
    l = strlen(buffer) - 2; /* skipping the final '\n' */
    for (i = 0; i < l; i++)
      if (buffer[i] == ' ' && buffer[i + 1] == '#') {
        l = i - 1;
        break;
      }  // skip comment
    while (buffer[l] == ' ' || buffer[l] == '\t')
      l--;
    if (buffer[l] == '/' || buffer[0] == '#' || l < 1)
      continue;
    type = 0;
    if (buffer[l] == ']') {
      while (buffer[l] != '[' && l > 0)
        l--;
      if (l == 0) {
        fprintf(stderr, "missing opening bracket in files.txt: [\n");
        exit(-1);
      }
      i = l - 1;
      l++;
      do {
        switch (buffer[l++]) {
          case 'l':
            type |= TYPE_LINUX;
            break;
          case 'm':
            type |= TYPE_MAC;
            break;
          case 'w':
            type |= TYPE_WINDOWS;
            break;
          case 'e':
            type |= TYPE_EXE;
            break;
          case 'd':
            type |= TYPE_DLL;
            break;
          default:
            fprintf(stderr, "unknown option: %s\n", &buffer[l - 1]);
            break;
        }
        for (;;)
          if (buffer[l] == ',') {
            l++;
            break;
          } else if (buffer[l] == ']')
            break;
          else
            l++;
      } while (buffer[l] != ']');
      if ((type & (TYPE_LINUX | TYPE_MAC | TYPE_WINDOWS)) == 0)
        type |= (TYPE_LINUX | TYPE_MAC | TYPE_WINDOWS);
      if (type & TYPE_DLL && (mode == MAC || mode == DEB || mode == SNAP)) {  // prefix "lib" to the basename
        j = i - 1;
        while (buffer[j] != '/' && j >= 0)
          j--;
        if (buffer[j + 1] != '_') {
          if (j >= 0) {
            j = i - 1;
            while (buffer[j] != '/' && j >= 0) {
              buffer[j + 3] = buffer[j];
              j--;
            }
            buffer[++j] = 'l';
            buffer[++j] = 'i';
            buffer[++j] = 'b';
            i += 3;
          } else
            fprintf(stderr, "DLL parsing: Reached the beggining of the string without finding a '/' character.");
        }
      }
      if (mode == ISS) {
        if (type & TYPE_EXE) {
          buffer[i++] = '.';
          buffer[i++] = 'e';
          buffer[i++] = 'x';
          buffer[i++] = 'e';
        } else if (type & TYPE_DLL) {
          buffer[i++] = '.';
          buffer[i++] = 'd';
          buffer[i++] = 'l';
          buffer[i++] = 'l';
        }
      } else if (mode == MAC) {
        if (type & TYPE_DLL) {
          buffer[i++] = '.';
          buffer[i++] = 'd';
          buffer[i++] = 'y';
          buffer[i++] = 'l';
          buffer[i++] = 'i';
          buffer[i++] = 'b';
        }
      } else { /* linux */
        if (type & TYPE_DLL) {
          buffer[i++] = '.';
          buffer[i++] = 's';
          buffer[i++] = 'o';
        }
      }
      l = i;
    } else {
      l++;
      type = TYPE_LINUX | TYPE_MAC | TYPE_WINDOWS;
    }
    buffer[l] = '\0';
    if (buffer[l - 1] == '/')
      continue;
    if (((type & TYPE_LINUX) && (mode == DEB || mode == SNAP)) || ((type & TYPE_WINDOWS) && (mode == ISS)) ||
        ((type & TYPE_MAC) && (mode == MAC))) {
      copy_file(buffer);
      // copy the .*.wbproj hidden files
      if (strcasecmp(&buffer[l - 4], ".wbt") == 0 && strstr(buffer, "/worlds/")) {
        sprintf(big_buffer, "%s/%s", webots_home, buffer);
        // we need to expand the possible wildcard
        int n;
        char **expanded_filename = expand_wildcard_filename(big_buffer, &n);
        for (i = 0; i < n; i++) {
          int l = strlen(expanded_filename[i]);
          char *project_filename = (char *)malloc(l + 5);  // ".%swbproj", including the final '\0' minus 'wbt'
          strcpy(project_filename, expanded_filename[i]);
          l--;
          for (; l >= 0; l--)
            if (project_filename[l] == '/')
              break;
          l++;
          project_filename[l] = '.';
          do
            project_filename[l + 1] = expanded_filename[i][l];
          while (expanded_filename[i][l++]);
          project_filename[--l] = '\0';  // remove the last 't'
          strcat(project_filename, "proj");
#ifdef _WIN32  // we need to mark this file with the hidden attribute using the Windows C API
          SetFileAttributes(project_filename, FILE_ATTRIBUTE_HIDDEN);
#endif
          copy_file(&project_filename[strlen(webots_home) + 1]);
          free(project_filename);
          free(expanded_filename[i]);
        }
        free(expanded_filename);
      }

      // check and copy the .*.cache hidden files
      if (strcasecmp(&buffer[l - 6], ".proto") == 0 && strstr(buffer, "/protos/")) {
        sprintf(big_buffer, "%s/%s", webots_home, buffer);
        // we need to expand the possible wildcard
        int n;
        char **expanded_filename = expand_wildcard_filename(big_buffer, &n);
        for (i = 0; i < n; i++) {
          int l = strlen(expanded_filename[i]);
          char *project_filename = (char *)malloc(l + 2);  // ".%scache", including the final '\0' minus 'proto'
          strcpy(project_filename, expanded_filename[i]);
          l--;
          for (; l >= 0; l--)
            if (project_filename[l] == '/')
              break;
          l++;
          project_filename[l] = '.';
          do
            project_filename[l + 1] = expanded_filename[i][l];
          while (expanded_filename[i][l++]);
          l -= 5;
          project_filename[l] = '\0';  // remove the last 'proto'
          strcat(project_filename, "cache");

          // compute MD5 of PROTO file
          const int md5_len = 16;
          unsigned char md5Value[md5_len];
          if (!compute_md5_of_file(expanded_filename[i], md5Value)) {
            SetForegroundColorToRed();
            printf("Missing file: %s\n", &expanded_filename[i][strlen(webots_home) + 1]);
            SetForegroundColorToDefault();
            exit(-1);
          }

          // read cache file and check MD5 value
          FILE *cacheFile;
          char line[256];
          int maxLen = 256;
          cacheFile = fopen(project_filename, "r");
          if (cacheFile == NULL) {
            SetForegroundColorToRed();
            printf("Missing file: %s\n", &project_filename[strlen(webots_home) + 1]);
            SetForegroundColorToDefault();
            exit(-1);
          }
          const char *header = "protoFileHash: ";
          char md5Sum[33];
          for (j = 0; j < 16; j++)
            sprintf(&md5Sum[2 * j], "%02x", md5Value[j]);
          md5Sum[32] = '\0';
          while (fgets(line, maxLen, cacheFile) != NULL) {
            if (strncmp(line, header, strlen(header)) == 0) {
              if (strncmp(md5Sum, &line[strlen(header)], 32) != 0) {
                SetForegroundColorToRed();
                printf("Out-of-date file: %s\n", &project_filename[strlen(webots_home) + 1]);
                SetForegroundColorToDefault();
                exit(-1);
              }
            }
          }
          fclose(cacheFile);

#ifdef _WIN32  // we need to mark this file with the hidden attribute using the Windows C API
          SetFileAttributes(project_filename, FILE_ATTRIBUTE_HIDDEN);
#endif
          copy_file(&project_filename[strlen(webots_home) + 1]);
          free(project_filename);
          free(expanded_filename[i]);
        }
        free(expanded_filename);
      }
      // printf("created: %s (%d)\n",buffer,type);
    }
  }
  fclose(projects_fd);

  if (mode == ISS) {
    FILE *f = fopen("msys64_files.iss", "r");
    while (fgets(buffer, sizeof(buffer), f) != NULL)
      fprintf(fd, "%s", buffer);
    fclose(f);
  }

  switch (mode) {
    case MAC:
      fprintf(fd, "cd \"%s/%s/lib/webots\"\n", distribution_path, bundle_name);
      fprintf(fd, "ln -s libssl.1.0.0.dylib libssl.dylib\n");
      fprintf(fd, "ln -s libcrypto.1.0.0.dylib libcrypto.dylib\n");
      fprintf(fd, "cd \"%s/%s/Contents/Frameworks\"\n", distribution_path, bundle_name);
      fprintf(fd, "cd QtConcurrent.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtConcurrent QtConcurrent\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtCore.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtCore QtCore\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtDBus.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtDBus QtDBus\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtGui.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtGui QtGui\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtMultimedia.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtMultimedia QtMultimedia\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtMultimediaWidgets.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtMultimediaWidgets QtMultimediaWidgets\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtNetwork.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtNetwork QtNetwork\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtOpenGL.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtOpenGL QtOpenGL\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtPositioning.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtPositioning QtPositioning\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtPrintSupport.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtPrintSupport QtPrintSupport\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtQml.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtQml QtQml\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtQmlModels.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtQmlModels QtQmlModels\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtQuick.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtQuick QtQuick\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtQuickWidgets.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtQuickWidgets QtQuickWidgets\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtSensors.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtSensors QtSensors\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtSql.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtSql QtSql\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtWebChannel.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtWebChannel QtWebChannel\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtWebEngine.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtWebEngine QtWebEngine\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtWebEngineCore.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtWebEngineCore QtWebEngineCore\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "ln -Fs Versions/5/Helpers Helpers\n");
      fprintf(fd, "ln -Fs Versions/5/Resources Resources\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtWebEngineWidgets.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtWebEngineWidgets QtWebEngineWidgets\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtWebSockets.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtWebSockets QtWebSockets\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtWidgets.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtWidgets QtWidgets\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd ..\n");
      fprintf(fd, "cd QtXml.framework\n");
      fprintf(fd, "ln -fs Versions/5/QtXml QtXml\n");
      fprintf(fd, "ln -Fs Versions/5/Headers Headers\n");
      fprintf(fd, "cd %s/\n", distribution_path);
      fprintf(fd, "echo \"{\" >> appdmg.json\n");
      fprintf(fd, "echo \"  \\\"title\\\": \\\"Webots\\\",\" >> appdmg.json\n");
      fprintf(fd, "echo \"  \\\"icon\\\": \\\"%s/Contents/Resources/webots_icon.icns\\\",\" >> appdmg.json\n", webots_home);
      fprintf(fd, "echo \"  \\\"icon-size\\\": 72,\" >> appdmg.json\n");
      fprintf(fd, "echo \"  \\\"background\\\": \\\"%s/scripts/packaging/MacOSXBackground.png\\\",\" >> appdmg.json\n",
              webots_home);
      fprintf(fd, "echo \"  \\\"format\\\": \\\"UDBZ\\\",\" >> appdmg.json\n");
      fprintf(fd, "echo \"  \\\"window\\\": {\" >> appdmg.json\n");
      fprintf(fd, "echo \"    \\\"position\\\": { \\\"x\\\": 400, \\\"y\\\": 100 },\" >> appdmg.json\n");
      fprintf(fd, "echo \"    \\\"size\\\": { \\\"width\\\": 480, \\\"height\\\": 580 }\" >> appdmg.json\n");
      fprintf(fd, "echo \"  },\" >> appdmg.json\n");
      fprintf(fd, "echo \"  \\\"contents\\\": [\" >> appdmg.json\n");
      fprintf(fd, "echo \"    { \\\"x\\\": 375, \\\"y\\\": 100, \\\"type\\\": \\\"link\\\", \\\"path\\\": "
                  "\\\"/Applications\\\" },\" >> appdmg.json\n");
      fprintf(fd,
              "echo \"    { \\\"x\\\": 100, \\\"y\\\": 100, \\\"type\\\": \\\"file\\\", \\\"path\\\": \\\"%s\\\" }\" >> "
              "appdmg.json\n",
              bundle_name);
      fprintf(fd, "echo \"  ]\" >> appdmg.json\n");
      fprintf(fd, "echo \"}\" >> appdmg.json\n");
      fprintf(fd, "appdmg appdmg.json %s-%s.dmg\n", application_name_lowercase_and_dashes, package_version);
      fprintf(fd, "rm -rf appdmg.json\n");
      break;
    case ISS:
      fprintf(fd, "\n[Icons]\n");
      fprintf(fd,
              "Name: \"{app}\\%s\"; Filename: \"{app}\\msys64\\mingw64\\bin\\webotsw.exe\"; WorkingDir: \"{app}\"; Comment: "
              "\"Robot simulator\"\n"
              "Name: \"{group}\\%s\"; Filename: \"{app}\\msys64\\mingw64\\bin\\webotsw.exe\"; WorkingDir: \"{app}\"; Comment: "
              "\"Robot simulator\"\n"
              "Name: \"{userdesktop}\\%s\"; Filename: \"{app}\\msys64\\mingw64\\bin\\webotsw.exe\"; WorkingDir: \"{app}\"; "
              "Comment: \"Robot simulator\"\n",
              application_name, application_name, application_name);
      fprintf(
        fd, "Name: \"{group}\\Uninstall %s\"; Filename: \"{uninstallexe}\"; WorkingDir: \"{app}\"; Comment: \"Uninstall %s\"\n",
        application_name, application_name);
      fprintf(fd,
              "\n[Registry]\n"
              "Root: HKA; SubKey: \"Software\\Classes\\.wbt\"; ValueType: string; ValueData: \"webotsfile\"; "
              "Flags: uninsdeletekey\n"
              "Root: HKA; SubKey: \"Software\\Classes\\.wbt\"; ValueType: string; ValueName: \"Content Type\"; "
              "ValueData: \"application/webotsfile\"; Flags: uninsdeletekey\n"
              "Root: HKA; SubKey: \"Software\\Classes\\webotsfile\\DefaultIcon\"; ValueType: string; "
              "ValueData: \"{app}\\resources\\icons\\core\\webots_doc.ico\"; Flags: uninsdeletekey\n"
              "Root: HKA; SubKey: \"Software\\Classes\\webotsfile\\shell\\open\"; ValueType: string; "
              "ValueName: \"FriendlyAppName\"; ValueData: \"Webots\"; Flags: uninsdeletekey\n"
              "Root: HKA; SubKey: \"Software\\Classes\\webotsfile\\shell\\open\\command\"; ValueType: string; ValueData: "
              "\"\"\"{app}\\msys64\\mingw64\\bin\\webotsw.exe\"\" \"\"%%1\"\"\"; Flags: uninsdeletekey\n"
              "Root: HKA; SubKey: \"Software\\Classes\\Applications\\webotsw.exe\"; ValueType: string; "
              "ValueName: \"SupportedTypes\"; ValueData: \".wbt\"; Flags: uninsdeletekey\n"
              "Root: HKA; SubKey: \"Software\\Classes\\Applications\\webotsw.exe\"; ValueType: string; "
              "ValueName: \"FriendlyAppName\"; ValueData: \"Webots\"; Flags: uninsdeletekey\n"
              "Root: HKCU; SubKey: \"Software\\Cyberbotics\"; Flags: uninsdeletekeyifempty dontcreatekey\n"
              "Root: HKCU; SubKey: \"Software\\Cyberbotics\\%s %s\"; Flags: uninsdeletekey dontcreatekey\n"
              "Root: HKA; SubKey: \"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\"; ValueType: string; "
              "ValueName: \"WEBOTS_HOME\"; ValueData: \"{app}\"; Flags: preservestringtype\n",
              application_name, version);
      fprintf(fd, "Root: HKA; SubKey: \"Software\\Classes\\webotsfile\"; "
                  "Flags: uninsdeletekey dontcreatekey\n");
      // On some systems (as already reported by two Chinese users), some unknown third party software badly installs a
      // zlib1.dll and libeay32.dll in the C:\Windows\System32 folder.
      // A similar problem occurs with the OpenSSL library needed to build ROS2 on Windows:
      // https://index.ros.org/doc/ros2/Installation/Dashing/Windows-Install-Binary/#install-openssl
      // recommends to install OpenSSL from https://slproweb.com/products/Win32OpenSSL.html
      // By default, this installer copies libcrypto-1_1-x64.dll and libssl-1_1-x64.dll in C:\Windows\System32.
      // Similarly, libjpeg-8.dll may be found there.
      // This is a very bad practise as such DLLs conflicts with the same DLLs provided in the msys64 folder of Webots.
      // So, we will delete any of these libraries from the C:\Windows\System32 folder before installing Webots.
      fprintf(fd, "\n[InstallDelete]\n");
      fprintf(fd, "Type: files; Name: \"{sys}\\zlib1.dll\"\n");
      fprintf(fd, "Type: files; Name: \"{sys}\\libeay32.dll\"\n");
      fprintf(fd, "Type: files; Name: \"{sys}\\libcrypto-1_1-x64.dll\"\n");
      fprintf(fd, "Type: files; Name: \"{sys}\\libssl-1_1-x64.dll\"\n");
      fprintf(fd, "Type: files; Name: \"{sys}\\libjpeg-8.dll\"\n");
      fprintf(fd, "\n[Code]\n");
      fprintf(fd, "function InitializeSetup(): Boolean;\n");
      fprintf(fd, "var\n");
      fprintf(fd, "  ResultCode: Integer;\n");
      fprintf(fd, "  Uninstall: String;\n");
      fprintf(fd, "begin\n");
      fprintf(fd, "  if isAdmin and RegQueryStringValue(HKLM, 'Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"
                  "Webots_is1', 'UninstallString', Uninstall) then begin\n");
      fprintf(fd, "    if MsgBox('A version of Webots is already installed for all users on this computer. "
                  "It will be removed and replaced by the version you are installing.', mbInformation, MB_OKCANCEL) = IDOK "
                  "then begin\n");
      fprintf(fd, "      Exec(RemoveQuotes(Uninstall), ' /SILENT', '', SW_SHOWNORMAL, ewWaitUntilTerminated, ResultCode);\n");
      fprintf(fd, "      Result := TRUE;\n");
      fprintf(fd, "    end else begin\n");
      fprintf(fd, "      Result := FALSE;\n");
      fprintf(fd, "    end;\n");
      fprintf(fd, "  end else begin\n");
      fprintf(fd, "    Result := TRUE;\n");
      fprintf(fd, "  end;\n");
      fprintf(fd, "  if RegQueryStringValue(HKCU, 'Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\"
                  "Webots_is1', 'UninstallString', Uninstall) then begin\n");
      fprintf(fd, "    if MsgBox('A version of Webots is already installed for the current user on this computer. It "
                  "will be removed and replaced by the version you are installing.', mbInformation, MB_OKCANCEL) = IDOK "
                  "then begin\n");
      fprintf(fd, "      Exec(RemoveQuotes(Uninstall), ' /SILENT', '', SW_SHOWNORMAL, ewWaitUntilTerminated, ResultCode);\n");
      fprintf(fd, "      Result := TRUE;\n");
      fprintf(fd, "    end else begin\n");
      fprintf(fd, "      Result := FALSE;\n");
      fprintf(fd, "    end;\n");
      fprintf(fd, "  end else begin\n");
      fprintf(fd, "    Result := TRUE;\n");
      fprintf(fd, "  end;\n");
      fprintf(fd, "  if RegQueryStringValue(HKLM32, 'Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Webots_is1', "
                  "'UninstallString', Uninstall) then begin\n");
      fprintf(fd, "    if MsgBox('A version of Webots (32 bit) is already installed on this computer. It will be removed "
                  "and replaced by the version (64 bit) you are installing.', mbInformation, MB_OKCANCEL) = IDOK then begin\n");
      fprintf(fd, "      Exec(RemoveQuotes(Uninstall), ' /SILENT', '', SW_SHOWNORMAL, ewWaitUntilTerminated, ResultCode);\n");
      fprintf(fd, "      Result := TRUE;\n");
      fprintf(fd, "    end else begin\n");
      fprintf(fd, "      Result := FALSE;\n");
      fprintf(fd, "    end;\n");
      fprintf(fd, "  end else begin\n");
      fprintf(fd, "    Result := TRUE;\n");
      fprintf(fd, "  end;\n");
      fprintf(fd, "end;\n\n");
      fprintf(fd, "procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);\n");
      fprintf(fd, "var\n");
      fprintf(fd, "  ResultCode: Integer;\n");
      fprintf(fd, "begin\n");
      fprintf(fd, "  if (CurUninstallStep = usPostUninstall) and DirExists(ExpandConstant('{app}')) then begin\n");
      fprintf(fd, "    if MsgBox(ExpandConstant('{app}') + ' was modified!'#13#10#13#10 +\n");
      fprintf(fd, "        'It seems you created or modified some files in this folder.'#13#10#13#10 +\n");
      fprintf(fd, "        'This is your last chance to do a backup of these files.'#13#10#13#10 +\n");
      fprintf(fd, "        'Do you want to delete the whole '+ ExpandConstant('{app}') +' folder now?'#13#10, mbConfirmation, "
                  "MB_YESNO) = IDYES\n");
      fprintf(fd, "    then begin  // User clicked YES\n");
      fprintf(fd, "      // fix read-only status of all files and folders to be able to delete them\n");
      fprintf(fd, "      Exec('cmd.exe', '/c \"attrib -R ' + ExpandConstant('{app}') + '\\*.* /s /d\"', '', SW_HIDE, "
                  "ewWaitUntilTerminated, ResultCode);\n");
      fprintf(fd, "      DelTree(ExpandConstant('{app}'), True, True, True);\n");
      fprintf(fd, "    end else begin  // User clicked NO\n");
      fprintf(fd, "      Abort;\n");
      fprintf(fd, "    end;\n");
      fprintf(fd, "  end;\n");
      fprintf(fd, "end;\n\n");
      fprintf(fd, "\n[Run]\n");
      fprintf(fd, "Filename: {app}\\msys64\\mingw64\\bin\\webotsw.exe; Description: \"Launch Webots\"; Flags: nowait "
                  "postinstall skipifsilent\n");
      break;
    case DEB:
#ifdef WEBOTS_UBUNTU_16_04
      copy_file("lib/webots/libssl.so.1.1");
      copy_file("lib/webots/libcrypto.so.1.1");
#endif
      // copy libraries that depends on OS and cannot be included in files_*.txt
      fprintf(fd, "cd %s/debian\n", distribution_path);
      fprintf(fd, "mkdir usr/share\n");
      fprintf(fd, "mkdir usr/share/mime-info\n");
      fprintf(fd, "mkdir usr/share/pixmaps\n");
      fprintf(fd, "mkdir usr/share/application-registry\n");
      fprintf(fd, "mkdir usr/share/applications\n");
      fprintf(fd, "mkdir usr/share/app-install\n");
      fprintf(fd, "mkdir usr/share/app-install/desktop\n");
      fprintf(fd, "cp $WEBOTS_HOME/scripts/packaging/webots.mime usr/share/mime-info/\n");
      fprintf(fd, "cp $WEBOTS_HOME/scripts/packaging/webots.keys usr/share/mime-info/\n");
      fprintf(fd, "cp $WEBOTS_HOME/scripts/packaging/webots.png usr/share/pixmaps/\n");
      fprintf(fd, "cp $WEBOTS_HOME/scripts/packaging/webots_doc.png usr/share/pixmaps/\n");
      fprintf(fd, "cp $WEBOTS_HOME/scripts/packaging/webots.applications usr/share/application-registry/\n");
      fprintf(fd, "cp $WEBOTS_HOME/scripts/packaging/webots.desktop usr/share/applications/\n");
#ifdef WEBOTS_UBUNTU_16_04
      fprintf(fd, "cp $WEBOTS_HOME/scripts/packaging/webots_snap.desktop usr/share/app-install/desktop/webots.desktop\n");
#else
      fprintf(fd, "cp $WEBOTS_HOME/scripts/packaging/webots.desktop usr/share/app-install/desktop/\n");
#endif
      fprintf(fd, "mkdir usr/local/bin\n");
      fprintf(fd, "ln -s /usr/local/%s/webots usr/local/bin/webots\n", application_name_lowercase_and_dashes);
      fprintf(fd, "cd %s/debian\n", distribution_path);
      // add the wrapper library corresponding to the default Python 3 versions
#ifdef WEBOTS_UBUNTU_16_04
      fprintf(fd, "mkdir usr/local/webots/lib/controller/python35\n");
      fprintf(fd, "cp $WEBOTS_HOME/lib/controller/python35/*.py usr/local/webots/lib/controller/python35/\n");
      fprintf(fd, "cp $WEBOTS_HOME/lib/controller/python35/_*.so usr/local/webots/lib/controller/python35/\n");
      // include system libraries in package that are needed on Ubuntu 18.04
      fprintf(fd, "cd %s/debian/usr/local/%s/lib/webots\n", distribution_path, application_name_lowercase_and_dashes);
      fprintf(fd, "ln -s libssl.so.1.1 libssl.so\n");
      fprintf(fd, "ln -s libcrypto.so.1.1 libcrypto.so\n");
      fprintf(fd, "cd %s/debian\n", distribution_path);
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libpng12.so.0 usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libvpx.so.3 usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libwebp.so.5 usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libwebpmux.so.1 usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libwebpdemux.so.1 usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libjasper.so.1 usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libevent-2.0.so.5 usr/local/webots/lib/webots\n");
#endif
      fprintf(fd, "mkdir DEBIAN\n");
      fprintf(fd, "echo \"Package: %s\" > DEBIAN/control\n", application_name_lowercase_and_dashes);
      fprintf(fd, "echo \"Version: %s\" >> DEBIAN/control\n", package_version + 1);  // remove initial R not supported
      fprintf(fd, "echo \"Section: science\" >> DEBIAN/control\n");
      fprintf(fd, "echo \"Priority: optional\" >> DEBIAN/control\n");
      fprintf(fd, "echo \"Architecture: %s\" >> DEBIAN/control\n", arch);
      fprintf(fd, "echo -n \"Installed-Size: \" >> DEBIAN/control\n");
      fprintf(fd, "du -sx %s/debian | awk '{print $1}' >> DEBIAN/control\n", distribution_path);
      fprintf(fd, "echo \"Depends: make, g++, libatk1.0-0 (>= 1.9.0), ffmpeg, libdbus-1-3, libfreeimage3 (>= 3.15.4-3), ");
      fprintf(fd, "libglib2.0-0 (>= 2.10.0), libglu1-mesa | libglu1, libgtk-3-0, ");
      fprintf(fd, "libnss3, libstdc++6 (>= 4.0.2-4), libxaw7, libxrandr2, libxrender1, ");
      fprintf(fd, "libzzip-0-13 (>= 0.13.62-2), libssh-dev, libzip-dev, xserver-xorg-core, libxslt1.1, ");
      fprintf(fd, "libgd3, libfreetype6, libxkbcommon-x11-0, libxcb-keysyms1, libxcb-image0, libxcb-icccm4, ");
      fprintf(fd, "libxcb-randr0, libxcb-render-util0, libxcb-xinerama0\" >> DEBIAN/control\n");

      if (!strcmp(application_name_lowercase_and_dashes, "webots")) {
        fprintf(fd, "echo \"Conflicts: webots-for-nao\" >> DEBIAN/control\n");
        fprintf(fd, "echo \"Maintainer: Olivier Michel <Olivier.Michel@cyberbotics.com>\" >> DEBIAN/control\n");
        fprintf(fd, "echo \"Description: Mobile robot simulation software\" >> DEBIAN/control\n");
        fprintf(fd, "echo \" Webots is a fast prototyping and simulation software\" >> DEBIAN/control\n");
        fprintf(fd, "echo \" which allows you to model, program and simulate any mobile\" >> DEBIAN/control\n");
        fprintf(fd, "echo \" robot, including wheeled, legged, swimming and flying robots.\" >> DEBIAN/control\n");
        fprintf(fd, "echo \" Transfer facilities allows you to transfer the robot\" >> DEBIAN/control\n");
        fprintf(fd, "echo \" controller from the simulation onto a real robot.\" >> DEBIAN/control\n");
        fprintf(fd, "cd ..\n");
      }

#ifdef WEBOTS_UBUNTU_18_04
      fprintf(fd, "fakeroot dpkg-deb -Zgzip --build debian %s\n", distribution_path);
#endif

      fprintf(fd, "echo creating the %s/%s-%s-%s.tar.bz2 tarball\n", distribution_path, application_name_lowercase_and_dashes,
              package_version, arch2);

      // copy include directories of libzip and libssh in tarball package
      fprintf(fd, "mkdir debian/usr/local/webots/include/libssh\n");
      fprintf(fd, "cp -a /usr/include/libssh debian/usr/local/webots/include/libssh/\n");
      fprintf(fd, "mkdir debian/usr/local/webots/include/libzip\n");
      fprintf(fd, "cp -a /usr/include/zip.h debian/usr/local/webots/include/libzip/\n");
#if defined(WEBOTS_UBUNTU_20_04)
      fprintf(fd, "cp /usr/include/zipconf.h debian/usr/local/webots/include/libzip/\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libzip.so.5 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libHalf.so.24 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libIex-2_3.so.24 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libIexMath-2_3.so.24 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libIlmThread-2_3.so.24 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libIlmImf-2_3.so.24 debian/usr/local/webots/lib/webots\n");
#else
      fprintf(fd, "cp /usr/include/x86_64-linux-gnu/zipconf.h debian/usr/local/webots/include/libzip/\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libzip.so.4 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libHalf.so.12 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libIex-2_2.so.12 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libIexMath-2_2.so.12 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libIlmThread-2_2.so.12 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libIlmImf-2_2.so.22 debian/usr/local/webots/lib/webots\n");
#endif

      // add the required libraries in order to avoid conflicts on other Linux distributions
      add_ros_dependencies("debian/usr/local/webots");
#ifdef WEBOTS_UBUNTU_16_04
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libraw.so.15 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libvpx.so.3 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libx264.so.148 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libavcodec-ffmpeg.so.56 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libpng12.so.0 debian/usr/local/webots/lib/webots\n");
#elif defined(WEBOTS_UBUNTU_18_04)
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libraw.so.16 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libvpx.so.5 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libx264.so.152 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libavcodec.so.57 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libwebp.so.6 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libwebpmux.so.3 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libpng16.so.16 debian/usr/local/webots/lib/webots\n");
#elif defined(WEBOTS_UBUNTU_20_04)
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libraw.so.19 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libvpx.so.6 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libx264.so.155 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libavcodec.so.58 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libwebp.so.6 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libwebpmux.so.3 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libpng16.so.16 debian/usr/local/webots/lib/webots\n");
#endif
      // libraries common to Ubuntu 16.04, 18.04 and 20.04
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libfreeimage.so.3 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libjpeg.so.8 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libjxrglue.so.0 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libopenjp2.so.7 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libjpegxr.so.0 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libzzip-0.so.13 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libjbig.so.0 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libgomp.so.1 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/liblcms2.so.2 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libXi.so.6 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libXrender.so.1 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libfontconfig.so.1 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libxslt.so.1 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libgd.so.3 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libssh.so.4 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libfreetype.so.6 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libxcb-keysyms.so.1 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libxcb-image.so.0 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libxcb-icccm.so.4 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libxcb-randr.so.0 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libxcb-render-util.so.0 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/libxcb-xinerama.so.0 debian/usr/local/webots/lib/webots\n");
      fprintf(fd, "cd debian/usr/local\n");
      fprintf(fd, "tar cf ../../../%s-%s-%s.tar.bz2 --use-compress-prog=pbzip2 %s\n", application_name_lowercase_and_dashes,
              package_version, arch2, application_name_lowercase_and_dashes);
      fprintf(fd, "rm -rf debian\n");
      break;
    case SNAP: {
      const char *usr_lib_x68_64_linux_gnu[] = {
        "libraw.so.16",        "libvpx.so.5",     "libx264.so.152",          "libavcodec.so.57",     "libwebp.so.6",
        "libwebpmux.so.3",     "libpng16.so.16",  "libfreeimage.so.3",       "libjxrglue.so.0",      "libopenjp2.so.7",
        "libjpegxr.so.0",      "libHalf.so.12",   "libIex-2_2.so.12",        "libIexMath-2_2.so.12", "libIlmThread-2_2.so.12",
        "libIlmImf-2_2.so.22", "libzip.so.4",     "libzzip-0.so.13",         "libjbig.so.0",         "libgomp.so.1",
        "liblcms2.so.2",       "libXi.so.6",      "libXrender.so.1",         "libfontconfig.so.1",   "libxslt.so.1",
        "libgd.so.3",          "libssh.so.4",     "libfreetype.so.6",        "libxcb-keysyms.so.1",  "libxcb-image.so.0",
        "libxcb-icccm.so.4",   "libxcb-randr.so", "libxcb-render-util.so.0", "libxcb-xinerama.so.0"};
      for (int i = 0; i < sizeof(usr_lib_x68_64_linux_gnu) / sizeof(char *); i++)
        fprintf(fd, "cp /usr/lib/x86_64-linux-gnu/%s $DESTDIR/usr/lib/x86_64-linux-gnu/\n", usr_lib_x68_64_linux_gnu[i]);
      fprintf(fd, "mkdir $DESTDIR/usr/share/webots/include/libssh\n");
      fprintf(fd, "cp -a /usr/include/libssh $DESTDIR/usr/share/webots/include/libssh/\n");
      fprintf(fd, "mkdir $DESTDIR/usr/share/webots/include/libzip\n");
      fprintf(fd, "cp -a /usr/include/zip.h $DESTDIR/usr/share/webots/include/libzip/\n");
      fprintf(fd, "cp /usr/include/x86_64-linux-gnu/zipconf.h $DESTDIR/usr/share/webots/include/libzip/\n");
      fprintf(fd, "cp $WEBOTS_HOME/scripts/packaging/webots_snap.desktop $DESTDIR/usr/share/webots/resources/webots.desktop\n");
      add_ros_dependencies("$DESTDIR/usr/share/webots");
      break;
    }
    default:
      break;
  }
  fclose(fd);
  if (mode == MAC) {
    snprintf(buffer, BUFFER_SIZE, "chmod a+x %s.mac", application_name_lowercase_and_dashes);
    system(buffer);
  } else if (mode == DEB) {
    snprintf(buffer, BUFFER_SIZE, "chmod a+x %s.deb", application_name_lowercase_and_dashes);
    system(buffer);
  } else if (mode == SNAP) {
    snprintf(buffer, BUFFER_SIZE, "chmod a+x %s.snap", application_name_lowercase_and_dashes);
    system(buffer);
  }
  printf(": done\n");
  fflush(stdout);
}

static void add_folder_recursively(FILE *fd, const char *folder, const char *options) {
  char path[1024];
  DIR *dd;
  struct dirent *d;
  struct stat s;

  fprintf(fd, "%s/", folder);
  if (options)
    fprintf(fd, " %s", options);
  fprintf(fd, "\n");
  sprintf(path, "%s/%s", webots_home, folder);
  dd = opendir(path);
  while ((d = readdir(dd))) {
    if (d->d_name[0] == '.' && d->d_name[1] == 0)
      continue;  // skip . directory
    if (d->d_name[0] == '.' && d->d_name[1] == '.' && d->d_name[2] == 0)
      continue;  // skip .. directory
    sprintf(path, "%s/%s/%s", webots_home, folder, d->d_name);
    bool success = stat(path, &s) == 0;
    if (!success) {
      fprintf(stderr, "Cannot stat \"%s\". This can be caused by a broken symbolic link.\n", path);
      exit(-1);
    }
    if (S_ISDIR(s.st_mode)) { /* directory */
      sprintf(path, "%s/%s", folder, d->d_name);
      add_folder_recursively(fd, path, options);
    } else if (S_ISREG(s.st_mode)) { /* regular file */
      char *protected_name =
#ifndef _WIN32
        str_replace(d->d_name, " ", "\\ ");
#else
        strdup(d->d_name);
#endif
      const int name_length = strlen(protected_name);
      if (strcmp(protected_name, ".gitignore") != 0 && strcmp(protected_name, ".DS_Store") != 0 &&
          strcmp(protected_name, ".DS_Store?") != 0 && strcmp(protected_name, ".Spotlight-V100") != 0 &&
          strcmp(protected_name, ".Trashes") != 0 && strcmp(protected_name, "ehthumbs.db") != 0 &&
          strcmp(protected_name, "Thumbs.db") != 0 && strncmp("._", protected_name, 2) != 0 &&
          (name_length < 3 || strcmp(protected_name + name_length - 4, ".swp") != 0) &&
          (name_length < 3 || strcmp(protected_name + name_length - 4, ".bak") != 0) &&
          strcmp(protected_name + name_length - 1, "~") != 0) {
        fprintf(fd, "%s/%s", folder, protected_name);
        if (options)
          fprintf(fd, " %s", options);
        fprintf(fd, "\n");
      }
      free(protected_name);
    }
  }
  closedir(dd);
}

#ifdef _WIN32
static void add_msys64_files() {
  FILE *fd = fopen("files.txt", "a");
  fprintf(fd, "\n# msys64 files (Windows only)\n\n");
  add_folder_recursively(fd, "msys64", "[windows]");
  fclose(fd);
}
#endif

static void add_files(const char *filename) {
  char line[1024];
  FILE *fd = fopen("files.txt", "a");
  FILE *fd2 = fopen(filename, "r");
  while (fgets(line, 1024, fd2)) {
    int l = strlen(line);
    if (l > 10 && strcmp(&line[l - 10], "[recurse]\n") == 0) {
      line[l - 11] = '\0';
      add_folder_recursively(fd, line, NULL);
    } else
      fputs(line, fd);
  }
  fclose(fd2);
  fclose(fd);
}

static void create_distributions(int m) {
  unlink("files.txt");
  add_files("files_core.txt");
#ifdef _WIN32
  add_msys64_files();
#endif
  add_files("files_projects.txt");
  add_files("textures_whitelist.txt");
  create_file("Webots", m);
}

int main(int argc, char *argv[]) {
  char month[16];
  int day;
  sscanf(__DATE__, "%15s %d %d", month, &day, &year);
  webots_home = getenv("WEBOTS_HOME");
  if (webots_home == NULL || webots_home[0] == '\0') {
    fprintf(stderr, "WEBOTS_HOME not defined\n");
    exit(-1);
  }

  const char *custom_distribution_path = getenv("WEBOTS_DISTRIBUTION_PATH");
  if (custom_distribution_path && !dir_exists(custom_distribution_path)) {
    fprintf(stderr, "WEBOTS_DISTRIBUTION_PATH is set to a directory (%s) who doesn't exists\n", custom_distribution_path);
    exit(-1);
  }
  if (custom_distribution_path)
    strcpy(distribution_path, custom_distribution_path);
  else
    snprintf(distribution_path, 256, "%s%cdistribution", webots_home, DIR_SEP);
  if (!dir_exists(distribution_path)) {
    fprintf(stderr, "distribution path (%s) doesn't exists\n", distribution_path);
    exit(-1);
  }

#ifdef _WIN32
  create_distributions(ISS);
#endif
#ifdef __APPLE__
  create_distributions(MAC);
#endif
#ifdef __linux__
  create_distributions(DEB);
  create_distributions(SNAP);
#endif
  return 0;
}
