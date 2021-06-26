#version 330 core

precision highp float;
in vec4 fragmentPosition;

out vec4 depth;

void main() {
  float zd = fragmentPosition.z / fragmentPosition.w;
  float zw = (gl_DepthRange.far - gl_DepthRange.near) * zd + gl_DepthRange.near;
  depth = vec4(zw, 0, 0, 0);
}
