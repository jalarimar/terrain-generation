#version 400

smooth in vec4 g_TexWeight;
smooth in float g_TexWeightSand;
smooth in vec2 g_TexCoord;
smooth in vec4 g_Col;
smooth in float g_Diffuse;

uniform sampler2D snow;
uniform sampler2D stone;
uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D water;
uniform int is_wireframe;

uniform int pass;

void main() 
{
	// assigns colour based on texture weight

	vec4 snowCol = texture(snow, g_TexCoord * 4);
	vec4 stoneCol = texture(stone, g_TexCoord * 5) + vec4(0.1, 0.1, 0.1, 1);
	vec4 grassCol = texture(grass, g_TexCoord * 7) + vec4(0.1, -0.3, -0.2, 1); // 0.1, 0.5, 0.1 // 0.1, 0.5, 0.4
	vec4 sandCol = texture(sand, g_TexCoord * 5) * vec4(0.88, 1, 0.95, 1);
	vec4 waterCol = texture(water, g_TexCoord * 3);

	vec4 texCol = (snowCol * g_TexWeight.x + stoneCol * g_TexWeight.y + grassCol * g_TexWeight.z + sandCol * g_TexWeightSand + waterCol * g_TexWeight.w);

	vec4 ambient = vec4(0.2, 0.2, 0.2, 1);

	gl_FragColor = (ambient * texCol) + (g_Diffuse * texCol);

	if (is_wireframe == 1) {
		gl_FragColor = vec4(0, 0, 1, 1);
	}
}
