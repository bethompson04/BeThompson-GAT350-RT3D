#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec2 otexcoord;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec4 ocolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material
{
	vec4 color;
	vec2 offset;
	vec2 tiling;
} material;

uniform struct Light
{
	//vec3 color;
	vec3 lightPosition;
	vec3 ambientLightColor;
	vec3 diffusedLightColor;
} light;



void main()
{
	otexcoord = (vtexcoord * material.tiling) + material.offset;
	onormal = vnormal;

	mat4 modelView = view * model;
	vec4 position = modelView * vec4(vposition,1);
	vec3 normal = normalize(mat3(modelView) * vnormal);
	vec3 lightDirection = normalize(light.lightPosition - position.xyz);
	float intensity = max(dot(lightDirection, normal), 0);

	vec3 lightColor = (light.diffusedLightColor * intensity) + light.ambientLightColor;
	ocolor = material.color * vec4(lightColor, 1);

	mat4 mvp = projection * model * view;
	gl_Position = mvp * vec4(vposition, 1.0);
}
