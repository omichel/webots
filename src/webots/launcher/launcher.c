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

// Description:  This is the source code of webots.exe and webotsw.exe, the binary launchers of webots-bin.exe (Windows only).
//               It adds paths to the PATH environment variable to ensure that the correct libraries will be found and starts
//               webots-bin.exe, passing all the command line arguments, wait until the completion of webots-bin.exe and
//               returns the exit status code of webots-bin.exe.
//               The main advantage of a binary launcher over a batch launcher (webots.bat) is that the binary launcher
//               doesn't open a DOS cmd.exe console in the background, whereas this is unavoidable when running a batch file.
//               webotsw.exe is a windows application (to be started from the icon or menu).
//               webots.exe is DOS application (to be started from a DOS command prompt or a script).
//               Starting webots.exe from the icon is fine, but will open a DOS command prompt in the background.
//               (a similar naming convention is used for python.exe / pythonw.exe, java.exe / javaw.exe, etc.)

#include <stdio.h>
#include <windows.h>

static int fail(const char *function, const char *info) {
  DWORD e = GetLastError();
  if (e) {
    LPSTR m = NULL;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e,
                  MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPSTR)&m, 0, NULL);
    char message[1024];
    const char *lf = info ? "\n" : "";
    const char *i = info ? info : "";
    // cppcheck-suppress nullPointer
    snprintf(message, sizeof(message), "%s failed with error %lu.\n%s%s%s", function, e, m, i, lf);
    LocalFree(m);
#ifdef WEBOTSW
    MessageBox(NULL, message, "Webots launcher error", MB_ICONERROR | MB_OK);
#else
    fprintf(stderr, message);
#endif
  } else
    fprintf(stderr, "%s failed with no error.\n", function);
  exit(e);
}

int main(int argc, char *argv[]) {
  // We retrieve the command line in wchar_t from the Windows system.
  const wchar_t *original_command_line = GetCommandLineW();
  // It should look like:
  // '"C:\Program Files\Webots\msys64\mingw64\bin\webotsw.exe" "C:\Users\Paul\Documents\my_project\worlds\my_project.wbt"' or
  // '"C:\Program Files\Webots\msys64\mingw64\bin\webots.exe" "C:\Users\Paul\Documents\my_project\worlds\my_project.wbt"'
  // (notice: webots.exe instead of webotsw.exe)
  wchar_t command_line[wcslen(original_command_line)];
  // In order to launch Webots, we simply need to replace 'webotsw.exe'/'webots.exe' with 'webots-bin.exe'
  const wchar_t *find = wcsstr(original_command_line, L"\\msys64\\mingw64\\bin\\webots");
  int index = find - original_command_line;
  index += 26;
  wcsncpy(command_line, original_command_line, index);
  command_line[index++] = '-';
  command_line[index++] = 'b';
  command_line[index++] = 'i';
  command_line[index++] = 'n';
#ifdef WEBOTSW
  const int offset = 3;  // strlen("webots-bin") - strlen("webotsw")
#else
  const int offset = 4;  // strlen("webots-bin") - strlen("webots")
#endif
  for (; original_command_line[index - offset - 1]; index++)  // we use (index - offset - 1) to include the final '\0'
    command_line[index] = original_command_line[index - offset];

  // compute the full command line with absolute path for webots-bin.exe, options and arguments
  const int LENGTH = 4096;
  wchar_t *module_path = malloc(LENGTH * sizeof(wchar_t));
  if (!GetModuleFileNameW(NULL, module_path, LENGTH))
    fail("GetModuleFileNameW", 0);
  const int l = wcslen(module_path)
#ifdef WEBOTSW
                - 1  // webotsw.exe (we need to remove the final 'w')
#endif
    ;
  // add "WEBOTS_HOME/msys64/mingw64/bin", "WEBOTS_HOME/msys64/mingw64/bin/cpp" and "WEBOTS_HOME/msys64/usr/bin" to the PATH
  // environment variable
  wchar_t *old_path = malloc(LENGTH * sizeof(wchar_t));
  wchar_t *new_path = malloc(LENGTH * sizeof(wchar_t));

  wcscpy(new_path, module_path);
  new_path[l - 11] = ';';  // removes "\webots.exe" or "\webotsw.exe"
  wcscpy(&new_path[l - 10], module_path);
  new_path[2 * l - 21] = '\0';
  wcscat(new_path, L"\\cpp;");
  wcscat(new_path, module_path);
  free(module_path);
  new_path[3 * l - 38] = '\0';
  wcscat(new_path, L"usr\\bin;");
  if (!GetEnvironmentVariableW(L"PATH", old_path, LENGTH))
    fail("GetEnvironmentVariableW", "PATH");
  wcscat(new_path, old_path);
  free(old_path);
  if (!SetEnvironmentVariableW(L"PATH", new_path))
    fail("SetEnvironmentVariableW", "PATH");
  free(new_path);
  if (!SetEnvironmentVariableW(L"QT_ENABLE_HIGHDPI_SCALING", L"1"))
    fail("SetEnvironmentVariableW", "QT_ENABLE_HIGHDPI_SCALING=1");

  // start the webots-bin.exe process, wait for completion and return exit code
  STARTUPINFOW info = {sizeof(info)};
  PROCESS_INFORMATION process_info;

  while (1) {
    if (!CreateProcessW(NULL, command_line, NULL, NULL, TRUE, 0, NULL, NULL, &info, &process_info))
      fail("CreateProcess", "Cannot launch Webots binary");

    // webots-bin.exe should be killed whenever its parent (webots.exe or webotsw.exe) terminates.
    HANDLE job = CreateJobObject(NULL, NULL);
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = {0};
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)))
      fail("SetInformationJobObject", 0);
    if (!AssignProcessToJobObject(job, process_info.hProcess))
      fail("AssignProcessToJobObject", 0);

    // wait for webots-bin.exe to terminate
    WaitForSingleObject(process_info.hProcess, INFINITE);  // return zero in case of success
    DWORD exit_code;
    if (!GetExitCodeProcess(process_info.hProcess, &exit_code))
      fail("GetExitCodeProcess", 0);
    if (!CloseHandle(process_info.hProcess))
      fail("CloseHandle", 0);
    if (!CloseHandle(process_info.hThread))
      fail("CloseHandle", 0);
    if (exit_code != 3030)  // special return code to restart Webots, see WbGuiApplication.cpp
      return exit_code;
  }
  return 0;
}
