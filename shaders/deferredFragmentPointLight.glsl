#version 400

// precision related
precision highp sampler2D;
precision highp float;

struct Lights
{
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float radius;
  float cutoff;
};
uniform Lights light;

//Geometry related
uniform vec2 gScreenSize;
vec2 TexCoord = gl_FragCoord.xy / gScreenSize;

//Light related
uniform sampler2D PositionTex, ColorTex, NormalTex, NormalMapTex, TangentTex, BiNormalTex, SpecularTex;

layout (location = 0) out vec4 FragColor;

vec4 DirectIllumination(vec3 pos, vec3 norm, vec3 normalMap, vec3 tangent, vec3 binormal, vec3 lightPos, vec4 diff, vec4 spec, float lightRadius, float cutoff)
{ 
	vec4 diffuse=vec4(0);
	vec4 ambient=vec4(0);
	vec4 specular=vec4(0);
	
    vec3 s = (lightPos - pos);	//surface // DO NOT NORMALIZE IT!!!
	vec3 v = normalize( vec3(-pos)); //view vector
	vec3 h = normalize ( v + s); //half vector
	float r = lightRadius;
    float distance = length(s);
    float d = max(distance - lightRadius, 0);
    s /= distance;
     
    // calculate basic attenuation
    float denom = d/lightRadius + 1;
    float attenuation = 1 / (denom*denom);
     
    // scale and bias attenuation such that:
    //   attenuation == 0 at extent of max influence
    //   attenuation == 1 when d == 0
    attenuation = (attenuation - cutoff) / (1 - cutoff);
    attenuation = max(attenuation, 0);
	
	//color and NormalMap 
	float sDotN = dot(s, norm); // surface Dot normal
	float sDotT = dot(s, tangent);
	float sDotB = dot(s, binormal);
	
	vec3 final_color_factor = vec3(sDotT, sDotB, sDotN);
	float ColorFactor= max( dot( final_color_factor, normalMap), 0.0);
	
	//specularity only
	float hDotN =  dot(h, norm); // halfVector Dot Normal
	float hDotT =  dot(h, tangent);
	float hDotB =  dot(h, binormal);
   	
	vec3 final_spec_factor = vec3(hDotT, hDotB, hDotN);
	float SpecFactor = pow ( max( dot( final_spec_factor, normalMap), 0.0), 50.0);
	
	//if we dont use normalMaps
	if(ColorFactor == 0)
	{
		diffuse+=diff*light.diffuse * max(sDotN, 0.0) * attenuation;
		ambient+=diff*light.ambient * attenuation;
		
		if (hDotN > 0.0)
		{
			specular += spec*light.specular*pow( max(hDotN, 0.0), 50.0) * attenuation;//material.shininess);
		}
	}
	
	else if (ColorFactor > 0.0)
		{
			// Compute components
			diffuse+=diff*light.diffuse * ColorFactor * attenuation;
			ambient+=diff*light.ambient * attenuation;
			
			if (SpecFactor > 0.0)
				{
					specular += spec*light.specular* SpecFactor * attenuation;//material.shininess);
				}
		}
	
    return diffuse + ambient + specular;
}

void LightPass()
{
	// Retrieve position and normal information from textures
    vec3 pos = vec3( texture( PositionTex, TexCoord ) );
	vec3 norm = normalize(vec3(texture(NormalTex, TexCoord)));
	vec3 normalMap = normalize(vec3(texture(NormalMapTex, TexCoord)));
	vec3 tangent = normalize(vec3( texture( TangentTex, TexCoord)));
	vec3 binormal = normalize(vec3( texture( BiNormalTex, TexCoord))); 
    vec4 diffColor = texture(ColorTex, TexCoord);
	vec4 specColor = texture(SpecularTex, TexCoord);
	
	FragColor = DirectIllumination(pos, norm, normalMap, tangent, binormal, light.position.xyz, diffColor, specColor, light.radius, light.cutoff);
}

void main() 
{    
	LightPass();
}
