#include <vector>
#include <ngl/VAOPrimitives.h>
#include "DeferredShading.h"


DeferredShading::DeferredShading(
																int _width,
																int _height
																)
{
	m_renderWidth = _width;
	m_renderHeight = _height;
}
//----------------------------------------------------------------------------------------------------------------------

void DeferredShading::setResizeScene(
																		int &_width,
																		int &_height
																		)
{
	m_renderWidth = _width;
	m_renderHeight = _height;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_posTex);
	glClear(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normTex);
	glClear(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_colorTex);
	glClear(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_tangentTex);
	glClear(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_binormalTex);
	glClear(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_normalMapTex);
	glClear(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_specularTex);
	glClear(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_depthTex);
	glClear(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, m_renderWidth, m_renderHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_finalTex);
	glClear(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
}
//----------------------------------------------------------------------------------------------------------------------

void DeferredShading::initScene()
{
	prim = ngl::VAOPrimitives::instance();
	
	//Geometry for PointLight
	prim->createSphere("SphereVolume", 1.0, 60);
	
	//Enabling depth test
	glEnable(GL_DEPTH_TEST); 
	
	// setting the number of light shaders to 0
	m_shaderNumber = -1;

	// Vertices for the final Quad
    GLfloat verts[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };

	// Colours for sampling the position
    GLfloat tc[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
	
	// Set up the buffers for the QUAD
    unsigned int handle[2];
    glGenBuffers(2, handle);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    //static draw since no modifications are needed and will be used alot for over and over again draws.
		glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW); 

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);

    // Set up the vertex array object. This will hold the quad for directional light
    glGenVertexArrays(1, &m_quad );
    glBindVertexArray(m_quad);

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray(0);  // Vertex position buffer

    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
		glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) ); 
    glEnableVertexAttribArray(1);  // Texture coordinates buffer mapped to input 1.
    glBindVertexArray(0);

		setFBO();
}
//----------------------------------------------------------------------------------------------------------------------

void DeferredShading::setFBO()
{

    // Create and bind the FBO
	glGenFramebuffers(1, &m_deferredFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFBO);

	// The position buffer
	// Generated the textures and set the parameters
	// Bind the texture to the buffer
	glGenTextures(1, &m_posTex);
	glBindTexture(GL_TEXTURE_2D, m_posTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // The normal buffer
  glGenTextures(1, &m_normTex);
  glBindTexture(GL_TEXTURE_2D, m_normTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // The color buffer
  glGenTextures(1, &m_colorTex);
  glBindTexture(GL_TEXTURE_2D, m_colorTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// The tangents buffer
	glGenTextures(1, &m_tangentTex);
	glBindTexture(GL_TEXTURE_2D, m_tangentTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// The tangents buffer
	glGenTextures(1, &m_binormalTex);
	glBindTexture(GL_TEXTURE_2D, m_binormalTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// The normalMap buffer
	glGenTextures(1, &m_normalMapTex);
	glBindTexture(GL_TEXTURE_2D, m_normalMapTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	// The material specularity buffer
	glGenTextures(1, &m_specularTex);
	glBindTexture(GL_TEXTURE_2D, m_specularTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_renderWidth, m_renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// The depth buffer
	glGenTextures(1, &m_depthTex);
	glBindTexture(GL_TEXTURE_2D, m_depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, m_renderWidth, m_renderHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// The final image buffer
	glGenTextures(1, &m_finalTex);
	glBindTexture(GL_TEXTURE_2D, m_finalTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_renderWidth, m_renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


  // Attach the images to the framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_posTex, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normTex, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_colorTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_tangentTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_binormalTex, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, m_normalMapTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, m_specularTex, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, m_finalTex, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//----------------------------------------------------------------------------------------------------------------------

// Loads a shader on a location specified by _shaderNumber (that will be used later on for all calculations. Creates an OBJloader object
void DeferredShading::loadDeferredShader(
																				GLint _shaderNumber,
																				SHADERTYPE _type,
																				std::string _shaderProgramName,
																				std::string _vertexShaderFileName,
																				std::string _fragmentShaderFileName
																				)
{
	// if the shader with the _shaderNumber was not previously created and we don't exceed the maximum number of light type shaders.
	if((_shaderNumber > m_shaderNumber) && (m_shaderNumber <= _MAXIMUM_LIGHTS_SHADERS_))
	{
	// create a shader for our specified light type shader
	m_deferredShader[_shaderNumber] = new ObjectLoader(_shaderProgramName, _vertexShaderFileName, _fragmentShaderFileName);
	m_shaderNumber = _shaderNumber;

	
	// Setting the 2D samplers. If we have a lighting shader
	if(_type == LIGHT)
	{
		ngl::ShaderLib *shader = ngl::ShaderLib::instance();
		(*shader)[_shaderProgramName]->setUniform1i("PositionTex", 0);
		(*shader)[_shaderProgramName]->setUniform1i("ColorTex", 1);
		(*shader)[_shaderProgramName]->setUniform1i("NormalTex", 2);
		(*shader)[_shaderProgramName]->setUniform1i("TangentTex", 3);
		(*shader)[_shaderProgramName]->setUniform1i("BiNormalTex", 4);
		(*shader)[_shaderProgramName]->setUniform1i("NormalMapTex", 5);
		(*shader)[_shaderProgramName]->setUniform1i("SpecularTex", 6);
	}
	// Setting Mesh type shader uniforms.
	if(_type == MESH)
	{
		// now bind the shader attributes for most NGL primitives we use the following
		// layout attribute 0 is the vertex data (x,y,z)
		ngl::ShaderLib *shader = ngl::ShaderLib::instance();
		(*shader)[_shaderProgramName]->setUniform1i("Normalize",1);
	}
	}
	// if the shader was previously created.
	else
	{
		std::cout<<"The shader with the number " + _shaderNumber<< " was previously created. Nothing was done! \n";
	}
}
//----------------------------------------------------------------------------------------------------------------------

// Return the number of loaded shaders.
GLint DeferredShading::getLoadedShaders() const
{
	return m_shaderNumber;
}
//----------------------------------------------------------------------------------------------------------------------

// Saves the original camera location to be used in later computations
void DeferredShading::setCameraLocation(
																				ngl::Camera *_camera
																				)
{
	// we save the camera position since it exists only one camera position for all the light types.
	m_camera = _camera;
}
//----------------------------------------------------------------------------------------------------------------------

// Enables the deferred FBO and usage of MRTs
void DeferredShading::startDeferredFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_deferredFBO);
  glDrawBuffer(GL_COLOR_ATTACHMENT7);
  glClear(GL_COLOR_BUFFER_BIT);
}
//----------------------------------------------------------------------------------------------------------------------

// Enables the geometry pass rendering
void DeferredShading::doGeometryPass()
{
	GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
	glDrawBuffers(7, drawBuffers);

	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);	
	// here we draw using OUR SHADER the geometry!!!!! (not the deferred one). In this case it is the "Phong" one loaded in the GlWindow.cpp
}
//----------------------------------------------------------------------------------------------------------------------

// Blends all the textures togheter from the light passes
void DeferredShading::deferredLightFinalPass()
{
	//bind the default FBO for draw and the deffered one to read from the MRTs
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_deferredFBO);
  glReadBuffer(GL_COLOR_ATTACHMENT7);
	//Blend the textures togheter
	glBlitFramebuffer(0, 0, m_renderWidth, m_renderHeight, 0, 0, m_renderWidth, m_renderHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
//----------------------------------------------------------------------------------------------------------------------
	