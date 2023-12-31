#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2


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
	int type;
	vec3 lightPosition;
	vec3 direction;
	vec3 diffusedLightColor;
	float intensity;
	float range;
	float innerAngle;
	float outerAngle;
} light;

uniform vec3 ambientLightColor;

vec3 ads(in vec3 position, in vec3 normal)
{
	// AMBIENT
	vec3 ambient = ambientLightColor;

	// ATTENUATION
	float attenuation = 1;
	if (light.type != DIRECTIONAL)
	{
		float distanceSqr = dot(light.lightPosition - position, light.lightPosition - position);
		float rangeSqr = light.range * light.range;
		attenuation = max(0,1 - pow((distanceSqr/rangeSqr), 2.0));
		attenuation = attenuation * attenuation;
	}

	// DIFFUSE
	// calculating where the light is comfing from
	vec3 lightDirection = (light.type == DIRECTIONAL) ? normalize(-light.direction) : normalize(light.lightPosition - fposition);

	float spotIntensity = 1;
	if (light.type == SPOT)
	{
		float angle = acos(dot(light.direction, -lightDirection));
		//if (angle > light.innerAngle) spotIntensity = 0;
		spotIntensity = smoothstep(light.outerAngle + 0.001, light.innerAngle, angle);
	}

	float intensity = max(dot(lightDirection, normal), 0) * spotIntensity;
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
		specular = material.specular * intensity * spotIntensity;
	}

	// Combining ambient, diffuse, and specular
	return ambient + (diffuse + specular) * light.intensity * attenuation;
}

void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1);
}
