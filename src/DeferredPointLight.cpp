#include <ngl/VAOPrimitives.h>
#include <boost/lexical_cast.hpp>
#include "DeferredShading.h"

void DeferredShading::startStencilPointLight(
																						GLint &_shaderNumber,
																						ngl::TransformStack &_tx,
																						GLint &_lightIndex
																						)
{
	// Ending geometry pass
	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);
  
	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		ngl::ShaderLib *shader=ngl::ShaderLib::instance();
		m_deferredShader[_shaderNumber]->setShaderAsActive();

		// transformation matrices
		ngl::Mat4 MVP;
		ngl::Mat4 MV;
		ngl::Mat4 M;

		// Making the light stay fixed relative to the world. 
		ngl::Vec4 m_currPos = m_deferredShader[_shaderNumber]->m_light[_lightIndex]->getPos();
		ngl::Mat4 globalMV =  _tx.getGlobalTransform().getMatrix() * m_camera->getViewMatrix();
		m_currPos= globalMV.transpose() * m_currPos;
		ngl::Vec4 result = ngl::Vec4(m_currPos.m_x, m_currPos.m_y, m_currPos.m_z, m_currPos.m_w);
		
		//Get the LightRadius and CutOff values
		ngl::Real _lightRadius = m_deferredShader[_shaderNumber]->m_sphereVolume[_lightIndex]->getLightRadius();
		ngl::Real _lightCutOff = m_deferredShader[_shaderNumber]->m_sphereVolume[_lightIndex]->getLightCutOff();
		ngl::Colour m_diffuse = m_deferredShader[_shaderNumber]->m_light[_lightIndex]->getColour();
		ngl::Colour m_specular = m_deferredShader[_shaderNumber]->m_light[_lightIndex]->getSpecColour();

		shader->setShaderParam4f("light.ambient",0.5,0.5,0.5,0.5);
    shader->setShaderParam4f("light.diffuse",m_diffuse.m_r,m_diffuse.m_g,m_diffuse.m_b,m_diffuse.m_a);
    shader->setShaderParam4f("light.specular",m_specular.m_r,m_specular.m_g,m_specular.m_b,m_specular.m_a);
		shader->setShaderParam4f("light.position",result.m_x, result.m_y, result.m_z, result.m_w);	
		shader->setShaderParam1f("light.radius", _lightRadius);
		shader->setShaderParam1f("light.cutoff",_lightCutOff);


		// Render the light volume for the specified light attached to this shader
		glDisable(GL_CULL_FACE);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 0, 0);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	
		m_currPos = m_deferredShader[_shaderNumber]->m_light[_lightIndex]->getPos();
		_tx.pushTransform();
		{
			// set the scale of the Sphere
			ngl::Real m_scale = m_deferredShader[_shaderNumber]->m_sphereVolume[_lightIndex]->getSphereRadius();
			_tx.setScale(m_scale, m_scale, m_scale);
			_tx.setPosition(m_currPos.m_x, m_currPos.m_y, m_currPos.m_z);
			M=_tx.getCurrAndGlobal().getMatrix();
			MV=_tx.getCurrAndGlobal().getMatrix()*m_camera->getViewMatrix();
			MVP= M*m_camera->getVPMatrix();
			shader->setShaderParamFromMat4("MVP",MVP);
			// load these values to the shader as well
			shader->setShaderParam2f("gScreenSize", m_renderWidth, m_renderHeight);
			// draw the light volume
			prim->draw("SphereVolume");
		}
			_tx.popTransform();
	}
	else
		{
			std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot render with an unknown shader! \n";
		}
}
//----------------------------------------------------------------------------------------------------------------------

void DeferredShading::deferredPointLight(
																				GLint &_shaderNumber,
																				ngl::TransformStack &_tx,
																				GLint &_lightIndex
																				)
{

	//bind
	glDrawBuffer(GL_COLOR_ATTACHMENT7);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_posTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_colorTex);
	glActiveTexture(GL_TEXTURE3);	
	glBindTexture(GL_TEXTURE_2D, m_tangentTex);
	glActiveTexture(GL_TEXTURE4);	
	glBindTexture(GL_TEXTURE_2D, m_binormalTex);
	glActiveTexture(GL_TEXTURE5);	
	glBindTexture(GL_TEXTURE_2D, m_normalMapTex);
	glActiveTexture(GL_TEXTURE6);	
	glBindTexture(GL_TEXTURE_2D, m_specularTex);
  

	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		ngl::ShaderLib *shader=ngl::ShaderLib::instance();
		m_deferredShader[_shaderNumber]->setShaderAsActive();

		// transformation matrices
		ngl::Mat4 MVP;
		ngl::Mat4 MV;
		ngl::Mat4 M;

		// Render the light attached to this shader
		
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
			
		ngl::Vec4 m_currPos = m_deferredShader[_shaderNumber]->m_light[_lightIndex]->getPos();
		_tx.pushTransform();
		{
			// set the scale of the Sphere
			ngl::Real m_scale = m_deferredShader[_shaderNumber]->m_sphereVolume[_lightIndex]->getSphereRadius();
			_tx.setScale(m_scale, m_scale, m_scale);
			_tx.setPosition(m_currPos.m_x, m_currPos.m_y, m_currPos.m_z);
			M=_tx.getCurrAndGlobal().getMatrix();
			MV=_tx.getCurrAndGlobal().getMatrix()*m_camera->getViewMatrix();
			MVP= M*m_camera->getVPMatrix();
			shader->setShaderParamFromMat4("MVP",MVP);
			shader->setShaderParam2f("gScreenSize", m_renderWidth, m_renderHeight);
			// draw the light volume
			prim->draw("SphereVolume");
		}
		_tx.popTransform();
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}
	else
		{
			std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot render with an unknown shader! \n";
		}
}
//----------------------------------------------------------------------------------------------------------------------
