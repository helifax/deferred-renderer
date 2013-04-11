#version 400 core
#pragma optionNV(unroll all)

uniform sampler2D tex;
uniform sampler2D spec;
uniform sampler2D normalMap;

in vec3 Position;
in vec2 TexCoord;
in vec3 fragmentNormal;
in vec3 Tangent;
in vec3 Binormal;

layout (location = 0) out vec3 PositionData;
layout (location = 1) out vec4 ColorData;
layout (location = 2) out vec3 NormalData;
layout (location = 3) out vec3 TangentData;
layout (location = 4) out vec3 BinormalData;
layout (location = 5) out vec3 NormalMap;
layout (location = 6) out vec4 MaterialSpecularity;



struct Materials
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

uniform Materials material;
uniform int use_material;

void GeometryPass()
{
    PositionData = Position;
	
	vec3 normal_Map=normalize( texture(normalMap, TexCoord).xyz *2.0 -1.0);
	
	vec4 diffuseTexture = texture(tex, TexCoord.st);
	vec4 specularTexture=texture(spec, TexCoord.st);
	vec4 Color = vec4(0,0,0,0);
	Color +=diffuseTexture;
    NormalData = fragmentNormal;
	NormalMap = normal_Map;
	TangentData = Tangent;
	BinormalData = Binormal;
	
	if (use_material == 1)
	{
		// We keep the ambient to half value since we dont want another render target nor do we want to influence the diffuse color too muuch.
		ColorData = Color + material.diffuse + (material.ambient * 0.5);
		MaterialSpecularity = material.specular + specularTexture ;
	}
	else
	{
		ColorData = Color;
		MaterialSpecularity = specularTexture ;
	}
}


void main ()
{
GeometryPass();
}
