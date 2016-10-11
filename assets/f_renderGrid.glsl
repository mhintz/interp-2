#version 330

in vec2 fTexCoord0;

uniform sampler2D uGridSampler;

out vec4 FragColor;

float invert(float x) {
  return 1.0 - clamp(x, 0.0, 1.0);
}

vec3 fromRGB(vec3 rgb) {
  return vec3(rgb.r / 255, rgb.g / 255, rgb.b / 255);
}

vec3 fromRGB(int r, int g, int b) {
  return vec3(r / 255, g / 255, b / 255);
}

void main() {
  vec4 gridValues = texture(uGridSampler, fTexCoord0);
  float A = gridValues.g;
  float B = gridValues.b;

  // float normB = clamp(B + step(0.5, B) * 0.5, 0.0, 1.0);
  float normB = step(0.01, B) * (0.3 + B * 0.7);

  // vec3 baseColor = vec3(0.0, A, B);
  // vec3 baseColor = vec3(1.0 - step(0.2, B));
  // vec3 baseColor = vec3(1.0 - smoothstep(0.5, 1.0, B + 0.5));

  vec3 turquoise = fromRGB(vec3(3.0, 109.0, 104.0));
  vec3 white = fromRGB(255, 255, 255);
  vec3 baseColor = mix(white, turquoise, normB);

  FragColor = vec4(baseColor, 1.0);
}
