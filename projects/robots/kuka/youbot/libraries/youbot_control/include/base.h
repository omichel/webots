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

/*
 * Description:   Allows to handle the base
 */

#ifndef BASE_H
#define BASE_H

#include <webots/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void base_init();

void base_reset();
void base_forwards();
void base_backwards();
void base_turn_left();
void base_turn_right();
void base_strafe_left();
void base_strafe_right();

void base_move();
void base_forwards_increment();
void base_backwards_increment();
void base_turn_left_increment();
void base_turn_right_increment();
void base_strafe_left_increment();
void base_strafe_right_increment();

void base_goto_init(double time_step);
void base_goto_set_target(double x, double z, double a);
void base_goto_run();
bool base_goto_reached();

#ifdef __cplusplus
}
#endif

#endif
