#version 330 core

out vec4 fragColor;
  
in vec2 outTexCoord;
in vec3 outColor;
in vec3 outLightColor;
in float visibility;

uniform sampler2D tex0;
uniform bool lightEnabled;

uniform bool frame = false;

uniform vec3 clearColor = vec3(0, 0, 0);

uniform vec2 screenRes = vec2(640, 480);
uniform vec2 frameRes = vec2(640, 480);

vec3 greyscale(vec4 color) {
    vec3 tcol = vec3(color.r, color.g, color.b);
    float g = dot(tcol, vec3(0.299, 0.587, 0.114));
    return mix(tcol, vec3(g), color.a);
}

const int ditherMatrix8x8[64] = int[](0,  32, 8,  40, 2,  34, 10, 42,
                                     48, 16, 56, 24, 50, 18, 58, 26,
                                     12, 44, 4,  36, 14, 46, 6,  38,
                                     60, 28, 52, 20, 62, 30, 54, 22,
                                     3,  35, 11, 43, 1,  33, 9,  41,
                                     51, 19, 59, 27, 49, 17, 57, 25,
                                     15, 47, 7,  39, 13, 45, 5,  37,
                                     63, 31, 55, 23, 61, 29, 53, 21);

float indexValue() {
    int x = int(mod(gl_FragCoord.x, 8));
    int y = int(mod(gl_FragCoord.y, 8));
    return ditherMatrix8x8[(x + y * 8)] / 64.0;
}

float ditherChannel(float color, int numLevels) {
    float levelStep = 1.0 / float(numLevels - 1);
    float d = indexValue();

    float quantizedColor = floor(color / levelStep) * levelStep;
    float nextQuantizedColor = clamp(quantizedColor + levelStep, 0.0, 1.0);

    float distance = color - quantizedColor;
    return (distance < d * levelStep) ? quantizedColor : nextQuantizedColor;
}

vec4 dither(vec4 color, int numLevels) {
    return vec4(
        ditherChannel(color.r, numLevels),
        ditherChannel(color.g, numLevels),
        ditherChannel(color.b, numLevels),
        color.a
    );
}

void main() {

    if(frame) {
        fragColor = texture(tex0, outTexCoord);
        return;
    }

    vec4 skyColor = vec4(clearColor/255, 1);

    if(lightEnabled) {
        fragColor = mix(skyColor, texture(tex0, outTexCoord) * vec4(outColor, 1.0) * vec4(outLightColor, 1.0), visibility);
    } else {
        fragColor = texture(tex0, outTexCoord) * vec4(outColor, 1.0);
        fragColor = mix(skyColor, fragColor, visibility);
    }

    fragColor = dither(fragColor, 12);

}
