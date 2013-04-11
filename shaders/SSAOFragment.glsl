#version 400

// The shader for SSAO is based on a different technique than the "classic Crysis" one. Instead of using the Depth Buffer it uses
// the position values from the Tex and the ModelView Normal Matrix.
// The implementation is a transcript of the HSLS shader and the implementation can be found at:
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/a-simple-and-practical-approach-to-ssao-r2753

precision highp sampler2D;
precision highp float;

uniform vec2 gScreenSize;
uniform sampler2D PosTex;
uniform sampler2D SampleTex;
uniform sampler2D NormalTex; // in viewspace not screen space
uniform mat3 Projection;


vec2 TexCoord = gl_FragCoord.xy / gScreenSize;
layout (location = 0) out vec4 Color;

//g_scale: scales distance between occluders and occludee.
//g_bias: controls the width of the occlusion cone considered by the occludee.
//g_sample_rad: the sampling radius.
//g_intensity: the ao intensity.

float random_size = 64*64;

float g_scale = 3;
float g_bias = 0.04;
float g_sample_rad = 0.09;
float g_intensity = 0.4;


vec3 getPosition(in vec2 TexCoord)
{
return texture(PosTex,TexCoord).xyz;
}

vec3 getNormal(in vec2 TexCoord)
{
return texture(NormalTex, TexCoord).xyz;
}

vec2 getRandom(in vec2 TexCoord)
{
return normalize(texture(SampleTex, gScreenSize * TexCoord / random_size).xy * 2.0f - 1.0f);
}

float doAmbientOcclusion(in vec2 tcoord,in vec2 TexCoord, in vec3 p, in vec3 cnorm)
{
vec3 diff = getPosition(tcoord + TexCoord) - p;
vec3 v = normalize(diff);
float d = length(diff)*g_scale;
return max(0.0,dot(cnorm,v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}

void main()
{
vec2 vec[4];			
vec[0] = vec2(1,0);
vec[1] = vec2(-1,0);
vec[2] = vec2(0,1);
vec[3] = vec2(0,-1);

vec3 p = getPosition(TexCoord);
//vec3 n = Projection * getNormal(TexCoord); //Normals in screen space.
vec3 n = getNormal(TexCoord); //Normals in screen space.
vec2 rand = getRandom(TexCoord);

float ao = 0.0f;
float rad = g_sample_rad/p.z;

//**SSAO Calculation**//
// 16 samples
int iterations = 16;
vec2 coord1, coord2;
for (int j = 0; j < iterations; ++j)
{
  coord1 = reflect(vec[j],rand)*rad;
  coord2 = vec2(coord1.x*0.707 - coord1.y*0.707,
			  coord1.x*0.707 + coord1.y*0.707);
  
  ao += doAmbientOcclusion(TexCoord,coord1*0.25, p, n);
  ao += doAmbientOcclusion(TexCoord,coord2*0.5, p, n);
  ao += doAmbientOcclusion(TexCoord,coord1*0.75, p, n);
  ao += doAmbientOcclusion(TexCoord,coord2, p, n);
}
ao = 1-(ao /iterations*16.0);

//Do stuff here with your occlusion value �ao�: modulate ambient lighting, write it to a buffer for later //use, etc.
Color.a = ao;
//Color.a = 1;
//Color =vec4(ao, ao, ao,1);
//Color = vec4(n, 0);
}