#include "DeferredShading.h"



/// @brief Load a specified OBJ file.
	void DeferredShading::loadModel(
																	GLint _shaderNumber,		
																	const std::string _modelName,
																	ngl::Material _material,
																	bool _useMaterial
																	)
	{
		if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
		{
			// Set our shader as active
			m_deferredShader[_shaderNumber]->setShaderAsActive();

			int _modelIndex = m_models.size();
			//resize the vector for models
			m_models.resize(_modelIndex+1);
			//resize the vector for model materials.
			m_model_material.resize(m_model_material.size() + 1);
			//resize the vector for position, scale and rotation
			m_modelPosition.resize(m_modelPosition.size() + 1);
			m_modelScale.resize(m_modelScale.size() + 1);
			m_modelRotation.resize(m_modelRotation.size() + 1);
			
			// Load the model
			m_models[_modelIndex] = new ModelLoader();
			m_models[_modelIndex]->loadMesh(_modelName, _shaderNumber);
			
			if (m_models[_modelIndex]->m_meshIsLoaded == true)
			{
				m_model_material[_modelIndex] = _material;
				m_model_material_use.resize(m_model_material_use.size()+1);
				m_model_material_use[_modelIndex] = _useMaterial;

				// store the number of models attached to the shader.
				m_modelIndex = _modelIndex;

				// Set implicit values for position, rotation, scale
				m_modelPosition[_modelIndex] = ngl::Vec3(0);
				m_modelScale[_modelIndex] = ngl::Vec3(1);
				m_modelRotation[_modelIndex] = ngl::Vec3(0);
			}
			else
			{
				// we decrement the size, as model was not loaded
				m_models.resize(m_models.size() -1 );
			}
		}
		else
		{
			std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Nothing was done! \n";
		}
	}

	/// @brief Load specified textures. Order is: TextureMap, NormalMap, SpecularMap.
	void DeferredShading::loadTextures(
																		GLint _modelIndex,
																		const int _numberOfTextures,
																		const std::string _textureName1,
																		const std::string _textureName2,
																		const std::string _textureName3
																		)
	{
			// Load the textures to the selected model.The number is decremented by 1 since the textere 0 is the 1st one.
			m_models[_modelIndex]->loadTexture((_numberOfTextures-1), _textureName1, _textureName2, _textureName3);
	}

	/// @brief Draw specified model. 
	void DeferredShading::drawModel(
																	ngl::TransformStack &_tx
                                   )
	{
		int _modelIndex = m_models.size() - 1;
		
		
		// draw all the models.
		while(_modelIndex>=0)
		{
			GLint _shader = m_models[_modelIndex]->getShaderNumber();
			if((_shader <= m_shaderNumber) && (_shader >= 0))
			{
				// Set our shader as active
				m_deferredShader[_shader]->setShaderAsActive();

				// Apply position, scale and rotation
				_tx.setPosition( getModelPosition(_modelIndex) );
				_tx.setScale( getModelScale(_modelIndex) );
				_tx.setRotation( getModelRotation(_modelIndex) );
			
				// Load the matrixes to shader
				ngl::ShaderLib *shader=ngl::ShaderLib::instance();
				ngl::Mat4 MV;
				ngl::Mat4 MVP;
				ngl::Mat3 normalMatrix;
				ngl::Mat4 M;
				M=_tx.getCurrentTransform().getMatrix();
				MV=  _tx.getCurrAndGlobal().getMatrix()*m_camera->getViewMatrix();
				MVP=  MV*m_camera->getProjectionMatrix();
				normalMatrix=MV;
				normalMatrix.inverse();
				shader->setShaderParamFromMat4("MV",MV);
				shader->setShaderParamFromMat4("MVP",MVP);
				shader->setShaderParamFromMat3("normalMatrix",normalMatrix);

					if (m_model_material_use[_modelIndex] == true)
					{
						ngl::ShaderLib *shader=ngl::ShaderLib::instance();
						//Apply material for each model.
						m_model_material[_modelIndex].loadToShader("material");
						// we add the material information
						shader->setShaderParam1i("use_material", 1);
					}
					else
					{
						// we dont add the material information
						shader->setShaderParam1i("use_material", 0);
					}
					// Draw each model.
					m_models[_modelIndex]->DrawMesh();
					_modelIndex--;
			}
		}
	}

	/// @brief Returns the number of loaded models for current shader. 
	GLint DeferredShading::getLoadedModels()
	{
		return m_models.size() - 1;
	}

	/// @brief Sets position, scale, rotation for current model
	void DeferredShading::setModelPosScaleRot(
																			GLint _modelIndex,
																			ngl::Vec3 _position,									
																			ngl::Vec3 _scale,
																			ngl::Vec3 _rotation
																			)
	{
			if(_modelIndex >= 0)
			{
				m_modelPosition[_modelIndex] = _position;
				if (_scale == ngl::Vec3(0,0,0))
				{
					_scale = ngl::Vec3(1,1,1);
				}
				m_modelScale[_modelIndex] = _scale;
				m_modelRotation[_modelIndex]= _rotation;
			}
		}
	
	/// @brief Gets position for current model
	ngl::Vec3 DeferredShading::getModelPosition(
																				GLint _modelIndex
																				)
	{
		return m_modelPosition[_modelIndex];
	}

	/// @brief Gets scale for current model
	ngl::Vec3 DeferredShading::getModelScale(
																			GLint _modelIndex
																			)
	{
		return m_modelScale[_modelIndex];
	}

	/// @brief Gets rotation for current model
	ngl::Vec3 DeferredShading::getModelRotation(
																				GLint _modelIndex
																				)
	{
		return m_modelRotation[_modelIndex];
	}