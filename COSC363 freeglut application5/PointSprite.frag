#version 400

uniform sampler2D sprite;

void main() 
{
	gl_FragColor = texture(sprite, gl_PointCoord).zyxw + vec4(0.1,0.1,0.1,0);
	if(gl_FragColor.a < 0.3) discard;
}
