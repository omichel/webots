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

#include "WbPointLight.hpp"

#include "WbField.hpp"
#include "WbFieldChecker.hpp"
#include "WbLightRepresentation.hpp"
#include "WbMFColor.hpp"
#include "WbNodeUtilities.hpp"
#include "WbSFBool.hpp"
#include "WbSFColor.hpp"
#include "WbSFDouble.hpp"
#include "WbSFVector3.hpp"
#include "WbTransform.hpp"
#include "WbWrenRenderingContext.hpp"

#include <wren/config.h>
#include <wren/node.h>
#include <wren/point_light.h>
#include <wren/scene.h>
#include <wren/transform.h>

void WbPointLight::init() {
  mLightRepresentation = NULL;
  mWrenLight = NULL;

  mAttenuation = findSFVector3("attenuation");
  mLocation = findSFVector3("location");
  mRadius = findSFDouble("radius");

  mSavedLocation[stateId()] = mLocation->value();
}

WbPointLight::WbPointLight(WbTokenizer *tokenizer) : WbLight("PointLight", tokenizer) {
  init();
  if (tokenizer == NULL) {
    mLocation->setYnoSignal(0.3);
    mAttenuation->setValueNoSignal(0.0, 0.0, 1.0);
  }
}

WbPointLight::WbPointLight(const WbPointLight &other) : WbLight(other) {
  init();
}

WbPointLight::WbPointLight(const WbNode &other) : WbLight(other) {
  init();
}

WbPointLight::~WbPointLight() {
  if (areWrenObjectsInitialized()) {
    detachFromUpperTransform();
    wr_node_delete(WR_NODE(mWrenLight));
    delete mLightRepresentation;
  }
}

void WbPointLight::reset(const QString &id) {
  WbLight::reset(id);
  mLocation->setValue(mSavedLocation[id]);
}

void WbPointLight::save(const QString &id) {
  WbLight::save(id);
  mSavedLocation[id] = mLocation->value();
}

void WbPointLight::preFinalize() {
  WbLight::preFinalize();

  updateAttenuation();
  updateLocation();
  updateRadius();
}

void WbPointLight::postFinalize() {
  WbLight::postFinalize();

  connect(mAttenuation, &WbSFVector3::changed, this, &WbPointLight::updateAttenuation);
  connect(mLocation, &WbSFVector3::changed, this, &WbPointLight::updateLocation);
  connect(mRadius, &WbSFDouble::changed, this, &WbPointLight::updateRadius);
}

WbVector3 WbPointLight::computeAbsoluteLocation() const {
  WbVector3 location = mLocation->value();
  WbTransform *ut = upperTransform();
  if (ut)
    location = ut->matrix() * location;
  return location;
}

void WbPointLight::createWrenObjects() {
  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::optionalRenderingChanged, this,
          &WbPointLight::updateOptionalRendering);

  mWrenLight = wr_point_light_new();
  attachToUpperTransform();
  WbLight::createWrenObjects();

  // Has to be done after WbLight::createWrenTransform (otherwise wrenNode() == NULL)
  mLightRepresentation = new WbLightRepresentation(wrenNode(), mLocation->value());

  applyLightAttenuationToWren();
  applyNodeLocationToWren();

  applyBillboardVisibilityToWren();
}

void WbPointLight::updateOptionalRendering(int option) {
  if (option == WbWrenRenderingContext::VF_LIGHTS_POSITIONS)
    mLightRepresentation->setVisible(WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(option));
}

void WbPointLight::updateAttenuation() {
  if (WbFieldChecker::resetVector3IfNegative(this, mAttenuation, WbVector3()))
    return;

  if (mAttenuation->value().x() > 0.0 || mAttenuation->value().y() > 0.0)
    parsingWarn(tr("A quadratic 'attenuation' should be preferred to have a realistic simulation of light. "
                   "Only the third component of the 'attenuation' field should be greater than 0."));

  checkAmbientAndAttenuationExclusivity();

  if (areWrenObjectsInitialized())
    applyLightAttenuationToWren();
}

void WbPointLight::updateLocation() {
  if (areWrenObjectsInitialized())
    applyNodeLocationToWren();
  emit locationChanged();
}

void WbPointLight::updateRadius() {
  if (WbFieldChecker::resetDoubleIfNegative(this, mRadius, 0.0))
    return;

  if (areWrenObjectsInitialized())
    applyLightAttenuationToWren();
}

void WbPointLight::updateAmbientIntensity() {
  checkAmbientAndAttenuationExclusivity();

  WbLight::updateAmbientIntensity();
}

void WbPointLight::updateIntensity() {
  WbLight::updateIntensity();
}

void WbPointLight::updateOn() {
  WbLight::updateOn();
  if (areWrenObjectsInitialized())
    applyBillboardVisibilityToWren();
}

void WbPointLight::updateColor() {
  WbLight::updateColor();
}

void WbPointLight::checkAmbientAndAttenuationExclusivity() {
  if (mAttenuation->value() != WbVector3(1.0, 0.0, 0.0) && ambientIntensity() != 0.0) {
    parsingWarn(
      tr("'ambientIntensity' and 'attenuation' cannot differ from their default values at the same time. 'ambientIntensity' "
         "was changed to 0."));
    setAmbientIntensity(0.0);
  }
}

void WbPointLight::attachToUpperTransform() {
  WbTransform *upperTransform = WbNodeUtilities::findUpperTransform(this);
  if (upperTransform)
    wr_transform_attach_child(upperTransform->wrenNode(), WR_NODE(mWrenLight));
}

void WbPointLight::detachFromUpperTransform() {
  WrNode *node = WR_NODE(mWrenLight);
  WrTransform *parent = wr_node_get_parent(node);
  if (parent)
    wr_transform_detach_child(parent, node);
}

void WbPointLight::applyLightIntensityToWren() {
  wr_point_light_set_intensity(mWrenLight, mIntensity->value());
}

void WbPointLight::applyLightColorToWren() {
  const float color[] = {static_cast<float>(mColor->red()), static_cast<float>(mColor->green()),
                         static_cast<float>(mColor->blue())};
  wr_point_light_set_color(mWrenLight, color);
}

void WbPointLight::applyLightVisibilityToWren() {
  wr_point_light_set_on(mWrenLight, mOn->value());

  const int maxCount = wr_config_get_max_active_point_light_count();
  const int activeCount = wr_scene_get_active_point_light_count(wr_scene_get_instance());
  if (activeCount == maxCount)
    parsingWarn(
      tr("Maximum number of active point lights (%1) has been reached, newly added lights won't be rendered.").arg(maxCount));
}

void WbPointLight::applyLightShadowsToWren() {
  wr_point_light_set_cast_shadows(mWrenLight, mCastShadows->value());
}

void WbPointLight::applyLightAttenuationToWren() {
  wr_point_light_set_radius(mWrenLight, mRadius->value());
  wr_point_light_set_attenuation(mWrenLight, mAttenuation->x(), mAttenuation->y(), mAttenuation->z());
}

void WbPointLight::applyNodeLocationToWren() {
  const float position[] = {static_cast<float>(mLocation->x()), static_cast<float>(mLocation->y()),
                            static_cast<float>(mLocation->z())};
  wr_point_light_set_position_relative(mWrenLight, position);
  mLightRepresentation->setPosition(mLocation->value());
}

void WbPointLight::applyBillboardVisibilityToWren() {
  const bool enabled =
    WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_LIGHTS_POSITIONS);
  mLightRepresentation->setVisible(isOn() && enabled);
}

double WbPointLight::computeAttenuation(double distance) const {
  return 1.0 / (mAttenuation->x() + mAttenuation->y() * distance + mAttenuation->z() * distance * distance);
}

void WbPointLight::exportNodeFields(WbVrmlWriter &writer) const {
  findField("attenuation", true)->write(writer);
  findField("location", true)->write(writer);
  findField("radius", true)->write(writer);
  WbLight::exportNodeFields(writer);
}
