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

#include "WbConcreteNodeFactory.hpp"

#include "WbAccelerometer.hpp"
#include "WbAppearance.hpp"
#include "WbBackground.hpp"
#include "WbBallJoint.hpp"
#include "WbBallJointParameters.hpp"
#include "WbBillboard.hpp"
#include "WbBox.hpp"
#include "WbBrake.hpp"
#include "WbCamera.hpp"
#include "WbCapsule.hpp"
#include "WbCharger.hpp"
#include "WbColor.hpp"
#include "WbCompass.hpp"
#include "WbCone.hpp"
#include "WbConnector.hpp"
#include "WbContactProperties.hpp"
#include "WbCoordinate.hpp"
#include "WbCylinder.hpp"
#include "WbDamping.hpp"
#include "WbDirectionalLight.hpp"
#include "WbDisplay.hpp"
#include "WbDistanceSensor.hpp"
#include "WbElevationGrid.hpp"
#include "WbEmitter.hpp"
#include "WbFluid.hpp"
#include "WbFocus.hpp"
#include "WbFog.hpp"
#include "WbGps.hpp"
#include "WbGroup.hpp"
#include "WbGyro.hpp"
#include "WbHinge2Joint.hpp"
#include "WbHingeJointParameters.hpp"
#include "WbImageTexture.hpp"
#include "WbImmersionProperties.hpp"
#include "WbIndexedFaceSet.hpp"
#include "WbIndexedLineSet.hpp"
#include "WbInertialUnit.hpp"
#include "WbLed.hpp"
#include "WbLens.hpp"
#include "WbLensFlare.hpp"
#include "WbLidar.hpp"
#include "WbLightSensor.hpp"
#include "WbLinearMotor.hpp"
#include "WbLog.hpp"
#include "WbMaterial.hpp"
#include "WbMesh.hpp"
#include "WbMicrophone.hpp"
#include "WbMuscle.hpp"
#include "WbNodeModel.hpp"
#include "WbNodeUtilities.hpp"
#include "WbNormal.hpp"
#include "WbPbrAppearance.hpp"
#include "WbPen.hpp"
#include "WbPhysics.hpp"
#include "WbPlane.hpp"
#include "WbPointLight.hpp"
#include "WbPointSet.hpp"
#include "WbPositionSensor.hpp"
#include "WbPropeller.hpp"
#include "WbProtoList.hpp"
#include "WbProtoModel.hpp"
#include "WbRadar.hpp"
#include "WbRadio.hpp"
#include "WbRangeFinder.hpp"
#include "WbReceiver.hpp"
#include "WbRecognition.hpp"
#include "WbRobot.hpp"
#include "WbRotationalMotor.hpp"
#include "WbShape.hpp"
#include "WbSkin.hpp"
#include "WbSliderJoint.hpp"
#include "WbSlot.hpp"
#include "WbSolid.hpp"
#include "WbSolidReference.hpp"
#include "WbSpeaker.hpp"
#include "WbSphere.hpp"
#include "WbSpotLight.hpp"
#include "WbTemplateManager.hpp"
#include "WbTextureCoordinate.hpp"
#include "WbTextureTransform.hpp"
#include "WbTouchSensor.hpp"
#include "WbTrack.hpp"
#include "WbTrackWheel.hpp"
#include "WbTransform.hpp"
#include "WbViewpoint.hpp"
#include "WbWorld.hpp"
#include "WbWorldInfo.hpp"
#include "WbZoom.hpp"

#include <QtCore/QStringList>

// this creates and destructs the global instance
WbConcreteNodeFactory WbConcreteNodeFactory::gFactory;

WbNode *WbConcreteNodeFactory::createNode(const QString &modelName, WbTokenizer *tokenizer, WbNode *parentNode,
                                          const QString *protoFilePath) {
  if (modelName == "Accelerometer")
    return new WbAccelerometer(tokenizer);
  if (modelName == "Appearance")
    return new WbAppearance(tokenizer);
  if (modelName == "Background")
    return new WbBackground(tokenizer);
  if (modelName == "BallJoint")
    return new WbBallJoint(tokenizer);
  if (modelName == "BallJointParameters")
    return new WbBallJointParameters(tokenizer);
  if (modelName == "Billboard")
    return new WbBillboard(tokenizer);
  if (modelName == "Box")
    return new WbBox(tokenizer);
  if (modelName == "Brake")
    return new WbBrake(tokenizer);
  if (modelName == "Camera")
    return new WbCamera(tokenizer);
  if (modelName == "Capsule")
    return new WbCapsule(tokenizer);
  if (modelName == "Charger")
    return new WbCharger(tokenizer);
  if (modelName == "Color")
    return new WbColor(tokenizer);
  if (modelName == "Compass")
    return new WbCompass(tokenizer);
  if (modelName == "Cone")
    return new WbCone(tokenizer);
  if (modelName == "Connector")
    return new WbConnector(tokenizer);
  if (modelName == "ContactProperties")
    return new WbContactProperties(tokenizer);
  if (modelName == "Coordinate")
    return new WbCoordinate(tokenizer);
  if (modelName == "Cylinder")
    return new WbCylinder(tokenizer);
  if (modelName == "Damping")
    return new WbDamping(tokenizer);
  if (modelName == "DirectionalLight")
    return new WbDirectionalLight(tokenizer);
  if (modelName == "Display")
    return new WbDisplay(tokenizer);
  if (modelName == "DistanceSensor")
    return new WbDistanceSensor(tokenizer);
  if (modelName == "ElevationGrid")
    return new WbElevationGrid(tokenizer);
  if (modelName == "Emitter")
    return new WbEmitter(tokenizer);
  if (modelName == "Fluid")
    return new WbFluid(tokenizer);
  if (modelName == "Focus")
    return new WbFocus(tokenizer);
  if (modelName == "Fog")
    return new WbFog(tokenizer);
  if (modelName == "GPS")
    return new WbGps(tokenizer);
  if (modelName == "Group")
    return new WbGroup(tokenizer);
  if (modelName == "Gyro")
    return new WbGyro(tokenizer);
  if (modelName == "Hinge2Joint")
    return new WbHinge2Joint(tokenizer);
  if (modelName == "Hinge2JointParameters")
    return new WbHingeJointParameters(tokenizer, true);  // DEPRECATED, only for backward compatibility
  if (modelName == "HingeJoint")
    return new WbHingeJoint(tokenizer);
  if (modelName == "HingeJointParameters")
    return new WbHingeJointParameters(tokenizer);
  if (modelName == "ImageTexture")
    return new WbImageTexture(tokenizer);
  if (modelName == "ImmersionProperties")
    return new WbImmersionProperties(tokenizer);
  if (modelName == "IndexedFaceSet")
    return new WbIndexedFaceSet(tokenizer);
  if (modelName == "IndexedLineSet")
    return new WbIndexedLineSet(tokenizer);
  if (modelName == "InertialUnit")
    return new WbInertialUnit(tokenizer);
  if (modelName == "JointParameters")
    return new WbJointParameters(tokenizer);
  if (modelName == "LED")
    return new WbLed(tokenizer);
  if (modelName == "Lens")
    return new WbLens(tokenizer);
  if (modelName == "LensFlare")
    return new WbLensFlare(tokenizer);
  if (modelName == "Lidar")
    return new WbLidar(tokenizer);
  if (modelName == "LightSensor")
    return new WbLightSensor(tokenizer);
  if (modelName == "LinearMotor")
    return new WbLinearMotor(tokenizer);
  if (modelName == "Mesh")
    return new WbMesh(tokenizer);
  if (modelName == "Material")
    return new WbMaterial(tokenizer);
  if (modelName == "Microphone")
    return new WbMicrophone(tokenizer);
  if (modelName == "Muscle")
    return new WbMuscle(tokenizer);
  if (modelName == "Normal")
    return new WbNormal(tokenizer);
  if (modelName == "PBRAppearance")
    return new WbPbrAppearance(tokenizer);
  if (modelName == "Pen")
    return new WbPen(tokenizer);
  if (modelName == "Physics")
    return new WbPhysics(tokenizer);
  if (modelName == "Plane")
    return new WbPlane(tokenizer);
  if (modelName == "PointLight")
    return new WbPointLight(tokenizer);
  if (modelName == "PointSet")
    return new WbPointSet(tokenizer);
  if (modelName == "PositionSensor")
    return new WbPositionSensor(tokenizer);
  if (modelName == "Propeller")
    return new WbPropeller(tokenizer);
  if (modelName == "Radar")
    return new WbRadar(tokenizer);
  if (modelName == "Radio")
    return new WbRadio(tokenizer);
  if (modelName == "RangeFinder")
    return new WbRangeFinder(tokenizer);
  if (modelName == "Receiver")
    return new WbReceiver(tokenizer);
  if (modelName == "Recognition")
    return new WbRecognition(tokenizer);
  if (modelName == "Robot")
    return new WbRobot(tokenizer);
  if (modelName == "RotationalMotor")
    return new WbRotationalMotor(tokenizer);
  if (modelName == "Shape")
    return new WbShape(tokenizer);
  if (modelName == "Skin")
    return new WbSkin(tokenizer);
  if (modelName == "SliderJoint")
    return new WbSliderJoint(tokenizer);
  if (modelName == "Slot")
    return new WbSlot(tokenizer);
  if (modelName == "Solid")
    return new WbSolid(tokenizer);
  if (modelName == "SolidReference")
    return new WbSolidReference(tokenizer);
  if (modelName == "Speaker")
    return new WbSpeaker(tokenizer);
  if (modelName == "Sphere")
    return new WbSphere(tokenizer);
  if (modelName == "SpotLight")
    return new WbSpotLight(tokenizer);
  if (modelName == "TextureCoordinate")
    return new WbTextureCoordinate(tokenizer);
  if (modelName == "TextureTransform")
    return new WbTextureTransform(tokenizer);
  if (modelName == "TouchSensor")
    return new WbTouchSensor(tokenizer);
  if (modelName == "Track")
    return new WbTrack(tokenizer);
  if (modelName == "TrackWheel")
    return new WbTrackWheel(tokenizer);
  if (modelName == "Transform")
    return new WbTransform(tokenizer);
  if (modelName == "Viewpoint")
    return new WbViewpoint(tokenizer);
  if (modelName == "WorldInfo")
    return new WbWorldInfo(tokenizer);
  if (modelName == "Zoom")
    return new WbZoom(tokenizer);

  // look for PROTOs
  WbProtoModel *const model =
    protoFilePath ?
      WbProtoList::current()->readModel(*protoFilePath, WbWorld::instance() ? WbWorld::instance()->fileName() : "") :
      WbProtoList::current()->findModel(modelName, WbWorld::instance() ? WbWorld::instance()->fileName() : "");
  if (!model)
    return NULL;

  // reset global parent that could be changed while parsing the PROTO model
  if (parentNode)
    WbNode::setGlobalParentNode(parentNode);
  WbNode *protoInstance =
    WbNode::createProtoInstance(model, tokenizer, WbWorld::instance() ? WbWorld::instance()->fileName() : "");
  if (protoInstance)
    WbTemplateManager::instance()->subscribe(protoInstance);
  return protoInstance;
}

WbNode *WbConcreteNodeFactory::createCopy(const WbNode &original) {
  const QString &modelName = original.nodeModelName();

  if (modelName == "Accelerometer")
    return new WbAccelerometer(original);
  if (modelName == "Appearance")
    return new WbAppearance(original);
  if (modelName == "Background")
    return new WbBackground(original);
  if (modelName == "BallJoint")
    return new WbBallJoint(original);
  if (modelName == "BallJointParameters")
    return new WbBallJointParameters(original);
  if (modelName == "Billboard")
    return new WbBillboard(original);
  if (modelName == "Box")
    return new WbBox(original);
  if (modelName == "Brake")
    return new WbBrake(original);
  if (modelName == "Camera")
    return new WbCamera(original);
  if (modelName == "Capsule")
    return new WbCapsule(original);
  if (modelName == "Charger")
    return new WbCharger(original);
  if (modelName == "Color")
    return new WbColor(original);
  if (modelName == "Compass")
    return new WbCompass(original);
  if (modelName == "Cone")
    return new WbCone(original);
  if (modelName == "Connector")
    return new WbConnector(original);
  if (modelName == "ContactProperties")
    return new WbContactProperties(original);
  if (modelName == "Coordinate")
    return new WbCoordinate(original);
  if (modelName == "Cylinder")
    return new WbCylinder(original);
  if (modelName == "Damping")
    return new WbDamping(original);
  if (modelName == "DirectionalLight")
    return new WbDirectionalLight(original);
  if (modelName == "Display")
    return new WbDisplay(original);
  if (modelName == "DistanceSensor")
    return new WbDistanceSensor(original);
  if (modelName == "ElevationGrid")
    return new WbElevationGrid(original);
  if (modelName == "Emitter")
    return new WbEmitter(original);
  if (modelName == "Fluid")
    return new WbFluid(original);
  if (modelName == "Focus")
    return new WbFocus(original);
  if (modelName == "Fog")
    return new WbFog(original);
  if (modelName == "GPS")
    return new WbGps(original);
  if (modelName == "Group")
    return new WbGroup(original);
  if (modelName == "Gyro")
    return new WbGyro(original);
  if (modelName == "Hinge2Joint")
    return new WbHinge2Joint(original);
  if (modelName == "Hinge2JointParameters")
    return new WbHingeJointParameters(original, true);  // DEPRECATED, only for backward compatibility
  if (modelName == "HingeJoint")
    return new WbHingeJoint(original);
  if (modelName == "HingeJointParameters")
    return new WbHingeJointParameters(original);
  if (modelName == "ImageTexture")
    return new WbImageTexture(original);
  if (modelName == "ImmersionProperties")
    return new WbImmersionProperties(original);
  if (modelName == "IndexedFaceSet")
    return new WbIndexedFaceSet(original);
  if (modelName == "IndexedLineSet")
    return new WbIndexedLineSet(original);
  if (modelName == "InertialUnit")
    return new WbInertialUnit(original);
  if (modelName == "JointParameters")
    return new WbJointParameters(original);
  if (modelName == "LED")
    return new WbLed(original);
  if (modelName == "Lens")
    return new WbLens(original);
  if (modelName == "LensFlare")
    return new WbLensFlare(original);
  if (modelName == "Lidar")
    return new WbLidar(original);
  if (modelName == "LightSensor")
    return new WbLightSensor(original);
  if (modelName == "LinearMotor")
    return new WbLinearMotor(original);
  if (modelName == "Material")
    return new WbMaterial(original);
  if (modelName == "Mesh")
    return new WbMesh(original);
  if (modelName == "Microphone")
    return new WbMicrophone(original);
  if (modelName == "Muscle")
    return new WbMuscle(original);
  if (modelName == "Normal")
    return new WbNormal(original);
  if (modelName == "PBRAppearance")
    return new WbPbrAppearance(original);
  if (modelName == "Pen")
    return new WbPen(original);
  if (modelName == "Physics")
    return new WbPhysics(original);
  if (modelName == "Plane")
    return new WbPlane(original);
  if (modelName == "PointLight")
    return new WbPointLight(original);
  if (modelName == "PointSet")
    return new WbPointSet(original);
  if (modelName == "PositionSensor")
    return new WbPositionSensor(original);
  if (modelName == "Propeller")
    return new WbPropeller(original);
  if (modelName == "Radar")
    return new WbRadar(original);
  if (modelName == "Radio")
    return new WbRadio(original);
  if (modelName == "RangeFinder")
    return new WbRangeFinder(original);
  if (modelName == "Receiver")
    return new WbReceiver(original);
  if (modelName == "Recognition")
    return new WbRecognition(original);
  if (modelName == "Robot")
    return new WbRobot(original);
  if (modelName == "RotationalMotor")
    return new WbRotationalMotor(original);
  if (modelName == "Shape")
    return new WbShape(original);
  if (modelName == "Skin")
    return new WbSkin(original);
  if (modelName == "SliderJoint")
    return new WbSliderJoint(original);
  if (modelName == "Slot")
    return new WbSlot(original);
  if (modelName == "Solid")
    return new WbSolid(original);
  if (modelName == "SolidReference")
    return new WbSolidReference(original);
  if (modelName == "Speaker")
    return new WbSpeaker(original);
  if (modelName == "Sphere")
    return new WbSphere(original);
  if (modelName == "SpotLight")
    return new WbSpotLight(original);
  if (modelName == "TextureCoordinate")
    return new WbTextureCoordinate(original);
  if (modelName == "TextureTransform")
    return new WbTextureTransform(original);
  if (modelName == "TouchSensor")
    return new WbTouchSensor(original);
  if (modelName == "Track")
    return new WbTrack(original);
  if (modelName == "TrackWheel")
    return new WbTrackWheel(original);
  if (modelName == "Transform")
    return new WbTransform(original);
  if (modelName == "Viewpoint")
    return new WbViewpoint(original);
  if (modelName == "WorldInfo")
    return new WbWorldInfo(original);
  if (modelName == "Zoom")
    return new WbZoom(original);

  return NULL;
}

const QString WbConcreteNodeFactory::slotType(WbNode *node) {
  return WbNodeUtilities::slotType(node);
}

bool WbConcreteNodeFactory::validateExistingChildNode(const WbField *field, const WbNode *childNode, const WbNode *node,
                                                      bool isInBoundingObject, QString &errorMessage) const {
  return WbNodeUtilities::validateExistingChildNode(field, childNode, node, isInBoundingObject, errorMessage);
}

void WbConcreteNodeFactory::exportAsVrml(const WbNode *node, WbVrmlWriter &writer) {
  if (node->nodeModelName() == "Plane") {
    WbPlane plane(*node);
    plane.write(writer);
    return;
  }
  if (node->nodeModelName() == "Capsule") {
    WbCapsule capsule(*node);
    capsule.write(writer);
    return;
  }
  if (node->nodeModelName() == "Mesh") {
    WbMesh mesh(*node);
    mesh.write(writer);
    return;
  }

  assert(0);  // we should not reach this line
}
