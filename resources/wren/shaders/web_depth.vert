#version 330 core

layout(location = 0) in vec3 vCoord;

uniform mat4 modelTransform;

out vec4 fragmentPosition;
// Camera transforms for this frame
layout(std140) uniform CameraTransforms {
  mat4 view;
  mat4 projection;
  mat4 infiniteProjection;
}
cameraTransforms;

void main() {
  mat4 modelView = cameraTransforms.view * modelTransform;

  fragmentPosition = cameraTransforms.projection * cameraTransforms.view * modelTransform * vec4(vCoord, 1.0);
  gl_Position = cameraTransforms.projection * cameraTransforms.view * modelTransform * vec4(vCoord, 1.0);
}
