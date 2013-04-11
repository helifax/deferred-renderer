/// \file DeferredShading.h
/// \Creating the atributes and methods required for Deferred Shading
/// \author Octavian Mihai Vasilovici
/// \version 1.4
/// \date 28/03/2013 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 28/03/2013

#pragma once //Mostly for Visual Studio compiler
#ifndef _DEFERREDSHADING_H_
#define _DEFERREDSHADING_H_
// The maximum number of shaders we can load since we only have "N" types of lights
// this value is arbitrary and will later be read from a config file.
// _MAXIMUM_LIGHTS_SHADERS_ is used to create an array of OBJLoader type.
// It can be switched with a vector later on but in this case an array is better since
// we dont require the extra features (and bigger memory allocation) that a vector gives.
#ifndef  _MAXIMUM_LIGHTS_SHADERS_
#define _MAXIMUM_LIGHTS_SHADERS_ (29) 
#endif

//Aditional File Includes
#include <vector>
#include <ngl/ShaderLib.h>
#include <ngl/Camera.h>
#include <ngl/Material.h>
#include "ObjectLoader.h"
#include "ModelLoader.h"

enum SHADERTYPE
  {
  LIGHT = 0, /// @brief Light type shader
  MESH = 1,  /// @brief Mesh type shader
  GEOMETRY = 2 /// @brief Geometry type shader. Not used anywhere yet.
  };

class DeferredShading
{

public:
	/// @brief VAO arrays for the cone and the base
	ngl::VAOPrimitives *prim;

	/// @brief ctor setting the same window size as the main application
	DeferredShading(
									int _width,
									int _height
									);
	
	/// @brief dtor removing all the pointer that are created
	~DeferredShading()	
	{ 
		std::cout<<"Removing the OBJLoader object. Shuting down.\n";
		delete m_camera;
		delete *m_deferredShader;
	}

	/// @brief Initializng Scene.
	void initScene();

	/// @brief Set width and height for re-draw
	void setResizeScene(
											int &_width,
											int &_height
											);
	
	/// @brief Load the specified shaders for light computation
	/// @brief Expected:
	/// @brief Shader number (first shader number is 0),
	/// @brief Shader program name,
	/// @brief Vertex shader filename,
	/// @brief Fragment shader filename
	void loadDeferredShader(
													GLint _shaderNumber,
													SHADERTYPE _type,
													std::string _shaderProgramName,
													std::string _vertexShaderFileName,
													std::string _fragmentShaderFileName
													);
	
	/// @brief Save our camera location to be used when calculating the different lights
	void setCameraLocation(
												ngl::Camera *_camera
												);

	/// @brief Setup the renderer to process a new frame.
	void startDeferredFrame();

	/// @brief Setup the renderer for Geometry pass.
	void doGeometryPass();
	
	/// @brief Setup the renderer for Point Lightning pass.
	void startStencilPointLight(
															GLint &_shaderNumber,
															ngl::TransformStack &_tx,
															GLint &_lightIndex
															);

	/// @brief Render the scene using the specified _shaderNumber and the transformation stack using point light.
	void deferredPointLight(
													GLint &_shaderNumber,
													ngl::TransformStack &_tx,
													GLint &_lightIndex
													);

	/// @brief Setup the renderer for Spot Lightning pass.
	void startStencilSpotLight(
														GLint &_shaderNumber,
														ngl::Transformation &_trans,
														ngl::TransformStack &_tx,
														GLint &_lightIndex
														);

	/// @brief Render the scene using the specified _shaderNumber and the transformation stack using point light.
	void deferredSpotLight(
												GLint &_shaderNumber,
												ngl::Transformation &_trans,
												ngl::TransformStack &_tx,
												GLint &_lightIndex
												);

	/// @brief Render the final scene using the specified _shaderNumber and directional light.
	void deferredDirectionalLight(
																GLint &_shaderNumber,
																ngl::TransformStack &_tx,
																GLint &_lightIndex
																);

	/// @brief Render the final scene.
	void deferredLightFinalPass();

	/// @brief Creates a light type and assigns it to a specific _shaderNumber.
	void setLightInScene(
											GLint _shaderNumber,
											ngl::Vec3 _lightPosition,
											ngl::Colour _diffuseColor,
											ngl::Colour _specularColor,
											ngl::LIGHTMODES _lightType,
											ngl::Real _Radius,
											ngl::Real _Height,
											ngl::Vec3 _lightLookAt
											);

	/// @brief Sets Constant, Linear and Quadratic light attenuations for the light.
	// It has sense only for SpotLights and Directional Lights.
	void setLightAttenuation (
														GLint _shaderNumber,
														ngl::Real _constant,
														ngl::Real _linear,
														ngl::Real _quadratic
														);

	/// @brief Sets custom Light radius and cutoff for point lights calculations
	// It has sense only for SpotLights and Directional Lights.
	// Light Cutoff values are between 0..1.
	void setPointLightAttenuation (
														GLint _shaderNumber,
														ngl::Real _LightRadius,
														ngl::Real _lightCutoff
														);

	/// @brief Sets the CutOff and Exponent for a spot light.
	void setSpotLightCutOffAndExponent(
																		GLint _shaderNumber,
																		ngl::Real _exponent,
																		ngl::Real _cutOffAngle
																		);

	/// @brief Load a specified OBJ file.
	void loadModel(
								GLint _shaderNumber,
								const std::string _modelName,
								ngl::Material _material,
								bool _useMaterial
								);

	/// @brief Load specified textures. Order is: TextureMap, NormalMap, SpecularMap.
	void loadTextures(
										GLint _modelIndex,
										const int _numberOfTextures,
										const std::string _textureName1,
										const std::string _textureName2,
										const std::string _textureName3
								);
	
	/// @brief Sets position, scale, rotation for current model
	void setModelPosScaleRot(
											GLint _modelIndex,
											ngl::Vec3 _position,									
											ngl::Vec3 _scale,
											ngl::Vec3 _rotation
											);
	
	/// @brief Gets position for current model
	ngl::Vec3 getModelPosition(																	
												GLint _modelIndex
												);

	/// @brief Gets scale for current model
	ngl::Vec3 getModelScale(
										GLint _modelIndex
										);

	/// @brief Gets rotation for current model
	ngl::Vec3 getModelRotation(
												GLint _modelIndex					
												);

	/// @brief Draw specified model. 
	void drawModel(
								ngl::TransformStack &_tx
								);

	/// @brief Draw specified model. 
	GLint getLoadedModels();

	/// @brief Return the number of loaded shaders.
	GLint getLoadedShaders() const;

	/// @brief Get the number of lights associated with the current _shaderNumber.
	GLint getLightIndex(GLint &_shaderNumber) const;

	/// @brief Get the type for _lightIndex light attached to _shaderNumber.
	ngl::LIGHTMODES getLightType(
															GLint &_shaderNumber,
															GLint &_lightIndex
															) const;

	/// @brief Get the Color of the light attached to _shaderNumber.
	ngl::Colour getLightColor(GLint &_shaderNumber,
														GLint &_lightIndex
														) const;

	/// @brief Get the Color of the light attached to _shaderNumber. It stores the un-altered value first declared.
	ngl::Colour getOriginalLightColor(GLint &_shaderNumber,
														GLint &_lightIndex
														) const;

	/// @brief Set the Color of the light attached to _shaderNumber.
	void setLightColor(GLint &_shaderNumber,
										GLint &_lightIndex,
										ngl::Colour &_color
										);

	// Add value to light position.
	/// @brief Usefull when a light position needs to be moved based on the mouse.
	void addToLightPosition(
													GLint _shaderNumber,
													GLint _lightIndex,
													ngl::Real _valueX,
													ngl::Real _valueY,
													ngl::Real _valueZ);

	// Substract value from light position.
	/// @brief Usefull when a light position needs to be moved based on the mouse.
	void subFromLightPosition(
														GLint _shaderNumber,
														GLint _lightIndex,
														ngl::Real _valueX,
														ngl::Real _valueY,
	
														ngl::Real _valueZ);
	
	/// @brief Initializes the scene. Loads the shaders, lights, models, texture from the .ini file.
	void loadFromInitFile();

		/// @brief Generate SSAO.
	void generateSSAOPass(GLint &_shaderNumber,
												ngl::TransformStack &_tx);
	

private:
	/// @brief Array of pointer to ObjectLoader type for the number of shaders for lights.
	ObjectLoader *m_deferredShader[_MAXIMUM_LIGHTS_SHADERS_];
	
	/// @brief Identifiers for:
	/// @brief depth texture
	/// @brief position texture
	/// @brief normal texture
	/// @brief color + diffuse texture
	/// @brief tangent texture
	/// @brief binormal texture
	/// @brief specularity texture
	/// @brief normalMap texture
	/// @brief texture containing the final composition after all the light are rendered
	GLuint m_depthTex;
	GLuint m_posTex;
	GLuint m_normTex;
	GLuint m_colorTex;
	GLuint m_tangentTex;
	GLuint m_binormalTex;
	GLuint m_specularTex;
	GLuint m_normalMapTex;
	GLuint m_finalTex;

	/// @brief Index containing the created and loaded shaders.
	GLint m_shaderNumber;
	
	// Needs to be equal to screen width
	/// @brief Used for calculting TexCoord in the light passes
	int m_renderWidth; 
	
	// Needs to be equal to screen height
	/// @brief Used for calculting TexCoord in the light passes
	int m_renderHeight; 
	
	/// @brief Frame Buffer Object for deferred shading
	GLuint m_deferredFBO; 

	/// @brief the Quad (2 triangles) for directional light rendering
	GLuint m_quad;

	/// @brief Our Camera used for light calculations. Is the same as the one for the whole scene.
	ngl::Camera *m_camera;

	/// @brief This vector will contain all the objects and textures that we will use to render.
	std::vector <ModelLoader*> m_models;
	
	//std::vector< std::vector<ModelLoader*> > m_models;

	/// @brief This vector will contain the materials per object.
	std::vector <ngl::Material>m_model_material;
	
	/// @brief This vector will the information is material is used or not.
	std::vector <bool>m_model_material_use;

	/// @brief Index containing all the models loaded so far to this shader.
	GLint m_modelIndex;

	/// @brief Vectors for position, scale, and rotation
	std::vector <ngl::Vec3> m_modelPosition;
	std::vector <ngl::Vec3> m_modelScale;
	std::vector <ngl::Vec3> m_modelRotation;

private:
	/// @brief Creates the deffered Frame Buffer Object and creates the MRTs.
	void setFBO();

};

#endif //_DEFERREDSHADING_H_
