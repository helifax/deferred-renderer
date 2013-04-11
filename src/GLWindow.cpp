#include "GLWindow.h"
#include <iostream>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Mat4.h>
#include <ngl/Transformation.h>
#include <ngl/TransformStack.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Texture.h>
#include "DeferredShading.h"

//----------------------------------------------------------------------------------------------------------------------
// Create an instance of our renderer
DeferredShading *g_renderer;
//----------------------------------------------------------------------------------------------------------------------
// Rotation based on the mouse position for our global
// transform
ngl::Transformation trans;
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for x/y translation with mouse movement
//----------------------------------------------------------------------------------------------------------------------
const static float INCREMENT=0.1;
//----------------------------------------------------------------------------------------------------------------------
/// @brief the increment for the wheel zoom
//----------------------------------------------------------------------------------------------------------------------
const static float ZOOM=1;

//----------------------------------------------------------------------------------------------------------------------
// in this ctor we need to call the CreateCoreGLContext class, this is mainly for the MacOS Lion version as
// we need to init the OpenGL 3.2 sub-system which is different than other platforms
//----------------------------------------------------------------------------------------------------------------------
GLWindow::GLWindow(
                   QWidget *_parent
                  )
                    : QGLWidget( new CreateCoreGLContext(QGLFormat::defaultFormat()), _parent )
{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
  // Now set the initial GLWindow attributes to default values
  // Roate is false
  m_rotate=false;
  // mouse rotation values set to 0
  m_spinXFace=0;
  m_spinYFace=0;
  // this timer is going to trigger an event every 40ms which will be processed in the
  //
  m_lightTimer =startTimer(40);
  m_animate=true;
	// used for point light animation to drive the cycles.
	light_animation = false;
  //----------------------------------------------------------------------------------------------------------------------
	//Setup our renderer resolution
	g_renderer = new DeferredShading(this->width(), this->height());
	//----------------------------------------------------------------------------------------------------------------------
}

GLWindow::~GLWindow()
{
	ngl::NGLInit *Init = ngl::NGLInit::instance();
	//----------------------------------------------------------------------------------------------------------------------
	// Removing the Defferedshading object
	std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
	g_renderer->~DeferredShading();
	//----------------------------------------------------------------------------------------------------------------------
	Init->NGLQuit();
}

//----------------------------------------------------------------------------------------------------------------------
// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining VBOs etc.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // we need to initialise the NGL lib, under windows and linux we also need to
  // initialise GLEW, under windows this needs to be done in the app as well
  // as the lib hence the WIN32 define
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  #ifdef WIN32
    glewInit(); // need a local glew init as well as lib one for windows
  #endif
  Init->initGlew();
  //----------------------------------------------------------------------------------------------------------------------
	//Initialize the deferred renderer
	g_renderer->initScene();
	//----------------------------------------------------------------------------------------------------------------------

  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/Phong.vs");
  shader->loadShaderSource("PhongFragment","shaders/Phong.fs");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();
    //shader->setShaderParam1i("Normalize",1);

  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,2,6);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  m_cam= new ngl::Camera(from,to,up,ngl::PERSPECTIVE);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam->setShape(45,(float)16.0/9.0,0.0001,350,ngl::PERSPECTIVE);
  shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

	ngl::Material m(ngl::GOLD);
  // load our material values to the shader into the structure material (see Vertex shader)
  m.loadToShader("material");
  //----------------------------------------------------------------------------------------------------------------------
	// Saving our camera location in order to do all the light calculations
	g_renderer->setCameraLocation(m_cam);
	//----------------------------------------------------------------------------------------------------------------------
	// SSAO shader for generation
	g_renderer->loadDeferredShader(0, LIGHT, "SSAOShader", "shaders/SSAOVertex.glsl", "shaders/SSAOFragment.glsl");
	//----------------------------------------------------------------------------------------------------------------------
	// Load the geometry + light information from the config file
	g_renderer->loadFromInitFile();
	//----------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------
	// Sampler for SSAO Is always loaded.
	ngl::Texture t("SSAO/sample.jpg");
	t.setMultiTexture(10);
	t.setTextureGL();
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	//----------------------------------------------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void GLWindow::resizeGL(
                        int _w,
                        int _h
                       )
{
  glViewport(0,0,_w,_h);
  m_cam->setShape(45,(float)_w/_h,0.0005,350,ngl::PERSPECTIVE);
  //----------------------------------------------------------------------------------------------------------------------
  //set our deferred renderer to the new size
  g_renderer->setResizeScene( _w, _h);
  //----------------------------------------------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void GLWindow::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
	
  // multiply the rotations
  ngl::Mat4 final=rotY*rotX;
  // add the translations
  final.m_m[3][0] = m_modelPos.m_x;
  final.m_m[3][1] = m_modelPos.m_y;
  final.m_m[3][2] = m_modelPos.m_z;
	
  // set this in the TX stack
  trans.setMatrix(final);
  m_transformStack.setGlobal(trans);

	//----------------------------------------------------------------------------------------------------------------------
	// Prepare the buffers for a new frame
	g_renderer->startDeferredFrame();
	//----------------------------------------------------------------------------------------------------------------------
	// Prepare the buffers for a the geometry pass
	g_renderer->doGeometryPass();
	//----------------------------------------------------------------------------------------------------------------------
	// Geometry pass
	drawScene("Phong");
	//----------------------------------------------------------------------------------------------------------------------
	// Set the depth mask and the stencil test to draw pointlights and spotlighs
	glDepthMask(GL_FALSE);	
	//----------------------------------------------------------------------------------------------------------------------
	glEnable(GL_STENCIL_TEST);
	//----------------------------------------------------------------------------------------------------------------------
	//Render using the POINTLIGHT OR SPOTLIGHT.
	for (GLint m_shader=1; m_shader<=g_renderer->getLoadedShaders(); ++m_shader)
	{
		GLint m_lightIndex = g_renderer->getLightIndex(m_shader);
		while (m_lightIndex>=0)
		{
			if (g_renderer->getLightType(m_shader, m_lightIndex) == ngl::POINTLIGHT)
			{
				g_renderer->startStencilPointLight(m_shader, m_transformStack, m_lightIndex);
				g_renderer->deferredPointLight(m_shader, m_transformStack, m_lightIndex);
			}
			else if (g_renderer->getLightType(m_shader, m_lightIndex) == ngl::SPOTLIGHT)
			{
				g_renderer->startStencilSpotLight(m_shader, trans, m_transformStack, m_lightIndex);
				g_renderer->deferredSpotLight(m_shader, trans, m_transformStack, m_lightIndex);
			}
			--m_lightIndex;
		}
	}
	//----------------------------------------------------------------------------------------------------------------------
	// Disable the stencil operation and draw the directional lights.
	glDisable(GL_STENCIL_TEST);
	//----------------------------------------------------------------------------------------------------------------------
	// Render using the Directional Light code.
	for (GLint m_shader=1; m_shader<=g_renderer->getLoadedShaders(); ++m_shader)
	{
		GLint m_lightIndex = g_renderer->getLightIndex(m_shader);
		while (m_lightIndex>=0)
		{
			if (g_renderer->getLightType(m_shader, m_lightIndex) == ngl::DIRECTIONALLIGHT)
			{
				g_renderer->deferredDirectionalLight(m_shader, m_transformStack, m_lightIndex);
			}
			--m_lightIndex;
		}
	}
	//----------------------------------------------------------------------------------------------------------------------
	// Render the SSAO pass binded on shader number 0.
	GLint m_shader = 0;
	g_renderer->generateSSAOPass(m_shader, m_transformStack);
	//----------------------------------------------------------------------------------------------------------------------
	// Copy the content from the final texture to screen.
	g_renderer->deferredLightFinalPass();
	//----------------------------------------------------------------------------------------------------------------------
}

void GLWindow::loadMatricesToShader(
                                     ngl::TransformStack &_tx
                                   )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat4 normalMatrix;
  ngl::Mat4 M;
  M=_tx.getCurrentTransform().getMatrix();
  MV=  _tx.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix();
  MVP=  MV*m_cam->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  //shader->setShaderParamFromMat4("M",M);
}


void GLWindow::drawScene(const std::string &_shader)
{
	// We draw our loaded models
		GLint m_modelsNumber = g_renderer->getLoadedModels();
		while (m_modelsNumber>=0)
		{
			m_transformStack.pushTransform();
			{
				g_renderer->drawModel(m_transformStack);
			}
			m_transformStack.popTransform();
			m_modelsNumber--;
		}

  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)[_shader]->use();
 
   // get the VBO instance and draw the built in teapot
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
		
#ifdef ADDLARGEMODELS
  m_transformStack.pushTransform();
  {
    m_transformStack.setPosition(-15.0,0,1.0);
    m_transformStack.setScale(0.3, 0.3, 0.3);
		m_transformStack.setRotation(0,180,0);
    loadMatricesToShader(m_transformStack);
    prim->draw("dragon");
  } // and before a pop
  m_transformStack.popTransform();

  m_transformStack.pushTransform();
  {
    m_transformStack.setPosition(-9.5, 0,1.0);
    m_transformStack.setScale(0.3, 0.3, 0.3);
		m_transformStack.setRotation(0,180,0);
    loadMatricesToShader(m_transformStack);
    prim->draw("buddah");
  } // and before a pop
  m_transformStack.popTransform();

  m_transformStack.pushTransform();
  {
    m_transformStack.setPosition(-4.5,0,1.0);
    m_transformStack.setScale(0.3, 0.3, 0.3);
		m_transformStack.setRotation(0,180,0);
    loadMatricesToShader(m_transformStack);
    prim->draw("bunny");
  } // and before a pop
  m_transformStack.popTransform();
#endif
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (
                               QMouseEvent * _event
                              )
{

  // note the method buttons() is the button state when event was called
  // this is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx=_event->x()-m_origX;
    int diffy=_event->y()-m_origY;
    m_spinXFace += (float) 0.5f * diffy;
    m_spinYFace += (float) 0.5f * diffx;
    m_origX = _event->x();
    m_origY = _event->y();		
		updateGL();

  }
	// right mouse translate code
  else if(m_translate && _event->buttons() == Qt::RightButton)
  {
		int diffX = (int)(_event->x() - m_origXPos);
		int diffY = (int)(_event->y() - m_origYPos);
		m_origXPos=_event->x();
    m_origYPos=_event->y();
		m_modelPos.m_x += INCREMENT * diffX;
		m_modelPos.m_y -= INCREMENT * diffY;
		//renderer->addToLightPosition(1,0, INCREMENT * (diffX/0.5), 0.0f, 0.0f);
		//renderer->subFromLightPosition(1,0, 0.0f, INCREMENT * (diffY/0.5), 0.0f);
		updateGL();

	}

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
                                QMouseEvent * _event
                               )
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate =true;
  }
	// right mouse translate mode
	else if(_event->button() == Qt::RightButton)
	{
    m_origXPos = _event->x();
    m_origYPos = _event->y();
		m_translate=true;
	}

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (
                                  QMouseEvent * _event
                                 )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
	// right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::wheelEvent(QWheelEvent *_event)
{

	// check the diff of the wheel position (0 means no change)
	if(_event->delta() > 0)
	{
		m_modelPos.m_z+=ZOOM;
		//renderer->addToLightPosition(1,0, 0.0f, 0.0f, ZOOM);
	}
	else if(_event->delta() <0 )
	{
		m_modelPos.m_z-=ZOOM;
		//renderer->subFromLightPosition(1,0, 0.0f, 0.0f, ZOOM);
	}
	updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateLight()
{
	// Makes a simple, "torch-like" animation for point lights
	for (GLint m_shader=1; m_shader<=g_renderer->getLoadedShaders(); ++m_shader)
	{
		// get all the shaders and lights attached
		GLint m_lightIndex = g_renderer->getLightIndex(m_shader);
		while (m_lightIndex>=0)
		{
			// if light is point light animate it.
			if(g_renderer->getLightType(m_shader, m_lightIndex) == ngl::POINTLIGHT)
			{
				// get the current color for the current light. Only alpha channel is important since it will be used to drive the intensity.
				ngl::Colour _color = g_renderer->getLightColor(m_shader,m_lightIndex);
				// get the original color for the current light. 
				ngl::Colour _original_color = g_renderer->getOriginalLightColor(m_shader,m_lightIndex);
				// Increment with 0.08 ~ 2 sec animation at 60 fps for alpha = 1.0;

				// Apply the intensity to the original value
				if((_color.m_a >=0.25) && (_color.m_a <= _original_color.m_a) && (!light_animation))
				{
					_color.m_a = _color.m_a - (_original_color.m_a /2.0/10.0);
				
					_color.m_r = _original_color.m_r * _color.m_a;
					_color.m_g = _original_color.m_g * _color.m_a;
					_color.m_b = _original_color.m_b  *_color.m_a;

					// we set the new color to the light
					_color.set(_color.m_r, _color.m_g, _original_color.m_b, _color.m_a);
					g_renderer->setLightColor(m_shader, m_lightIndex, _color);
				}
				else if((_color.m_a>0.15) && (_color.m_a < _original_color.m_a))
				{
					_color.m_a = _color.m_a + (_original_color.m_a /2.0/10.0);

					_color.m_r = _original_color.m_r * _color.m_a;
					_color.m_g = _original_color.m_g * _color.m_a;
					_color.m_b = _original_color.m_b  *_color.m_a;

					// we set the new color to the light
					_color.set(_color.m_r, _color.m_g, _color.m_b, _color.m_a);
					g_renderer->setLightColor(m_shader, m_lightIndex, _color);
					light_animation = true;
					}
				else // if the animation cycle is done start anew.
				{
					light_animation = false;
				}
			}
			--m_lightIndex;
			}
		}
	}

void GLWindow::timerEvent(
                          QTimerEvent *_event
                         )
{
// if the timer is the light timer call the update light method
  if(_event->timerId() == m_lightTimer && m_animate==true)
  {
    updateLight();
  }
    // re-draw GL
updateGL();
}

void GLWindow::processKey( QKeyEvent *_event)
{
// Nothing to be done atm.
}
