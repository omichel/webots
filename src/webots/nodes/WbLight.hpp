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

#ifndef WB_LIGHT_HPP
#define WB_LIGHT_HPP

//
// Description: abstract base class for all lights
// Inherited by:
//   WbDirectionalLight, WbPointLight, WbSpotLight
//

#include "WbBaseNode.hpp"
#include "WbRgb.hpp"

#include <QtCore/QList>

class WbLight : public WbBaseNode {
  Q_OBJECT

public:
  // destructor
  virtual ~WbLight();

  // reimplemented public functions
  void createWrenObjects() override;
  void preFinalize() override;
  void postFinalize() override;

  // field accessors
  bool isOn() const;
  bool castShadows() const;
  bool castLensFlares() const;
  double intensity() const;
  double ambientIntensity() const;
  const WbRgb &color() const;

  const WbRgb &initialColor() const { return mInitialColor; }
  void setColor(const WbRgb &color);

  void toggleOn(bool on);

  // specific functions
  static bool doesAtLeastOneLightCastsShadows() { return numberOfLightsCastingShadows() > 0; }
  static const QList<const WbLight *> &lights() { return cLights; }
  static int numberOfOnLights();

protected:
  // all constructors are reserved for derived classes only
  WbLight(const WbLight &other);
  WbLight(const WbNode &other);
  WbLight(const QString &modelName, WbTokenizer *tokenizer);

  void setAmbientIntensity(double value);

  void exportNodeFields(WbVrmlWriter &writer) const override;

  // user accessible fields
  WbSFColor *mColor;
  WbSFDouble *mIntensity;
  WbSFBool *mOn;
  WbSFDouble *mAmbientIntensity;
  WbSFBool *mCastShadows;
  WbSFBool *mCastLensFlares;

protected slots:
  virtual void updateAmbientIntensity();
  virtual void updateIntensity();
  virtual void updateOn();
  virtual void updateColor();

private:
  static QList<const WbLight *> cLights;
  static int numberOfLightsCastingShadows();

  // other variables
  WbRgb mInitialColor;

  WbLight &operator=(const WbLight &);  // non copyable
  // only derived classes can be cloned
  WbNode *clone() const override = 0;

  void init();
  virtual void applyLightIntensityToWren() = 0;
  virtual void applyLightColorToWren() = 0;
  virtual void applyLightVisibilityToWren() = 0;
  virtual void applyLightShadowsToWren() = 0;

  static void applySceneAmbientColorToWren();
  static void computeAmbientLight();

private slots:
  void updateCastShadows();

signals:
  void castLensFlaresChanged();
  void isOnChanged();
  void colorChanged();
  void intensityChanged();
  void locationChanged();
  void directionChanged();
};

#endif
