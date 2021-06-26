/*
 * Description:  Test Supervisor API
 *               This file contains some tests of the methods relative to fields,
 *               i.e. functions starting with wb_supervisor_field_* for:
 *                  SF_FLOAT
 *                  MF_COLOR
 */

#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[0]);

  WbNodeRef root, texture, material, background, proto, shape, plane, box, internal_physics, mfTest, self, internal_appearance;
  WbFieldRef field, internal_field, internal_sf_node_field, internal_transparency_field, supervisor_field;
  const char *charArray;
  const double *doubleArray;
  double doubleVec3[3] = {0.0, 0.0, 0.0};
  int i;
  double d;

  root = wb_supervisor_node_get_root();
  ts_assert_pointer_not_null(root, "Root node is not found");

  background = wb_supervisor_node_get_from_def("BACKGROUND");
  ts_assert_pointer_not_null(background, "Node \"BACKGROUND\" is not found");

  proto = wb_supervisor_node_get_from_def("E_PUCK");
  ts_assert_pointer_not_null(proto, "Proto node \"E_PUCK\" is not found");

  box = wb_supervisor_node_get_from_def("BOX");
  ts_assert_pointer_not_null(box, "Node \"BOX\" is not found");

  texture = wb_supervisor_node_get_from_def("BOX_IMAGE_TEXTURE");
  ts_assert_pointer_not_null(texture, "Node \"BOX_IMAGE_TEXTURE\" is not found");

  material = wb_supervisor_node_get_from_def("BOX_MATERIAL");
  ts_assert_pointer_not_null(texture, "Node \"BOX_MATERIAL\" is not found");

  shape = wb_supervisor_node_get_from_def("BOX_SHAPE");
  ts_assert_pointer_not_null(shape, "Node \"BOX_SHAPE\" is not found");

  plane = wb_supervisor_node_get_from_def("PLANE");
  ts_assert_pointer_not_null(plane, "Node \"PLANE\" is not found");

  mfTest = wb_supervisor_node_get_from_def("MF_FIELDS");
  ts_assert_pointer_not_null(mfTest, "Node \"MF_FIELDS\" is not found");

  // Check field setters and getters

  field = wb_supervisor_node_get_field(proto, "camera_fieldOfView");
  i = wb_supervisor_field_get_type(field);
  ts_assert_boolean_equal(i == WB_SF_FLOAT, "Proto \"camera_fieldOfView\" field should have type %d not %d", WB_SF_FLOAT, i);

  charArray = wb_supervisor_field_get_type_name(field);
  ts_assert_string_equal(charArray, "SFFloat",
                         "Proto \"camera_fieldOfView\" field should have type name \"SFFloat\" not \"%s\"", charArray);

  d = wb_supervisor_field_get_sf_float(field);
  ts_assert_double_equal(d, 0.84, "Proto \"camera_fieldOfView\" SFFloat field should have value 0.84 not %f", d);

  // set and get new SFFloat value
  wb_supervisor_field_set_sf_float(field, 0.73);
  d = wb_supervisor_field_get_sf_float(field);
  ts_assert_double_equal(d, 0.73, "Proto \"camera_fieldOfView\" SFFloat field should have value 0.73 not %f", d);

  // invalid to retrive a non-PROTO field or a PROTO parameter
  internal_field = wb_supervisor_node_get_proto_field(box, "scale");
  ts_assert_pointer_null(internal_field, "wb_supervisor_node_get_proto_field should only work for PROTO nodes.");
  internal_field = wb_supervisor_node_get_proto_field(mfTest, "mfBool");
  ts_assert_pointer_null(internal_field, "wb_supervisor_node_get_proto_field should only work for internal PROTO fields.");

  // internal SFNode
  internal_sf_node_field = wb_supervisor_node_get_proto_field(proto, "physics");
  ts_assert_pointer_not_null(internal_sf_node_field, "wb_supervisor_node_get_proto_field should return an internal field.");
  internal_physics = wb_supervisor_field_get_sf_node(internal_sf_node_field);
  ts_assert_pointer_not_null(internal_physics, "wb_supervisor_field_get_sf_node should return an internal node.");
  internal_field = wb_supervisor_node_get_field(internal_physics, "density");
  ts_assert_pointer_not_null(internal_field, "wb_supervisor_node_get_proto_field should return an internal field.");
  d = wb_supervisor_field_get_sf_float(internal_field);
  ts_assert_double_equal(d, -1.0, "Returned value should be -1.0 and not %f", d);

  // internal SFFloat value
  internal_field = wb_supervisor_node_get_field(proto, "cpuConsumption");
  ts_assert_pointer_null(internal_field, "wb_supervisor_node_get_field should not return internal fields.");
  internal_field = wb_supervisor_node_get_proto_field(proto, "cpuConsumption");
  ts_assert_pointer_not_null(internal_field, "wb_supervisor_node_get_proto_field should return an internal field.");
  d = wb_supervisor_field_get_sf_float(internal_field);
  ts_assert_double_equal(d, 1.11, "Returned value should be 1.11 and not %f", d);
  wb_supervisor_field_set_sf_float(internal_field, 1.5);

  // internal EPUCK_TRANSPARENT_APPEARANCE SFFloat value
  internal_appearance = wb_supervisor_node_get_from_proto_def(proto, "EPUCK_TRANSPARENT_APPEARANCE");
  ts_assert_pointer_not_null(
    internal_appearance, "wb_supervisor_node_get_from_proto_def should return the internal EPUCK_TRANSPARENT_APPEARANCE node.");
  internal_transparency_field = wb_supervisor_node_get_field(internal_appearance, "transparency");
  ts_assert_pointer_not_null(
    internal_transparency_field,
    "wb_supervisor_node_get_field should return the field of the internal EPUCK_TRANSPARENT_APPEARANCE node.");
  wb_supervisor_field_set_sf_float(internal_transparency_field, 0.0);
  d = wb_supervisor_field_get_sf_float(internal_transparency_field);
  ts_assert_double_equal(d, 0.4, "Returned value for invalid set of internal node should be 0.4 and not %f", d);

  // get SFFloat on a different field type
  field = wb_supervisor_node_get_field(proto, "camera_height");
  i = wb_supervisor_field_get_type(field);
  ts_assert_boolean_equal(i == WB_SF_INT32, "Proto \"camera_height\" field should have type %d not %d", WB_SF_INT32, i);
  d = wb_supervisor_field_get_sf_float(field);
  ts_assert_double_equal(d, 0.0, "Returned value for non SFFloat field should be 0 not %f", d);

  // set SFFloat on a different field type
  wb_supervisor_field_set_sf_float(field, 0.2);  // should raise a warning in the console and not be executed
  i = wb_supervisor_field_get_sf_int32(field);
  ts_assert_int_equal(i, 39, "Returned value for SFInt32 field should be 39 not %d",
                      i);  // original value should not be changed

  // MFColor
  field = wb_supervisor_node_get_field(background, "skyColor");
  i = wb_supervisor_field_get_type(field);
  ts_assert_boolean_equal(i == WB_MF_COLOR, "Background \"skyColor\" field should have type %d not %d", WB_MF_COLOR, i);
  charArray = wb_supervisor_field_get_type_name(field);
  ts_assert_string_equal(charArray, "MFColor", "Background \"skyColor\" field should have type name \"MFColor\" not \"%s\"",
                         charArray);

  // count items
  i = wb_supervisor_field_get_count(field);
  ts_assert_int_equal(i, 3, "Background \"skyColor\" mf_color field should have 3 items not %f", i);

  // get item
  doubleArray = wb_supervisor_field_get_mf_color(field, 0);
  ts_assert_vec3_equal(doubleArray[0], doubleArray[1], doubleArray[2], 0.4, 0.7, 1,
                       "Item 0 of \"skyColor\" field of Background node should be [0.4, 0.7, 1] not [%f, %f, %f]",
                       doubleArray[0], doubleArray[1], doubleArray[2]);
  doubleArray = wb_supervisor_field_get_mf_color(field, 1);
  ts_assert_vec3_equal(doubleArray[0], doubleArray[1], doubleArray[2], 0.5, 0.4, 0.74,
                       "Item 1 of \"skyColor\" field of Background node should be [0.5, 0.4 0.74] not [%f, %f, %f]",
                       doubleArray[0], doubleArray[1], doubleArray[2]);
  doubleArray = wb_supervisor_field_get_mf_color(field, -1);
  ts_assert_vec3_equal(doubleArray[0], doubleArray[1], doubleArray[2], 0.666667, 1.0, 0,
                       "Item 2 of \"skyColor\" field of Background node should be [0.666667, 1, 0] not [%f, %f, %f]",
                       doubleArray[0], doubleArray[1], doubleArray[2]);
  doubleArray = wb_supervisor_field_get_mf_color(field, 3);
  ts_assert_pointer_null((void *)doubleArray,
                         "When requesting a MFColor field item with index out of bounds, the result should be NULL");
  doubleArray = wb_supervisor_field_get_mf_color(field, -4);
  ts_assert_pointer_null((void *)doubleArray,
                         "When requesting a MFColor field item with index out of bounds, the result should be NULL");

  // set item
  doubleVec3[0] = 0.0;
  doubleVec3[1] = 0.0;
  doubleVec3[2] = 0.5;
  wb_supervisor_field_set_mf_color(field, 0, doubleVec3);
  doubleArray = wb_supervisor_field_get_mf_color(field, 0);
  ts_assert_vec3_equal(
    doubleArray[0], doubleArray[1], doubleArray[2], doubleVec3[0], doubleVec3[1], doubleVec3[2],
    "Item 0 of \"skyColor\" field of Background node should be [%f, %f, %f] not [%f, %f, %f] after changing it", doubleVec3[0],
    doubleVec3[1], doubleVec3[2], doubleArray[0], doubleArray[1], doubleArray[2]);

  // set items with wrong indices, this shouldn't crash neither Webots, nor the controller
  wb_supervisor_field_set_mf_color(field, 3, doubleVec3);
  wb_supervisor_field_set_mf_color(field, -1, doubleVec3);
  wb_supervisor_field_set_mf_color(NULL, 0, doubleVec3);

  // get NULL field
  doubleArray = wb_supervisor_field_get_mf_color(NULL, 0);
  ts_assert_pointer_null((void *)doubleArray, "Returned value for a NULL MFColor field should be NULL");

  // get / set with wrong MF type
  field = wb_supervisor_node_get_field(texture, "url");
  i = wb_supervisor_field_get_type(field);
  ts_assert_boolean_equal(i == WB_MF_STRING, "Texture \"url\" field should have type %d not %d", WB_MF_STRING, i);
  doubleArray = wb_supervisor_field_get_mf_color(field, 0);
  ts_assert_pointer_null((void *)doubleArray, "Returned value for non MFColor fields should be NULL");

  // set SFFloat on a different field type
  wb_supervisor_field_set_mf_color(field, 0, doubleArray);
  charArray = wb_supervisor_field_get_mf_string(field, 0);
  ts_assert_string_equal(charArray, "textures/white256.png",
                         "Returned value for Texture url field should be \"textures/white256.png\", not \"%s\"", charArray);

  // get / set with wrong SF type
  field = wb_supervisor_node_get_field(material, "emissiveColor");
  i = wb_supervisor_field_get_type(field);
  ts_assert_boolean_equal(i == WB_SF_COLOR, "Material \"emissiveColor\" field should have type %d not %d", WB_SF_COLOR, i);
  doubleArray = wb_supervisor_field_get_mf_color(field, 0);
  ts_assert_pointer_null((void *)doubleArray, "Returned value for non MFColor fields should be NULL");
  doubleArray = wb_supervisor_field_get_sf_color(field);
  ts_assert_vec3_equal(doubleArray[0], doubleArray[1], doubleArray[2], 1, 0.666, 0,
                       "Material \"emissiveColor\" field should be [1, 0.666, 0] not [%f, %f, %f]", doubleArray[0],
                       doubleArray[1], doubleArray[2]);

  // set SFFloat on a different field type
  wb_supervisor_field_set_mf_color(field, 0, doubleArray);
  doubleArray = wb_supervisor_field_get_sf_color(field);
  ts_assert_vec3_equal(doubleArray[0], doubleArray[1], doubleArray[2], 1, 0.666, 0,
                       "Material \"emissiveColor\" field should be [1, 0.666, 0] not [%f, %f, %f]", doubleArray[0],
                       doubleArray[1], doubleArray[2]);

  // test SF field getters and setters
  // SFBool
  field = wb_supervisor_node_get_field(shape, "castShadows");
  bool castShadowsValue = wb_supervisor_field_get_sf_bool(field);
  ts_assert_boolean_equal(castShadowsValue, "'wb_supervisor_field_get_sf_bool' failed");
  wb_supervisor_field_set_sf_bool(field, false);
  castShadowsValue = wb_supervisor_field_get_sf_bool(field);
  ts_assert_boolean_equal(!castShadowsValue, "'wb_supervisor_field_set_sf_bool' failed");

  // SFInt32
  field = wb_supervisor_node_get_field(proto, "receiver_channel");
  int channel = wb_supervisor_field_get_sf_int32(field);
  ts_assert_int_equal(channel, 5, "'wb_supervisor_field_get_sf_int32' failed");
  wb_supervisor_field_set_sf_int32(field, 12);
  channel = wb_supervisor_field_get_sf_int32(field);
  ts_assert_int_equal(channel, 12, "'wb_supervisor_field_set_sf_int32' failed");

  // SFString
  field = wb_supervisor_node_get_field(box, "name");
  const char *sf_value1 = wb_supervisor_field_get_sf_string(field);
  ts_assert_string_equal(sf_value1, "solid(1)", "'wb_supervisor_field_get_sf_string' failed: read \"%s\" instead of \"solid\"",
                         sf_value1);
  wb_supervisor_field_set_sf_string(field, "wrong");
  wb_supervisor_field_set_sf_string(field, "obstacle");
  const char *sf_value2 = wb_supervisor_field_get_sf_string(field);
  ts_assert_string_equal(sf_value2, "obstacle",
                         "'wb_supervisor_field_set_sf_string' failed: reading \"%s\" instead of \"obstacle\"", sf_value2);

  // SFVec2f
  field = wb_supervisor_node_get_field(plane, "size");
  double vector2_expected[2] = {1.0, 1.0};
  const double *vector2_default = wb_supervisor_field_get_sf_vec2f(field);
  ts_assert_doubles_equal(2, vector2_default, vector2_expected, "'wb_supervisor_field_get_sf_vec2f' failed");
  vector2_expected[0] = 2.1;
  vector2_expected[1] = 5.0;
  wb_supervisor_field_set_sf_vec2f(field, vector2_expected);
  const double *vector2_modified = wb_supervisor_field_get_sf_vec2f(field);
  ts_assert_doubles_equal(2, vector2_modified, vector2_expected, "'wb_supervisor_field_set_sf_vec2f' failed");

  // SFVec3f
  field = wb_supervisor_node_get_field(box, "translation");
  double vector3_expected[3] = {-0.2, 0.2, -0.2};
  const double *vector3_default = wb_supervisor_field_get_sf_vec3f(field);
  ts_assert_doubles_equal(3, vector3_default, vector3_expected, "'wb_supervisor_field_get_sf_vec3f' failed");
  vector3_expected[0] = 0.123;
  vector3_expected[1] = 0.0;
  vector3_expected[2] = 0.7777;
  wb_supervisor_field_set_sf_vec3f(field, vector3_expected);
  const double *vector3_modified = wb_supervisor_field_get_sf_vec3f(field);
  ts_assert_doubles_equal(3, vector3_modified, vector3_expected, "'wb_supervisor_field_set_sf_vec3f' failed");

  // SFRotation
  field = wb_supervisor_node_get_field(box, "rotation");
  double rotation_expected[4] = {0.70710339, 0.5000024, -0.5000024, 0.5708};  // values taken from the world file
  const double *rotation_default = wb_supervisor_field_get_sf_rotation(field);
  // Note: the small difference (1e8) we get here is due to the normalization of the rotation vector performed by Webots after
  // loading the original rotation value. Therefore, the initial rotation value is slightly different from the one read in the
  // world file.
  ts_assert_doubles_in_delta(4, rotation_default, rotation_expected, 1e-8, "'wb_supervisor_field_get_sf_rotation' failed");
  rotation_expected[0] = 0.0;
  rotation_expected[1] = 1.0;
  rotation_expected[2] = 0.0;
  rotation_expected[2] = 0.2585;
  wb_supervisor_field_set_sf_rotation(field, rotation_expected);
  const double *rotation_modified = wb_supervisor_field_get_sf_rotation(field);
  ts_assert_doubles_equal(4, rotation_modified, rotation_expected, "'wb_supervisor_field_set_sf_rotation' failed");

  // SFNode
  field = wb_supervisor_node_get_field(shape, "geometry");
  WbNodeRef boxGeom = wb_supervisor_field_get_sf_node(field);
  ts_assert_boolean_equal(boxGeom == wb_supervisor_node_get_from_def("BOX_GEOM"), "'wb_supervisor_field_get_sf_node' failed");
  field = wb_supervisor_node_get_field(box, "physics");
  WbNodeRef boxPhysics = wb_supervisor_field_get_sf_node(field);
  ts_assert_pointer_null(boxPhysics, "'wb_supervisor_field_get_sf_node' should return NULL for empty SFNode");

  // test MF field getters and setters
  // MFBool
  field = wb_supervisor_node_get_field(mfTest, "mfBool");
  bool mf_bool = wb_supervisor_field_get_mf_bool(field, 0);
  ts_assert_boolean_equal(!mf_bool, "'wb_supervisor_field_get_mf_bool' failed");
  wb_supervisor_field_set_mf_bool(field, 0, true);
  mf_bool = wb_supervisor_field_get_mf_bool(field, 0);
  ts_assert_boolean_equal(mf_bool, "'wb_supervisor_field_set_mf_bool' failed");

  // MFInt32
  field = wb_supervisor_node_get_field(mfTest, "mfInt");
  int mf_int = wb_supervisor_field_get_mf_int32(field, 1);
  ts_assert_int_equal(mf_int, 0, "'wb_supervisor_field_get_mf_int32' failed");
  wb_supervisor_field_set_mf_int32(field, 1, 11);
  mf_int = wb_supervisor_field_get_mf_int32(field, 1);
  ts_assert_int_equal(mf_int, 11, "'wb_supervisor_field_set_mf_int32' failed");

  // MFString
  field = wb_supervisor_node_get_field(mfTest, "mfString");
  ts_assert_string_equal(wb_supervisor_field_get_mf_string(field, 3), "test string",
                         "'wb_supervisor_field_get_mf_string' failed");
  wb_supervisor_field_set_mf_string(field, 3, "ô");
  ts_assert_string_equal(wb_supervisor_field_get_mf_string(field, 3), "ô", "'wb_supervisor_field_get_mf_string' failed");

  // MFVec2f
  field = wb_supervisor_node_get_field(mfTest, "mfVec2");
  double mf_vector2_expected[2] = {1.0, 1.0};
  const double *mf_vector2_default = wb_supervisor_field_get_mf_vec2f(field, 1);
  ts_assert_doubles_equal(2, mf_vector2_default, mf_vector2_expected, "'wb_supervisor_field_get_mf_vec2f' failed");
  mf_vector2_expected[0] = 2.1;
  mf_vector2_expected[1] = 5.0;
  wb_supervisor_field_set_mf_vec2f(field, 1, mf_vector2_expected);
  const double *mf_vector2_modified = wb_supervisor_field_get_mf_vec2f(field, 1);
  ts_assert_doubles_equal(2, mf_vector2_modified, mf_vector2_expected, "'wb_supervisor_field_set_mf_vec2f' failed");

  // MFVec3f
  field = wb_supervisor_node_get_field(mfTest, "mfVec3");
  double mf_vector3_expected[3] = {-0.2, 0.2, -0.2};
  const double *mf_vector3_default = wb_supervisor_field_get_mf_vec3f(field, -2);
  ts_assert_doubles_equal(3, mf_vector3_default, mf_vector3_expected, "'wb_supervisor_field_get_mf_vec3f' failed");
  mf_vector3_expected[0] = 0.123;
  mf_vector3_expected[1] = 0.0;
  mf_vector3_expected[2] = 0.7777;
  wb_supervisor_field_set_mf_vec3f(field, -2, mf_vector3_expected);
  const double *mf_vector3_modified = wb_supervisor_field_get_mf_vec3f(field, -2);
  ts_assert_doubles_equal(3, mf_vector3_modified, mf_vector3_expected, "'wb_supervisor_field_get_mf_vec3f' failed");

  // MFRotation
  field = wb_supervisor_node_get_field(mfTest, "mfRotation");
  double mf_rotation_expected[4] = {0.70717411, 0.49995239, -0.49995239, 0.5708};  // values from the world file
  const double *mf_rotation_default = wb_supervisor_field_get_mf_rotation(field, 0);
  ts_assert_doubles_equal(4, mf_rotation_default, mf_rotation_expected, "'wb_supervisor_field_get_mf_rotation' failed");
  mf_rotation_expected[0] = 0.0;
  mf_rotation_expected[1] = 1.0;
  mf_rotation_expected[2] = 0.0;
  mf_rotation_expected[2] = 0.2585;
  wb_supervisor_field_set_mf_rotation(field, 0, mf_rotation_expected);
  const double *mf_rotation_modified = wb_supervisor_field_get_mf_rotation(field, 0);
  ts_assert_doubles_equal(4, mf_rotation_modified, mf_rotation_expected, "'wb_supervisor_field_set_mf_rotation' failed");

  // MFNode
  field = wb_supervisor_node_get_field(mfTest, "mfNode");
  WbNodeRef node = wb_supervisor_field_get_mf_node(field, -1);
  ts_assert_boolean_equal(wb_supervisor_node_get_type(node) == WB_NODE_BALL_JOINT, "'wb_supervisor_field_get_mf_node' failed");

  // test the initial position of the robot before starting the simulation: it should be (0, 0, 0) according to world file
  field = wb_supervisor_node_get_field(proto, "translation");
  const double *vector3_current = wb_supervisor_field_get_sf_vec3f(field);
  ts_assert_vec3_equal(vector3_current[0], vector3_current[1], vector3_current[2], 0, 0, 0, "Robot not in initial position");

  // supervisor field tracking
  field = wb_supervisor_node_get_field(box, "translation");
  ts_assert_pointer_not_null(field, "Translation field is not found");
  wb_supervisor_field_enable_sf_tracking(field, TIME_STEP);

  for (int i = 0; i < 10; i++) {
    wb_robot_step(TIME_STEP);
    vector3_modified = wb_supervisor_field_get_sf_vec3f(field);
    vector3_expected[0] = 0.123;
    vector3_expected[1] = 0;
    vector3_expected[2] = 0.7777;
    ts_assert_doubles_equal(3, vector3_modified, vector3_expected,
                            "Field tracking failed, should be [%lf, %lf, %lf] instead of [%lf, %lf, %lf]", vector3_expected[0],
                            vector3_expected[1], vector3_expected[2], vector3_modified[0], vector3_modified[1],
                            vector3_modified[2]);
  }
  wb_supervisor_field_disable_sf_tracking(field);

  // test resetting the translation field of a moving robot
  for (i = 0; i < 10; i++)
    wb_robot_step(TIME_STEP);
  vector3_current = wb_supervisor_field_get_sf_vec3f(field);
  double vector3_start[3];
  for (i = 0; i < 3; i++)
    vector3_start[i] = vector3_current[i];
  for (i = 0; i < 10; i++)
    wb_robot_step(TIME_STEP);
  double vector3_end[3];
  vector3_current = wb_supervisor_field_get_sf_vec3f(field);
  for (i = 0; i < 3; i++)
    vector3_end[i] = vector3_current[i];
  wb_supervisor_field_set_sf_vec3f(field, vector3_start);
  for (i = 0; i < 10; i++)
    wb_robot_step(TIME_STEP);
  vector3_current = wb_supervisor_field_get_sf_vec3f(field);
  double vector3[3];
  for (i = 0; i < 3; i++)
    vector3[i] = vector3_current[i] - vector3_end[i];
  // We get a difference (about 1cm). This is probably due to the fact that the robot doesn't have
  // exactly the same dynamics on the first run and the second run due to the accumulated inertia.
  ts_assert_vec3_in_delta(vector3[0], vector3[1], vector3[2], 0, 0, 0, 0.01,
                          "Moving robot reset translation test failed: error is (%f, %f, %f)", vector3[0], vector3[1],
                          vector3[2]);

  // test setting a value, running the simulation for a couple of steps and reading the value
  field = wb_supervisor_node_get_field(box, "translation");
  vector3_expected[0] = 0.456;
  vector3_expected[1] = 0.0;
  vector3_expected[2] = 0.8888;
  wb_supervisor_field_set_sf_vec3f(field, vector3_expected);
  wb_robot_step(TIME_STEP);
  wb_robot_step(TIME_STEP);
  vector3_modified = wb_supervisor_field_get_sf_vec3f(field);
  ts_assert_doubles_equal(3, vector3_modified, vector3_expected, "Delayed 'wb_supervisor_field_set_sf_vec3f' failed");

  // test internal SFFloat was read-only (wb_supervisor_field_set_sf_float failed)
  // field reference is invalid after regeneration
  int internal_field_type = wb_supervisor_field_get_type(internal_field);
  printf("internal_field_type %d\n", internal_field_type);
  ts_assert_int_equal(internal_field_type, 0, "Internal field reference is invalid after PROTO regeneration");
  internal_field = wb_supervisor_node_get_proto_field(proto, "cpuConsumption");
  printf("internal_field %p\n", internal_field);
  ts_assert_pointer_not_null(internal_field, "Node reference should be invalid after PROTO regeneration");
  d = wb_supervisor_field_get_sf_float(internal_field);
  ts_assert_double_equal(d, 1.11, "Returned value should be 1.11 and not %f", d);

  // multiple MFFloat field operations in one step
  wb_robot_step(TIME_STEP);
  field = wb_supervisor_node_get_field(mfTest, "mfFloat");
  wb_supervisor_field_set_mf_float(field, 0, 1.0);
  wb_supervisor_field_set_mf_float(field, 1, 2.0);
  double mf_float_expected = wb_supervisor_field_get_mf_float(field, 0);
  ts_assert_double_equal(mf_float_expected, 1.0, "Consecutive wb_supervisor_field_set_mf_float: first set instruction failed.");
  wb_robot_step(TIME_STEP);
  mf_float_expected = wb_supervisor_field_get_mf_float(field, 1);
  ts_assert_double_equal(mf_float_expected, 2.0,
                         "Consecutive wb_supervisor_field_set_mf_float: second set instruction failed.");
  wb_robot_step(TIME_STEP);

  // supervisor field update
  self = wb_supervisor_node_get_self();
  supervisor_field = wb_supervisor_node_get_field(self, "supervisor");
  ts_assert_boolean_equal(wb_supervisor_field_get_sf_bool(supervisor_field), "'supervisor' field should be true");
  ts_assert_boolean_equal(wb_robot_get_supervisor(), "'wb_robot_get_supervisor' field should return true");
  wb_supervisor_field_set_sf_bool(supervisor_field, false);
  wb_robot_step(TIME_STEP);

  ts_assert_boolean_equal(!wb_robot_get_supervisor(), "'wb_robot_get_supervisor' field should return false");
  const int self_id = wb_supervisor_node_get_id(self);
  ts_assert_int_equal(
    self_id, -1,
    "Returned value for 'wb_supervisor_node_get_id' field should be '-1' when 'supervisor' field is set to FALSE and not '%d'",
    self_id);

  ts_send_success();
  return EXIT_SUCCESS;
}
