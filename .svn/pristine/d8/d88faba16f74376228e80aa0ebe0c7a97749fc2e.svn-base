#include <cmath>
#include <ngl/VAOPrimitives.h>
#include "DeferredShading.h"

void DeferredShading::startStencilSpotLight(GLint &_shaderNumber, ngl::Transformation &_trans, ngl::TransformStack &_tx, GLint &_lightIndex)
{
	// Ending geometry pass
	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);
  
	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		ngl::ShaderLib *shader=ngl::ShaderLib::instance();
		m_deferredShader[_shaderNumber]->setShaderAsActive();
		
		// get the current position of the light
		ngl::Vec4 m_currentPos = m_deferredShader[_shaderNumber]->m_spotlight[_lightIndex]->getPosition();
		ngl::Vec3 m_lookAt = m_deferredShader[_shaderNumber]->m_coneVolume[_lightIndex]->getLightLookAt();
		// get the cone Radius for scaling
		ngl::Real m_scaleX = m_deferredShader[_shaderNumber]->m_coneVolume[_lightIndex]->getRadiusAndHeigth().m_x;
		ngl::Real m_scaleZ = m_scaleX;
		// get the cone height for scaling
		ngl::Real m_scaleY = m_deferredShader[_shaderNumber]->m_coneVolume[_lightIndex]->getRadiusAndHeigth().m_y;
		
		ngl::Mat4 globalMV =  _tx.getGlobalTransform().getMatrix() * m_camera->getViewMatrix();
		ngl::Mat4 transposedMV = globalMV.transpose(); 
		ngl::Vec4 m_lightPos= transposedMV * m_currentPos;
		ngl::Vec4 m_light_lookAt = transposedMV * m_lookAt;
		
		// Load the calculated values to the shader.
		m_deferredShader[_shaderNumber]->m_spotlight[_lightIndex]->loadToShader("light");
		shader->setShaderParam4f("light.position",m_lightPos.m_x, m_lightPos.m_y, m_lightPos.m_z,float(1));
		shader->setShaderParam3f("light.direction", m_light_lookAt.m_x - m_lightPos.m_x, m_light_lookAt.m_y - m_lightPos.m_y, m_light_lookAt.m_z - m_lightPos.m_z);

		// Render the light volume for the specified light attached to this shader
		glDisable(GL_CULL_FACE);
		glClear(GL_STENCIL_BUFFER_BIT);
		//Stencil is set to allways pass
		glStencilFunc(GL_ALWAYS, 0, 0);
		//If both depth and stencil tests pass invert the vertex value. Keep the rest
		//glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		
	
		_tx.pushTransform();
		{
			
			ngl::Vec3 m_l, m_c;
			ngl::Vec3	m_cone_base, m_cross_axis_prod;
			ngl::Vec3 m_light_pos(m_currentPos.m_x, m_currentPos.m_y, m_currentPos.m_z);
			ngl::Real m_dot_angle_prod;


			m_cone_base.m_x = m_light_pos.m_x;
			m_cone_base.m_y = m_light_pos.m_y - 2.5f; //default height is 2.5f units
			m_cone_base.m_z = m_light_pos.m_z;


			//Direction vectors.
			//Light direction.
			m_l = m_lookAt - m_light_pos;
			m_l.normalize();
			//Cone orientation vector.
			m_c = m_cone_base - m_light_pos;	
			m_c.normalize();
  
			//Calculate the angle in radians between the two vectors.
			m_dot_angle_prod = (m_l.dot(m_c));
			//Calculate the axis of rotations between the two vectors.
			m_cross_axis_prod.set(m_l.cross(m_c));
			m_cross_axis_prod.normalize();
	
			//Aquire the actual angle, in radians.
			m_dot_angle_prod = acos(m_dot_angle_prod);
			//Convert the angle in degrees.
			m_dot_angle_prod = m_dot_angle_prod * (180.0f/M_PI);

			//Create the Euler rotation matrix, based on the above angle and rotation axis.
			ngl::Mat4 RotMat;
			RotMat.euler(m_dot_angle_prod, m_cross_axis_prod.m_x, m_cross_axis_prod.m_y, m_cross_axis_prod.m_z);
		
			//Get the current Model Matrix and scale the model.
			ngl::Mat4 OldMat = _tx.getCurrentTransform().getMatrix()	;
	
			RotMat.translate(m_light_pos.m_x, m_light_pos.m_y, m_light_pos.m_z);
			OldMat.scale(m_scaleX, m_scaleY, m_scaleZ);
			RotMat *=OldMat;
	
			//Push the matrix as transform matrix and use it to draw the cone.
			_trans.setMatrix(RotMat);  
			_tx.setCurrent(_trans);

			// transformation matrices
		   ngl::Mat4 M=_tx.getCurrentTransform().getMatrix();
		   ngl::Mat4 MV =_tx.getCurrAndGlobal().getMatrix() * m_camera->getViewMatrix();	
			 ngl::Mat4 MVP=  MV*m_camera->getProjectionMatrix();	

			shader->setShaderParamFromMat4("MVP",MVP);
			// load these values to the shader as well
			shader->setShaderParam2f("gScreenSize", m_renderWidth, m_renderHeight);

			// Draw the cone
			prim->draw("DeferredCone");
		}
			_tx.popTransform();
	}
	else
		{
			std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot render with an unknown shader! \n";
		}
}
//----------------------------------------------------------------------------------------------------------------------

void DeferredShading::deferredSpotLight(GLint &_shaderNumber, ngl::Transformation &_trans, ngl::TransformStack &_tx, GLint &_lightIndex)
{

	//bind the final image texture
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
		ngl::Mat4 MVLIGHT =_tx.getCurrAndGlobal().getMatrix();
		ngl::Mat4 MV;
		ngl::Mat4 M;

		// Render the light attached to this shader
		// This is used for vizualisation only. Key is "V".
		
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
			
		// Draw the same light volume and do the light calculations inside this volume
		// Get the current position of the light
		ngl::Vec4 m_currentPos = m_deferredShader[_shaderNumber]->m_spotlight[_lightIndex]->getPosition();
		ngl::Vec3 m_lookAt = m_deferredShader[_shaderNumber]->m_coneVolume[_lightIndex]->getLightLookAt();
		
		
		// get the cone Radius for scaling
		ngl::Real m_scaleX = m_deferredShader[_shaderNumber]->m_coneVolume[_lightIndex]->getRadiusAndHeigth().m_x;
		ngl::Real m_scaleZ = m_scaleX;
		// get the cone height for scaling
		ngl::Real m_scaleY = m_deferredShader[_shaderNumber]->m_coneVolume[_lightIndex]->getRadiusAndHeigth().m_y;
		// scale the cone for the current light

		//ngl::Mat4 globalMV =  _tx.getGlobalTransform().getMatrix() * m_camera->getViewMatrix();
		//ngl::Mat4 transposedMV = globalMV.transpose(); 
		//m_currentPos= transposedMV * m_currentPos;
		//m_lookAt = transposedMV * m_lookAt;
		
		_tx.pushTransform();
			{
		ngl::Vec3 m_l, m_c;
		ngl::Vec3	m_cone_base, m_cross_axis_prod;
		ngl::Vec3 m_light_pos(m_currentPos.m_x, m_currentPos.m_y, m_currentPos.m_z);
		ngl::Real m_dot_angle_prod;


		m_cone_base.m_x = m_light_pos.m_x;
		m_cone_base.m_y = m_light_pos.m_y - 2.5f; //default height is 2.5f units
		m_cone_base.m_z = m_light_pos.m_z;


		//Direction vectors.
		//Light direction.
		m_l = m_lookAt - m_light_pos;
		m_l.normalize();
		//Cone orientation vector.
		m_c = m_cone_base - m_light_pos;	
		m_c.normalize();
  
		//Calculate the angle in radians between the two vectors.
		m_dot_angle_prod = (m_l.dot(m_c));
		//Calculate the axis of rotations between the two vectors.
		m_cross_axis_prod.set(m_l.cross(m_c));
		m_cross_axis_prod.normalize();
	
		//Aquire the actual angle, in radians.
		m_dot_angle_prod = acos(m_dot_angle_prod);
		//Convert the angle in degrees.
		m_dot_angle_prod = m_dot_angle_prod * (180.0f/M_PI);

		//Create the Euler rotation matrix, based on the above angle and rotation axis.
		ngl::Mat4 RotMat;
		RotMat.euler(m_dot_angle_prod, m_cross_axis_prod.m_x, m_cross_axis_prod.m_y, m_cross_axis_prod.m_z);
		
		//Get the current Model Matrix and scale the model.
		ngl::Mat4 OldMat = _tx.getCurrentTransform().getMatrix()	;
		//OldMat.translate(m_light_pos.m_x, m_light_pos.m_y, m_light_pos.m_z);
		RotMat.translate(m_light_pos.m_x, m_light_pos.m_y, m_light_pos.m_z);
		OldMat.scale(m_scaleX, m_scaleY, m_scaleZ);
		RotMat *=OldMat;
		
		//Push the matrix as transform matrix and use it to draw the cone.
		_trans.setMatrix(RotMat);  
		_tx.setCurrent(_trans);
		

				//M=_tx.getCurrAndGlobal().getMatrix();
				//MV =_tx.getCurrAndGlobal().getMatrix() * m_camera->getViewMatrix();
				//MVP= M*m_camera->getVPMatrix();
			 M=_tx.getCurrentTransform().getMatrix();	
		   MV =_tx.getCurrAndGlobal().getMatrix() * m_camera->getViewMatrix();	
		   MVP=  MV*m_camera->getProjectionMatrix();
				
				shader->setShaderParamFromMat4("MVP",MVP);
			
				// load these values to the shader as well
			shader->setShaderParam2f("gScreenSize", m_renderWidth, m_renderHeight);

			// Draw the cone
			prim->draw("DeferredCone");
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
