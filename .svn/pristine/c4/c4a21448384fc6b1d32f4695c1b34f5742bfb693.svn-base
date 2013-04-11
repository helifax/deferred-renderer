#version 400

struct Lights
{
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float spotCosCutoff;
  float linearAttenuation;
  float constantAttenuation;
  float quadraticAttenuation;
};

uniform Lights light;

//Geometry related
uniform vec2 gScreenSize;
vec2 TexCoord = gl_FragCoord.xy / gScreenSize;

//Light related
uniform sampler2D PositionTex, ColorTex, NormalTex, NormalMapTex, TangentTex, BiNormalTex, SpecularTex;

layout (location = 0) out vec4 FragColor;

	vec4 DirectionalLight( vec3 pos, vec3 norm, vec3 normalMap, vec3 tangent, vec3 binormal, vec4 diff, vec4 spec)
{
	float attenuation;  
	vec4 diffuse=vec4(0);
	vec4 ambient=vec4(0);
	vec4 specular=vec4(0);
	
	// Compute attenuation
    float distance = length (vec3 (light.position) - pos);
	float atten = 1.f / (light.constantAttenuation +
                         light.linearAttenuation * distance +
                         light.quadraticAttenuation * distance * distance);
    vec3 s;
      s = normalize( vec3(light.position) - pos);
	  	  
    vec3 v = normalize(vec3(-pos));
    vec3 r = reflect( -s, norm );
	
	
	//color and NormalMap 
	float sDotN = dot(s, norm); // surface Dot normal
	float sDotT = dot(s, tangent);
	float sDotB = dot(s, binormal);
	
	vec3 final_color_factor = vec3(sDotT, sDotB, sDotN);
	float ColorFactor= max( dot( final_color_factor, normalMap), 0.0);
	
	//specularity only
	float hDotN =  dot(r, norm); // halfVector Dot Normal
	float hDotT =  dot(r, tangent);
	float hDotB =  dot(r, binormal);
   	
	vec3 final_spec_factor = vec3(hDotT, hDotB, hDotN);
	float SpecFactor = pow ( max( dot( final_spec_factor, normalMap), 0.0), 50.0);
	
	
	//if we dont use normalMaps
	if(ColorFactor == 0)
	{
		diffuse+=diff*light.diffuse * max(sDotN, 0.0) * atten;
		ambient = light.ambient* atten;
		specular += spec*light.specular * max(dot(r, v),0.0) * atten;//material.shininess);
	}
	else if (ColorFactor > 0.0)
	{
		diffuse+=diff*light.diffuse * ColorFactor * atten;
		ambient = light.ambient* atten;
		specular += spec*light.specular*SpecFactor* atten;//material.shininess);
    }
	return ambient + diffuse + specular;
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
	
	FragColor = DirectionalLight(pos, norm, normalMap, tangent, binormal, diffColor, specColor);
}

void main() 
{    
	LightPass();
}
