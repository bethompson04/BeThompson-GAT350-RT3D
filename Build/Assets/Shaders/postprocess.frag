#version 430

#define	INVERT_MASK     (1 << 0)
#define GRAYSCALE_MASK  (1 << 1)
#define COLORTINT_MASK  (1 << 2)

in layout(location = 1) vec2 ftexcoord;
out layout(location = 0) vec4 ocolor;

uniform float blend = 1;
uniform uint params = 0;

layout(binding = 0) uniform sampler2D screenTexture;

vec4 invert(in vec4 color)
{
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 grayscale(in vec4 color)
{
	return vec4(vec3((color.r + color.g + color.b) / 3), color.a);
}

void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = invert(basecolor);

	if (bool(params & INVERT_MASK)) postprocess = invert(postprocess);
	if (bool(params & GRAYSCALE_MASK)) postprocess = grayscale(postprocess);
	if (bool(params & INVERT_MASK)) postprocess = invert(postprocess);



	ocolor = mix(basecolor, postprocess, blend);
}
