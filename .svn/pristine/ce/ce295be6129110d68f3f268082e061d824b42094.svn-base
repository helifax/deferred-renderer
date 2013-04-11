#include <ngl/VAOPrimitives.h>
#include "DeferredShading.h"

void DeferredShading::deferredDirectionalLight(
																							GLint &_shaderNumber,
																							ngl::TransformStack &_tx,
																							GLint &_lightIndex
																							)
{
	
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

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		// Set our shader as active
		ngl::ShaderLib *shader=ngl::ShaderLib::instance();
		m_deferredShader[_shaderNumber]->setShaderAsActive();
		
		// Making the light stay fixed relative to the world. 
		// This is the only way to make it "stick". Multiplying by the transpose matrix doesn't work for unknown reasons...
		ngl::Vec4 m_currPos = m_deferredShader[_shaderNumber]->m_light[_lightIndex]->getPos();
		ngl::Mat4 globalMV =  _tx.getGlobalTransform().getMatrix() * m_camera->getViewMatrix();
		// For rotation
		m_currPos = m_currPos * globalMV;
		// For position
		m_currPos.m_x = m_currPos.m_x + globalMV.m_30;
		m_currPos.m_y = m_currPos.m_y + globalMV.m_31;
		m_currPos.m_z = m_currPos.m_z + globalMV.m_32;
		ngl::Vec4 result = ngl::Vec4(m_currPos.m_x, m_currPos.m_y, m_currPos.m_z, m_currPos.m_w);
					
		// load these values to the shader as well
		m_deferredShader[_shaderNumber]->m_light[_lightIndex]->loadToShader("light");
		shader->setShaderParam4f("light.position",result.m_x, result.m_y, result.m_z, result.m_w);	

		shader->setShaderParam2f("gScreenSize", m_renderWidth, m_renderHeight);
		_tx.pushTransform();
		{
			// transformation matrices
			ngl::Mat4 MVP;
			MVP.identity();
			shader->setShaderParamFromMat4("MVP",MVP);

			// Render the quad
			glBindVertexArray(m_quad);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisable(GL_BLEND);
		}
		_tx.popTransform();
	}
	else
		{
			std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot render with an unknown shader! \n";
		}
}
//----------------------------------------------------------------------------------------------------------------------
