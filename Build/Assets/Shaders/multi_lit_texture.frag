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
} lights[3];

uniform vec3 ambientLightColor;
uniform int numLights = 3;

float attenuation(in vec3 position1, in vec3 position2, in float range)
{
	float distanceSqr = dot(position1 - position2, position1 - position2);
	float rangeSqr = pow(range, 2.0);
	float attenuation = max(0, 1 - pow((distanceSqr / rangeSqr), 2.0));
	attenuation = pow(attenuation, 2.0);
 
	return attenuation;
}

void phong(in Light light, in vec3 position, in vec3 normal, out vec3 diffuse, out vec3 specular)
{
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
	diffuse = material.diffuse * (light.diffusedLightColor * intensity);

	// SPECULAR
	specular = vec3(0);
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
}

void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	// set ambient light
	ocolor = vec4(ambientLightColor, 1) * texcolor;
 
	// set lights
	for (int i = 0; i < numLights; i++)
	{
		vec3 diffuse;
		vec3 specular;
 
		float attenuation = (lights[i].type == DIRECTIONAL) ? 1 : attenuation(lights[i].lightPosition, fposition, lights[i].range);
 
		phong(lights[i], fposition, fnormal, diffuse, specular);
		ocolor += ((vec4(diffuse, 1) * texcolor) + vec4(specular, 1)) * lights[i].intensity * attenuation;
	}
}
