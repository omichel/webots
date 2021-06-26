// Copyright 1996-2021 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef WR_JS_HELPER_H
#define WR_JS_HELPER_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

float *wrjs_array3(float element0, float element1, float element2);
float *wrjs_array4(float element0, float element1, float element2, float element3);
int *wrjs_pointerOnInt(int number);
int *wrjs_pointerOnIntBis(int number);
char *wrjs_pointerOnFloat(float number);

void wrjs_init_context(int width, int height);
void wrjs_exit();
#ifdef __cplusplus
}
#endif

#endif  // WR_JS_HELPER_H
