/// \file ModelLoader.h
/// \Class that contains all the attributes specific for loading and handling models (obj) + textures.
/// \author Octavian Mihai Vasilovici
/// \version 0.4
/// \date 28/03/2013 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 28/03/2013
/// \todo Fix a the shader for when using only diffuse textures. Known issue

#pragma once //Mostly for Visual Studio compiler
#ifndef _MODELLOADER_H_
#define _MODELLOADER_H_

//Aditional File Includes
#include <ngl/VertexArrayObject.h>
#include "ObjectLoader.h"

class ModelLoader
							
{

public:
	/// @brief default ctor for loading model and attached textures.
	 //ModelLoader();
	/// @brief default dctor
	~ModelLoader() {;}

public:

	// Returns true if mesh is loaded, false otherwise
	bool m_meshIsLoaded;

	/// @brief loads a Mesh, OBJ file.
	/// Method not tested yet.
	void loadMesh(const std::string _meshFileName, const int _shaderNumber);
	/// @brief loads a texture file.
	/// Method not tested yet.
	void loadTexture(
									GLint _numberOfTextures,
									const std::string _textureFileName1,
									const std::string _textureFileName2,
									const std::string _textureFileName3
									);
	/// @brief Draws the current loaded mesh.
	/// Method not tested yet.
	void DrawMesh();

	/// @brief Return the shader used to draw the model.
	int getShaderNumber();

private:
	/// @brief the mesh data vertex array object
	ngl::VertexArrayObject *m_vaoMesh;

	/// @ brief attached shader.
	int m_shader;

	/// @ brief Texture numbers per model.
	GLuint texture, normalTexture, specTexture;

	/// @brief a simple structure to hold our vertex data
struct vertData
{
  GLfloat u; // tex cords from obj
	GLfloat v; // tex cords
	GLfloat nx; // normal from obj mesh
	GLfloat ny;
	GLfloat nz;
	GLfloat x; // position from obj
	GLfloat y;
	GLfloat z;
	GLfloat tx; // tangent calculated by us
	GLfloat ty;
	GLfloat tz;
	GLfloat bx; // binormal (bi-tangent really) calculated by us
	GLfloat by;
	GLfloat bz;
};
	
};
#endif //_MODELLOADER_H_




