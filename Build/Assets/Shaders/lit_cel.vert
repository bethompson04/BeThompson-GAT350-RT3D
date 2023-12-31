#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;
out layout(location = 3) vec4 oshadowcoord;
out layout(location = 4) vec3 oviewdir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 shadowVP;

uniform struct Material
{
	uint params;

	vec3 albedo;
	vec3 specular;
	vec3 emissive;
	float shine;
	
	vec2 offset;
	vec2 tiling;
} material;

void main()
{
	mat4 modelView = view * model;

	// Convert Position and Normal to world view space
	oposition = vec3(modelView * vec4(vposition,1));
	onormal = normalize(mat3(modelView) * vnormal);
	otexcoord = (vtexcoord * material.tiling) + material.offset;

	//calculate view direction, the oposition has already been moved to world view space
	oviewdir = normalize(-oposition);

	oshadowcoord = shadowVP * model * vec4(vposition, 1);

	mat4 mvp = projection * modelView;
	gl_Position = mvp * vec4(vposition, 1.0);
}
