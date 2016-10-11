#version 330

in vec2 fTexCoord0;

uniform sampler2D uGridSampler;

out vec4 FragColor;

void main() {
  vec4 gridValues = texture(uGridSampler, fTexCoord0);
  float A = gridValues.g;
  float B = gridValues.b;

  // vec3 baseColor = vec3(0.0, A, B);
  vec3 baseColor = vec3(1.0 - step(0.2, B));

  FragColor = vec4(baseColor, 1.0);
}
