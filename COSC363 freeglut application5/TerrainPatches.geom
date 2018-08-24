#version 400

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec2 te_TexCoord[];

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat4 norMatrix;

uniform vec4 lightPos;
uniform float snowLevel;
uniform float waterLevel;

out vec4 g_TexWeight;
out float g_TexWeightSand;
out vec2 g_TexCoord;
out float g_Diffuse;

void main()
{
	// calculates unit normal n and outputs diffuse term n.l
	// takes position in world coords and outputs position in clip coords

	int i;
	for (i = 0; i < gl_in.length(); i++) {
		vec4 movement_offset = vec4(0.0, 0.0, 10.0, 0.0);
		gl_Position = gl_in[i].gl_Position + movement_offset;

		// calculate normal
		vec3 A = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
		vec3 B = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
		vec3 normal = -1 * normalize(cross(A,B));

		vec4 posnEye = mvMatrix * (gl_in[i].gl_Position + movement_offset);
		vec4 lightVec = normalize(lightPos - posnEye);
		vec4 normalEye = norMatrix * vec4(normal, 0);
		float diffTerm = max(dot(lightVec, normalEye), 0); // n.l
		g_Diffuse = diffTerm;

		vec4 weight = vec4(0, 0, 0, 0); // snow, stone, grass, water
		float sandWeight = 0.0f;
		float height = gl_in[i].gl_Position.y;
		float rockLevel = 5.0f;
		float rockgrass_range = 3.0f;
		float snow_range = 1.0f;
		float sand_range = 0.4f;

		if (height < waterLevel) { // water
			float diff = waterLevel - height;
			float range = 1.5 * waterLevel;
			float w = max(0.1, 1 - diff / range);
			weight = vec4(0, 0, 0, w*1.2);
			gl_Position.y = waterLevel;
		} else if (height < snowLevel && height > snowLevel - snow_range && height >= waterLevel + sand_range && height > rockLevel) { // snow rock blend
			float snow_weight = (height - (snowLevel - snow_range)) / snow_range;
			float stone_weight = 1 - snow_weight;
			weight = vec4(snow_weight, stone_weight, 0, 0);
		} else if (height < snowLevel && height > snowLevel - snow_range && height >= waterLevel + sand_range && height < rockLevel && height > rockLevel - rockgrass_range) { // snow grass stone blend
			float snow_weight = (height - (snowLevel - snow_range)) / snow_range;
			float stone_weight = (1 - snow_weight) * (height - (rockLevel - rockgrass_range)) / rockgrass_range;
			float grass_weight = 1 - snow_weight - stone_weight;
			weight = vec4(snow_weight, stone_weight, grass_weight, 0);
		} else if (height < snowLevel && height > snowLevel - snow_range && height >= waterLevel + sand_range) { // snow grass blend
			float snow_weight = (height - (snowLevel - snow_range)) / snow_range;
			float grass_weight = 1 - snow_weight;
			weight = vec4(snow_weight, 0, grass_weight, 0);
		} else if (height > snowLevel) { // snow
			weight = vec4(1, 0, 0, 0);
		} else if (height < rockLevel && height > rockLevel - rockgrass_range && height >= waterLevel + sand_range) { // grass stone blend
			float stone_weight = (height - (rockLevel - rockgrass_range)) / rockgrass_range;
			float grass_weight = 1 - stone_weight;
			weight = vec4(0, stone_weight, grass_weight, 0);
		} else if (height < rockLevel && height >= waterLevel + sand_range) { // grass
			weight = vec4(0, 0, 1, 0);
		} else if (height < rockLevel) { // sand
			sandWeight = 1.0f;
		} else {
			weight = vec4(0, 1, 0, 0); // stone
		}

		gl_Position = mvpMatrix * gl_Position;
		g_TexWeight = weight;
		g_TexWeightSand = sandWeight;
		g_TexCoord = te_TexCoord[i];
		EmitVertex();
	}
	EndPrimitive();
}
