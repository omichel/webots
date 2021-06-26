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

#include "image_private.h"

#include <webots/robot.h>
#include "robot_private.h"

#ifdef _WIN32
#include <windows.h>
#else  // POSIX shared memory segments
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif
#include <stdlib.h>

Image *image_new() {
  Image *i = malloc(sizeof(Image));
  i->shm_key = NULL;
  i->shmid = -1;
  i->shm_size = 0;
  i->data = NULL;
  i->requested = false;
  return i;
}

void image_cleanup_shm(Image *i) {
  if (i->shm_size <= 0)
    return;

#ifdef _WIN32
  if (i->shm_file != NULL) {
    UnmapViewOfFile(i->data);
    CloseHandle(i->shm_file);
  }
#else  // POSIX shared memory
  if (i->shmid > 0) {
    munmap(i->data, i->shm_size);
    shm_unlink(i->shm_key);
  }
#endif
}

void image_setup_shm(Image *i, WbRequest *r) {
  i->shm_size = request_read_int32(r);
  if (i->shm_size > 0) {
    i->shm_key = request_read_string(r);
    image_get_shm(i);
  }
}

void image_get_shm(Image *i) {
#ifdef _WIN32
  i->shm_file = OpenFileMapping(FILE_MAP_WRITE, FALSE, i->shm_key);
  ROBOT_ASSERT(i->shm_file);
  i->data = MapViewOfFile(i->shm_file, FILE_MAP_WRITE, 0, 0, 0);
#else  // POSIX shared memory segments
  i->shmid = shm_open(i->shm_key, O_RDWR, 0400);
  i->data = (unsigned char *)mmap(0, i->shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, i->shmid, 0);
#endif

  ROBOT_ASSERT(i->data);
}
