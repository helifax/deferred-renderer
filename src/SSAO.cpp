#include "DeferredShading.h"

//----------------------------------------------------------------------------------------------------------------------
// The SSAO pass.
// It uses the position and normal texture attachements.
// The result is blended in the texture 7 (COLOR_ATTACHEMENT7) which holds the final image
// The blending is done using the alpha channel since the ssao is outputed in the alpha channel in the shader.
//
void DeferredShading::generateSSAOPass(GLint &_shaderNumber,
																			ngl::TransformStack &_tx
																			)
{
	// We attach our SSAO texture.
	glDrawBuffer(GL_COLOR_ATTACHMENT7);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_posTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normTex);

	// The sample textures used in SSAO
	glActiveTexture(GL_TEXTURE19);
	// The sample texture loaded in the begining.
	glBindTexture(GL_TEXTURE_2D,11);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	// Blending the final image with the SSAO texture. In the shader we store the SSAO values as alpha.
	glBlendFunc(GL_ONE, GL_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE);


	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		// Set our shader as active
		ngl::ShaderLib *shader=ngl::ShaderLib::instance();
		m_deferredShader[_shaderNumber]->setShaderAsActive();
		
		// Set the textures to be used.
		shader->setShaderParam1i("PosTex", 0);
		shader->setShaderParam1i("NormalTex", 2);
		shader->setShaderParam1i("SampleTex", 19);

		// Projection matrix
		ngl::Mat3 ProjectionMatrix = m_camera->getProjectionMatrix();
		shader->setShaderParamFromMat3("Projection",ProjectionMatrix);

		// Set the screensize in order to use the correct TexCoords
		shader->setShaderParam2f("gScreenSize", m_renderWidth, m_renderHeight);
		
		_tx.pushTransform();
		{
			// transformation matrices
			ngl::Mat4 MVP;
			MVP.identity();
			shader->setShaderParamFromMat4("MVP",MVP);

			// Render the quad with the SSAO
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