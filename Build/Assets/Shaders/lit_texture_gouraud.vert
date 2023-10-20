#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;
out layout(location = 3) vec4 ocolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shine;

	vec2 offset;
	vec2 tiling;
} material;

uniform struct Light
{
	vec3 lightPosition;
	vec3 diffusedLightColor;
} light;

uniform vec3 ambientLightColor;

vec3 ads(in vec3 position, in vec3 normal)
{
	// AMBIENT
	vec3 ambient = ambientLightColor;

	// DIFFUSE
	vec3 lightDirection = normalize(light.lightPosition - position);
	float intensity = max(dot(lightDirection, normal), 0);

	vec3 diffuse = material.diffuse * (light.diffusedLightColor * intensity);

	// SPECULAR
	vec3 specular = vec3(0);
	if (intensity > 0)
	{
		vec3 reflection = reflect(-lightDirection, normal);
		vec3 viewDirection = normalize(-position);
		intensity = max(dot(reflection, viewDirection), 0);
		intensity = pow(intensity, material.shine);
		specular = material.specular * intensity;
	}

	return ambient + diffuse + specular;
}

void main()
{
	mat4 modelView = view * model;

	// Convert Position and Normal to world view space
	oposition = vec3(modelView * vec4(vposition,1));
	onormal = normalize(mat3(modelView) * vnormal);
	otexcoord = (vtexcoord * material.tiling) + material.offset;


	ocolor = vec4(ads(oposition, onormal), 1);

	mat4 mvp = projection * model * view;
	gl_Position = mvp * vec4(vposition, 1.0);
}
