/// \file ObjectLoader.h
/// \Creating the atributes and methods required for OBJ loading along with textures and shaders.
/// \author Octavian Mihai Vasilovici
/// \version 0.5
/// \date 28/03/2013 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 28/03/2013
/// \todo

#pragma once //Mostly for Visual Studio compiler
#ifndef _OBJECTLOADER_H_
#define _OBJECTLOADER_H_

//Aditional File Includes

#include <ngl/ShaderLib.h>
#include <ngl/Light.h>
#include <ngl/VertexArrayObject.h>
#include "ConeLightVolume.h"
#include "SphereLightVolume.h"
#include "SpotLight.h"

class ObjectLoader
{

public:
	/// @brief ctor for loading model and attached textures and shaders. It creates the shader
	ObjectLoader(
							const std::string _shaderProgramName,
							const std::string _vertexShaderFileName,
							const std::string _fragmentShaderFilename
							);
	/// @brief ctor for loading model and textures using shaders that already exist. It uses an existing shader
	ObjectLoader(
							const std::string 
							_shaderProgramName
							);
	/// dctor
	~ObjectLoader() {;}

public:
	/// @brief vector containing all the point lights in the scene
	std::vector <ngl::Light*> m_light;
	std::vector <ngl::Light*> m_light_original;

	/// @brief vector containing all the point lights in the scene
	std::vector <Spotlight*> m_spotlight;

	/// @brief  vector containing all the light types in a scene
	std::vector <ngl::LIGHTMODES> m_lightType;

	/// @brief  Create a vector of pointers to the specific Light Type Geometry
	std::vector <ConeLightVolume*> m_coneVolume;
	std::vector <SphereLightVolume*> m_sphereVolume;

	/// @brief
	GLuint getShaderProgramHandle() const;
	/// @brief Set the current shader as the active one. All the information sent to the
	/// shader will be on this one.
	void setShaderAsActive() const;

protected:
	/// @brief Pointer to a shader program
	ngl::ShaderLib *m_shader;
	/// @brief String used to make a new Shader program
	std::string m_shaderProgramName;
	/// @brief String used to load a new vertex shader
	std::string m_vertexShaderName;
	/// @brief String used to load a new pixel shader
	std::string m_fragmentShaderName;
	
};
#endif //_OBJECTLOADER_H_
