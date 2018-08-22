#version 400

uniform sampler2D sprite;

void main() 
{
	gl_FragColor = texture(sprite, gl_PointCoord) + vec4(0.2, 0.2, 0.2, 0);
	if(gl_FragColor.a < 0.1) discard;
}
