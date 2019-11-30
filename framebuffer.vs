#version 330 core

layout(location = 0) in vec2 aCoord;
layout(location = 1) in vec2 aTexCoord;

out vec2 texCoords;

void main()
{
    gl_Position = vec4(aCoord, 0.0f, 1.0f);
    texCoords  = aTexCoord;
}