#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 colour;

uniform mat4 mvpMatrix;

void main()
{
	gl_Position = mvpMatrix * vec4(position, 1.0);
	gl_PointSize = 110 - 0.7 * gl_Position.z;
}
