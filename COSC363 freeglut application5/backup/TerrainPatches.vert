#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 oTexCoord;

uniform mat4 mvpMatrix;
out vec4 gl_Position;


void main()
{
   gl_Position = mvpMatrix * position;
   oTexCoord = texCoord;
   
}
