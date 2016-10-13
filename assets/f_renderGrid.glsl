#version 330

in vec2 fTexCoord0;

uniform sampler2D uGridSampler;

out vec4 FragColor;

float invert(float x) {
  return 1.0 - clamp(x, 0.0, 1.0);
}

vec3 fromRGB(int r, int g, int b) {
  return vec3(r / 255.0, g / 255.0, b / 255.0);
}

void main() {
  vec4 gridValues = texture(uGridSampler, fTexCoord0);
  float A = gridValues.g;
  float B = gridValues.b;

  // vec3 baseColor = vec3(0.0, A, B);

  // vec3 baseColor = vec3(1.0 - step(0.2, B));

  // vec3 baseColor = vec3(1.0 - smoothstep(0.5, 1.0, B + 0.5));
  // baseColor = invert(step(0.01, B)) * cos(sin(baseColor));

  vec3 baseColor = vec3(tan(4.2 * B), sin(5.1423 * B), cos(4.12312 * B)) * step(0.01, B);

  // vec3 darkTurquoise = fromRGB(3, 109, 104);
  // vec3 white = fromRGB(255, 255, 255);
  // vec3 darkblue = fromRGB(0, 0, 80);
  // vec3 turquoise = fromRGB(48, 213, 200);
  // float normB = clamp(B + step(0.5, B) * 0.5, 0.0, 1.0);
  // float normB = step(0.01, B) * mix(0.3, 1.0, B);
  // float normB = smoothstep(0.5, 1.0, B + 0.5);
  // float normB = step(0.2, B);
  // float normB = smoothstep(0.13, 0.3, B);
  // vec3 baseColor = mix(darkblue, turquoise, normB);

  FragColor = vec4(baseColor, 1.0);
}
