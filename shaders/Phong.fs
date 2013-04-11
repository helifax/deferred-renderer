#version 400 core

// precision related
precision highp float;

in vec3 Position;
in vec4 fragmentNormal;

layout (location = 0) out vec3 PositionData;
layout (location = 1) out vec4 ColorData;
layout (location = 2) out vec4 NormalData;
layout (location = 6) out vec4 MaterialSpecularity;


struct Materials
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

uniform Materials material;

void GeometryPass()
{
    // Store position, normal, and diffuse color in textures
    PositionData = Position;
	// We keep the ambient to half value since we dont want another render target nor do we want to influence the diffuse color too muuch.
    ColorData = material.diffuse + (material.ambient * 0.5);
	NormalData = fragmentNormal;
	MaterialSpecularity = material.specular;
}


void main ()
{
GeometryPass();
}

