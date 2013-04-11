#version 400 core


layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord; //Not used in this shader
layout (location = 2) in vec3 VertexNormal;

uniform bool Normalize;

out vec3 Position;
out vec2 TexCoord;
out vec3 fragmentNormal;


uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform mat3 normalMatrix;


void main()
{
// calculate the fragments surface normal
Position = vec3( MV * vec4(VertexPosition,1.0) );
fragmentNormal = normalMatrix*VertexNormal;
gl_Position = MVP*vec4(VertexPosition,1.0);
}