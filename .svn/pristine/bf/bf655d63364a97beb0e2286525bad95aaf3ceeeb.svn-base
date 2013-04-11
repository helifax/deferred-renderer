#version 400 core
#pragma optionNV(unroll all)

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord; 
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec3 TangentTexCoord;
layout (location = 4) in vec3 BinormalTexCoord;


uniform mat4 MVP;
uniform mat4 M;
uniform mat4 MV;
uniform mat3 normalMatrix;

out vec3 Position;
out vec2 TexCoord;
out vec3 fragmentNormal;
out vec3 Tangent;
out vec3 Binormal;


void main()
{	
	Position = vec3( MV * vec4(VertexPosition,1.0) );
	TexCoord=VertexTexCoord;
	fragmentNormal = (normalMatrix * VertexNormal);
	Tangent = TangentTexCoord;
	Binormal = BinormalTexCoord;
	gl_Position = MVP*vec4(VertexPosition, 1.0);
}
