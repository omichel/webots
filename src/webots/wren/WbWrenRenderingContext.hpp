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

#ifndef WB_WREN_RENDERING_CONTEXT_HPP
#define WB_WREN_RENDERING_CONTEXT_HPP

//
// Description: singleton class storing the context in which the
//              objects have to be created
//

#include <QtCore/QHash>
#include <QtCore/QObject>

class WbWrenRenderingContext : public QObject {
  Q_OBJECT

public:
  // projection modes
  enum { PM_PERSPECTIVE, PM_ORTHOGRAPHIC };
  // rendering modes
  enum { RM_PLAIN, RM_WIREFRAME };

  // visibility flags and masks
  enum {
    // rendering triggered by mouse selection
    VF_INVISIBLE_FROM_CAMERA = 0x00000002,  // flag for selected outlines and billboards

    // optional rendering from the menu (up to 30 flags)
    VF_ALL_BOUNDING_OBJECTS = 0x00000004,   // flag for the lines defining a bounding object
    VF_CAMERA_FRUSTUMS = 0x00000008,        // flag for camera frustum
    VF_CONNECTOR_AXES = 0x00000010,         // flag for connector axes
    VF_CONTACT_POINTS = 0x00000020,         // flag for contact points
    VF_COORDINATE_SYSTEM = 0x00000040,      // flag for the coordinate system of a viewpoint
    VF_DISTANCE_SENSORS_RAYS = 0x00000080,  // flag for distance sensors
    VF_LIGHT_SENSORS_RAYS = 0x00000100,     // flag for light sensors
    VF_LIGHTS_POSITIONS = 0x00000200,       // flag for a light s'flare
    VF_JOINT_AXES = 0x00000400,             // flag for joint axes
    VF_PEN_RAYS = 0x00000800,               // flag for pen
    VF_LIDAR_POINT_CLOUD = 0x00001000,      // flag for lidar point cloud
    VF_ODE_DEBUG_INFO = 0x00002000,         // flag for physics debug info
    VF_RANGE_FINDER_FRUSTUMS = 0x00004000,  // flag for range-finder frustum
    VF_LIDAR_RAYS_PATHS = 0x00008000,       // flag for lidar ray path
    VF_RADAR_FRUSTUMS = 0x00010000,         // flag for radar frustum
    VF_SKIN_SKELETON = 0x00020000,          // flag for skin skeleton
    VF_NORMALS = 0x00040000,                // Display mesh normals

    // distance sensors laser beam
    VF_LASER_BEAM = 0x00080000,

    // sugar for cameras definition
    VM_ALL = 0xFFFFFFFF,
    VM_NONE = 0x00000000,
    VM_REGULAR = 0xFFF00000,  // no special renderings, i.e. no outlines and no optional renderings from menu selection
    VM_MAIN = 0xFFFFFFFE,
    VM_WEBOTS_CAMERA = 0xFFFA0000,       // mask for WbCamera (all WREN MovableObjects)
    VM_WEBOTS_RANGE_CAMERA = 0xFFF80000  // mask for WbRangeFinder and WbLidar (all WREN MovableObjects except laser beam)
  };

  enum {
    // query masks
    QM_QUERIABLE = 0x00000001,
    QM_GEOMETRY = 0x00000002,
    QM_RESIZE_HANDLES = 0x00000004,
    QM_SCALE_HANDLES = 0x00000008,
    QM_TRANSLATE_HANDLES = 0x00000010,
    QM_ROTATE_HANDLES = 0x00000020,
    QM_NOT_QUERIABLE = 0x00000000,

    // query flags
    QF_GEOMETRY = QM_QUERIABLE | QM_GEOMETRY,
    QF_RESIZE_HANDLES = QM_QUERIABLE | QM_RESIZE_HANDLES,
    QF_SCALE_HANDLES = QM_QUERIABLE | QM_SCALE_HANDLES,
    QF_TRANSLATE_HANDLES = QM_QUERIABLE | QM_TRANSLATE_HANDLES,
    QF_ROTATE_HANDLES = QM_QUERIABLE | QM_ROTATE_HANDLES
  };

  // post-processing effects for cameras
  enum {
    PP_PASS_THROUGH,
    PP_GTAO,
    PP_SPHERICAL_CAMERA_MERGE,
    PP_LENS_DISTORTION,
    PP_DEPTH_OF_FIELD,
    PP_BLOOM,
    PP_LENS_FLARE,
    PP_MOTION_BLUR,
    PP_HDR,
    PP_COLOR_NOISE,
    PP_RANGE_NOISE,
    PP_DEPTH_RESOLUTION,
    PP_NOISE_MASK,
    PP_SMAA
  };

  static const double SOLID_LINE_SCALE_FACTOR;
  static const double RESIZE_HANDLE_LINE_SCALE_FACTOR;
  // Returns the current wren rendering context in use
  static WbWrenRenderingContext *instance() {
    if (!cRenderingContext)
      cRenderingContext = new WbWrenRenderingContext(0, 0);
    return cRenderingContext;
  }

  static void setWrenRenderingContext(int width, int height);
  static void cleanup();

  int width() const { return mWidth; }
  int height() const { return mHeight; }
  void setDimension(int width, int height);

  double lineScale() const { return mLineScale; }
  double solidLineScale() const { return mSolidLineScale; }
  void setLineScale(float lineScale);

  // retrieves the current selection of optional renderings through a 32-bits mask
  unsigned int optionalRenderingsMask() const { return mOptionalRenderingsMask; }
  bool isOptionalRenderingEnabled(int optionalRendering) const;
  void enableOptionalRendering(int optionalRendering, bool enable, bool userAction = true);

  int renderingMode() const { return mRenderingMode; }
  void setRenderingMode(int renderingMode, bool userAction = true);

  int projectionMode() const { return mProjectionMode; }
  void setProjectionMode(int projectionMode, bool userAction = true);

  unsigned int visibilityMask() const;

  // get renderer
  bool isIntelRenderer() const;
  bool isMesaRenderer() const;
  bool isMicrosoftRenderer() const;
  bool isAmdRenderer() const;
  bool isNvidiaRenderer() const;

  // add/remove handles render queue listener
  void enableDepthBufferReset(bool enabled);

  void requestView3dRefresh();

signals:
  void lineScaleChanged();
  void optionalRenderingChanged(int optionalRendering);
  void view3dRefreshRequired();
  void renderingModeChanged();
  void projectionModeChanged();
  void dimensionChanged();
  void numberOfOnLightsChanged();
  void shadowsStateChanged();
  void fogNodeAdded();
  void fogNodeDeleted();
  void backgroundColorChanged();

  void mainRenderingStarted(bool fromPhysics);
  void mainRenderingEnded(bool fromPhysics);

  void makeCurrentRequested();
  void doneCurrentRequested();

private:
  WbWrenRenderingContext(int width, int height);
  ~WbWrenRenderingContext();

  WbWrenRenderingContext(const WbWrenRenderingContext &);             // Prevent copy-construction
  WbWrenRenderingContext &operator=(const WbWrenRenderingContext &);  // Prevent assignment

  static WbWrenRenderingContext *cRenderingContext;

  int mWidth;
  int mHeight;
  double mLineScale;
  double mSolidLineScale;
  int mRenderingMode;
  int mProjectionMode;

  unsigned int mOptionalRenderingsMask;  // 32 bits integer mask encoding all the optional rendering selections;
  // enum
  enum { X, Y, Z };
};
#endif
