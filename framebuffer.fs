#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D screenTexture;

// for 3x3 kernels
const float offset = 1.0/300.0f;

void main()
{
    float kernel[9] = float[](
        1/9.0f, 1/9.0f, 1/9.0f,
        1/9.0f, 1/9.0f, 1/9.0f,
        1/9.0f, 1/9.0f, 1/9.0f
        // -1.0, 9.0, -1.0,
        // -1.0, -1.0, -1.0
    );

    vec2 offsets[9] = vec2[](
         vec2(-offset, offset),
         vec2(0.0f, offset),
         vec2(offset, offset),
         vec2(-offset, 0.0f),
         vec2(0.0f, 0.0f),
         vec2(offset, 0.0f),
         vec2(-offset, -offset),
         vec2(0.0f, -offset),
         vec2(offset, -offset)
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; ++i) {
        sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; ++i)
        col += sampleTex[i] * kernel[i];

    color = vec4(vec3(1.0f - col), 1.0f);
    // color = texture(screenTexture, texCoords);
}


// archive

// color = texture(screenTexture, texCoords);
// float ave = (2 * color.r + 3 * color.g + 1 * color.b) / 6.0f;
// color = vec4(ave, ave, ave, 1.0f);
// color = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0f);
// float kernel[9] = float[](
//     4 / 25.0, 2 / 25.0, 4 / 25.0,
//     2 / 25.0, 1 / 25.0, 2 / 25.0,
//     4 / 25.0, 2 / 25.0, 4 / 25.0
// );