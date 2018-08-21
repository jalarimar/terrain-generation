#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 colour;

uniform mat4 mvpMatrix;


out vec2 v_TexCoord;
out vec3 v_Col;

void main()
{
   //pass through shader, position in world coords

   gl_Position = vec4(position, 1.0);
   v_TexCoord = texCoord;
   v_Col = colour; 
}
