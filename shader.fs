#version 330 core

out vec4 color;
in vec2 TexCoord;

// uniform vec4 cusColor;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
// uniform float visibility;

void main(){

	// color = vec4(0.2f, 0.5f, 1.0f, 1.0f);
        // color = ourColor;
        // color = vec4(ourColor, 1.0f);
        color = mix(texture(ourTexture1, TexCoord),
                    texture(ourTexture2, TexCoord), 0.4f) *
                vec4(1.0f, 1.0f, 1.0f, 1.0f);
}