#version 400

uniform sampler2D sprite;

void main() 
{
	gl_FragColor = vec4(1,1,1,1) - texture(sprite, gl_PointCoord);
	if(gl_FragColor.a > 0.1) discard;
}
