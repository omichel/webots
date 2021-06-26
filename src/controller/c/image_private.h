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

#ifndef IMAGE_PRIVATE_H
#define IMAGE_PRIVATE_H

#ifdef _WIN32
#include <windows.h>
#endif
#include "request.h"

typedef struct {
  char *shm_key;
  int shmid;
  int shm_size;
  unsigned char *data;
  bool requested;
#ifdef _WIN32
  HANDLE shm_file;
#endif
} Image;

Image *image_new();
void image_cleanup_shm(Image *i);
void image_setup_shm(Image *i, WbRequest *r);
void image_get_shm(Image *i);

#endif  // IMAGE_PRIVATE_H
