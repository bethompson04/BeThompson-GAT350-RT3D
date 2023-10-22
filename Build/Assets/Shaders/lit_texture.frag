in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;



layout(binding = 0) uniform sampler2D tex;

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
	// calculating where the light is comfing from
	vec3 lightDirection = normalize(light.lightPosition - fposition);
	float intensity = max(dot(lightDirection, normal), 0);

	vec3 diffuse = material.diffuse * (light.diffusedLightColor * intensity);

	// SPECULAR
	vec3 specular = vec3(0);
	if (intensity > 0)
	{
		// calculating reflection 
		vec3 reflection = reflect(-lightDirection, fnormal);
		// calculating direction
		vec3 viewDirection = normalize(-fposition);
		intensity = max(dot(reflection, viewDirection), 0);
		intensity = pow(intensity, material.shine);
		specular = material.specular * intensity;
	}

	// Combining ambient, diffuse, and specular
	return ambient + diffuse + specular;
}

void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1);
}
