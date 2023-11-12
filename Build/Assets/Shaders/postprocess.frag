#version 430

#define	INVERT_MASK     (1 << 0)
#define GRAYSCALE_MASK  (1 << 1)
#define COLORTINT_MASK  (1 << 2)
#define GRAIN_MASK      (1 << 3)
#define SCANLINE_MASK   (1 << 4)

in layout(location = 1) vec2 ftexcoord;
out layout(location = 0) vec4 ocolor;

uniform float blend = 1;
uniform uint params = 0;
uniform vec3 tint;
uniform float time = 0;

layout(binding = 0) uniform sampler2D screenTexture;

float random(vec2 st) 
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 invert(in vec4 color)
{
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 grayscale(in vec4 color)
{
	return vec4(vec3((color.r + color.g + color.b) / 3), color.a);
}

vec4 colortint(in vec4 color, in vec3 tintColor)
{
	return vec4(vec3(color.rgb * tintColor.rgb), 1);
}

vec4 grain(in vec4 color)
{
	return color * random(gl_FragCoord.xy + time);
}

vec4 scanline(in vec4 color)
{
	

	return (int(gl_FragCoord.y) % 2 != 0) ? vec4(0,0,0,0) : color;
}


void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	if (bool(params & INVERT_MASK)) postprocess = invert(postprocess);
	if (bool(params & GRAYSCALE_MASK)) postprocess = grayscale(postprocess);
	if (bool(params & COLORTINT_MASK)) postprocess = colortint(postprocess, tint);
	if (bool(params & GRAIN_MASK)) postprocess = grain(postprocess);
	if (bool(params & SCANLINE_MASK)) postprocess = scanline(postprocess);



	ocolor = mix(basecolor, postprocess, blend);
}
