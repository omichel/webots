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

#ifndef FIELD_HPP
#define FIELD_HPP

#define WB_USING_CPP_API
#include <string>
#include <webots/Node.hpp>
#include "../../c/webots/types.h"

#ifdef MF_STRING
#undef MF_STRING
#endif

namespace webots {
  class Node;
  class Field {
  public:
    typedef enum {
      NO_FIELD = 0x00,
      SF_BOOL = 0x01,
      SF_INT32,
      SF_FLOAT,
      SF_VEC2F,
      SF_VEC3F,
      SF_ROTATION,
      SF_COLOR,
      SF_STRING,
      SF_NODE,
      MF = 0x10,
      MF_BOOL,
      MF_INT32,
      MF_FLOAT,
      MF_VEC2F,
      MF_VEC3F,
      MF_ROTATION,
      MF_COLOR,
      MF_STRING,
      MF_NODE
    } Type;

    std::string getName() const;
    Type getType() const;
    std::string getTypeName() const;
    int getCount() const;

    void enableSFTracking(int samplingPeriod);
    void disableSFTracking();

    bool getSFBool() const;
    int getSFInt32() const;
    double getSFFloat() const;
    const double *getSFVec2f() const;
    const double *getSFVec3f() const;
    const double *getSFRotation() const;
    const double *getSFColor() const;
    std::string getSFString() const;
    Node *getSFNode() const;

    bool getMFBool(int index) const;
    int getMFInt32(int index) const;
    double getMFFloat(int index) const;
    const double *getMFVec2f(int index) const;
    const double *getMFVec3f(int index) const;
    const double *getMFRotation(int index) const;
    const double *getMFColor(int index) const;
    std::string getMFString(int index) const;
    Node *getMFNode(int index) const;

    void setSFBool(bool value);
    void setSFInt32(int value);
    void setSFFloat(double value);
    void setSFVec2f(const double values[2]);
    void setSFVec3f(const double values[3]);
    void setSFRotation(const double values[4]);
    void setSFColor(const double values[3]);
    void setSFString(const std::string &value);

    void setMFBool(int index, bool value);
    void setMFInt32(int index, int value);
    void setMFFloat(int index, double value);
    void setMFVec2f(int index, const double values[2]);
    void setMFVec3f(int index, const double values[3]);
    void setMFRotation(int index, const double values[4]);
    void setMFColor(int index, const double values[3]);
    void setMFString(int index, const std::string &value);

    void insertMFBool(int index, bool value);
    void insertMFInt32(int index, int value);
    void insertMFFloat(int index, double value);
    void insertMFVec2f(int index, const double values[2]);
    void insertMFVec3f(int index, const double values[3]);
    void insertMFRotation(int index, const double values[4]);
    void insertMFColor(int index, const double values[3]);
    void insertMFString(int index, const std::string &value);

    void removeMF(int index);
    void removeSF();

    void importMFNode(int position, const std::string &filename);
    void importMFNodeFromString(int position, const std::string &nodeString);
    void importSFNode(const std::string &filename);
    void importSFNodeFromString(const std::string &nodeString);

    // Deprecated functions

    // Deprecated since Webots 8.6.0, please use removeMF() instead
    void removeMFNode(int position);

    // DO NOT USE THESE FUNCTIONS: THEY ARE RESERVED FOR INTERNAL USE:
    static Field *findField(WbFieldRef ref);
    static void cleanup();

  private:
    Field(WbFieldRef ref);
    ~Field() {}

    WbFieldRef fieldRef;
  };
}  // namespace webots

#endif  // FIELD_HPP
