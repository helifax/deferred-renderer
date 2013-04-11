#version 400

struct SpotLight
{
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec3 direction;  // Direction of the spotlight in eye coords.
  float exponent;  // Angular attenuation exponent
  float cutoff;    // Cutoff angle (between 0 and 90)
  float linearAttenuation;
  float constantAttenuation;
  float quadraticAttenuation;
};

uniform SpotLight light;
//Geometry related
uniform vec2 gScreenSize;
vec2 TexCoord = gl_FragCoord.xy / gScreenSize;

//Light related

in vec3 eyeDirection;
uniform sampler2D PositionTex, ColorTex, NormalTex, NormalMapTex, TangentTex, BiNormalTex, SpecularTex;

layout (location = 0) out vec4 FragColor;

vec4 spotLight( vec3 pos, vec3 norm, vec3 normalMap, vec3 tangent, vec3 binormal, vec4 diff, vec4 spec)
{
  float attenuation;  
  vec4 diffuse=vec4(0);
  vec4 ambient=vec4(0);
  vec4 specular=vec4(0);
  {
	vec3 direction1 = vec3(light.direction);
	
	// Compute vector from surface to light position
	vec3 s = normalize ( vec3(light.position) -pos); //normalize IT???????????????????
	vec3 v = normalize(vec3(-pos));
    vec3 h = normalize( v + s ); //eyeDirection;
	vec3 spotDir = normalize(direction1);
	float angle = acos( dot(-s, spotDir) );
    float cutoff = radians( clamp( light.cutoff, 0.0, 90.0 ) );
	
	
	// Compute attenuation
    float distance = length (vec3 (light.position) - pos);
	float atten = 1.f / (light.constantAttenuation +
                         light.linearAttenuation * distance +
                         light.quadraticAttenuation * distance * distance);
	
	ambient+=diff*light.ambient *atten;		
	
	if( angle < cutoff ) 
	{
        float spotFactor = pow( dot(-s, spotDir), light.exponent );
        vec3 v = normalize(vec3(-pos));
        vec3 h = normalize( v + s ); //eyeDirection;
		
		//float sDotN = max(dot(s, norm), 0.0); // surface Dot normal
		//float hDotN = max(dot(h, norm), 0.0); // halfVector Dot Normal
		
		
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
		
		if(ColorFactor == 0)
		{
				diffuse+=diff*light.diffuse * sDotN * spotFactor *atten;	
				ambient+=diff*light.ambient *atten;	
			
				if (hDotN > 0.0)
				{
					specular += spec*light.specular * pow( max(hDotN, 0.0), 50.0) * attenuation;//material.shininess);
				}
		}
		
		else if (ColorFactor > 0.0)
		{
		
			diffuse+=diff*light.diffuse * ColorFactor * spotFactor *atten;	
			ambient+=diff*light.ambient *atten;	
				
			if (SpecFactor > 0.0)
			{
				specular += spec*light.specular * SpecFactor *atten;//material.shininess);
			}
		}
	return ambient + diffuse + specular;
    }
	else
	{
	return ambient;
	}
  }
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
	
	FragColor = spotLight(pos, norm, normalMap, tangent, binormal, diffColor, specColor);
}

void main() 
{    
	LightPass();
}
