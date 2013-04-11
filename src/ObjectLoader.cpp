#include <iostream>
#include <ngl/ShaderLib.h>
#include <ngl/Texture.h>
#include <ngl/Obj.h>
#include "ObjectLoader.h"

//Loading a shader both vertex and fragment shaders. Attaching, compiling and linking phases are included.
ObjectLoader::ObjectLoader(
													const std::string _shaderProgramName,
													const std::string _vertexShaderFileName,
													const std::string _fragmentShaderFilename
													)
{
	m_shaderProgramName = _shaderProgramName;
	m_vertexShaderName = _shaderProgramName + "Vertex";
	m_fragmentShaderName = _shaderProgramName + "Fragment";
	
	m_shader = ngl::ShaderLib::instance();
	m_shader->createShaderProgram(_shaderProgramName);

	m_shader->attachShader(_shaderProgramName + "Vertex", ngl::VERTEX);
	m_shader->attachShader(_shaderProgramName + "Fragment", ngl::FRAGMENT);

	m_shader->loadShaderSource(_shaderProgramName + "Vertex", _vertexShaderFileName);
	m_shader->loadShaderSource(_shaderProgramName + "Fragment", _fragmentShaderFilename);

	m_shader->compileShader(_shaderProgramName + "Vertex");
	m_shader->compileShader(_shaderProgramName + "Fragment");

	m_shader->attachShaderToProgram(_shaderProgramName, _shaderProgramName + "Vertex" );
	m_shader->attachShaderToProgram(_shaderProgramName, _shaderProgramName + "Fragment" );

	m_shader->linkProgramObject(_shaderProgramName);

	(*m_shader)[_shaderProgramName]->use();
}
//----------------------------------------------------------------------------------------------------------------------

//Useful when loading a mesh with a shader that already exists
ObjectLoader::ObjectLoader(
													const std::string _shaderProgramName
													)
{
	m_shaderProgramName = _shaderProgramName;
	m_vertexShaderName = _shaderProgramName + "Vertex";
	m_fragmentShaderName = _shaderProgramName + "Fragment";

	m_shader = ngl::ShaderLib::instance();
	
	if(m_shader->getProgramID(_shaderProgramName) != GLint(-1))
	{
	m_shader->linkProgramObject(_shaderProgramName);
	(*m_shader)[_shaderProgramName]->use();
	}
	else
	{
		std::cout<<"The Shader "<<_shaderProgramName<<" does not exist. Nothing was done! \n";
	}
}
//----------------------------------------------------------------------------------------------------------------------

//To be used when loading a mesh using an already existing shader
GLuint ObjectLoader::getShaderProgramHandle() const
{
	return m_shader->getProgramID(m_shaderProgramName);
}
//----------------------------------------------------------------------------------------------------------------------

void ObjectLoader::setShaderAsActive() const
{
	(*m_shader)[m_shaderProgramName]->use();
}
//----------------------------------------------------------------------------------------------------------------------
