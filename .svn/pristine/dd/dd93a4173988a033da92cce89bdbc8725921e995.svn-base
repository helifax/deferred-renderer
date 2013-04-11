#include <ngl/VAOPrimitives.h>
#include <boost/lexical_cast.hpp>
#include "DeferredShading.h"
#include "ConeLightVolume.h"
#include "SphereLightVolume.h"
#include "SpotLight.h"


void DeferredShading::setLightInScene(
																			GLint _shaderNumber,
																			ngl::Vec3 _lightPosition,
																			ngl::Colour _diffuseColor,
																			ngl::Colour _specularColor,
																			ngl::LIGHTMODES _lightType,
																			ngl::Real _Radius,
																			ngl::Real _Height,
																			ngl::Vec3 _lightLookAt)
{
		if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
		{
		// Set our shader as active
		m_deferredShader[_shaderNumber]->setShaderAsActive();

		if (_lightType == ngl::SPOTLIGHT)
			{
				//Seting up our light
				//increment the size of the vector to keep the new size. store the value in a intermediate variable _lightIndex.
				int _lightIndex = m_deferredShader[_shaderNumber]->m_spotlight.size();

				//resize the vector
				m_deferredShader[_shaderNumber]->m_spotlight.resize(_lightIndex + 1);
				m_deferredShader[_shaderNumber]->m_lightType.push_back(_lightType);

				m_deferredShader[_shaderNumber]->m_spotlight[_lightIndex] = new Spotlight(_lightPosition, _diffuseColor, _specularColor);
				
				// Calculating the direction vector for the light. Is basically the oposite of where the light exists.
				ngl::Vec3 lightDir = _lightLookAt - _lightPosition;
				m_deferredShader[_shaderNumber]->m_spotlight[_lightIndex]->setLightDirection(lightDir);

				//Try to aproximate the default values for exponent and cutoff
				
				ngl::Real _exponent;
				ngl::Real _cutoff = 90.0f;

				//Scale on Y only
				if (( _Height > 1.0) && (_Radius == 1.0))
				{
					_exponent = 45.0 * _Height * _Height * 2.0;
				}
				// If uniform scale
				else if ( _Height == _Radius)
				{
					_exponent = 45.0 + (45.0 / (_Height / 2));
				}
				// If Y scale is bigger 
				else if ( _Height > _Radius)
				{
					_exponent = 45.0 * _Height / 2;
				}
				// If XZ scale is bigger
				else if ( _Height < _Radius)
				{
					_exponent = 45.0 / _Radius * _Height;
				}

				setSpotLightCutOffAndExponent(_shaderNumber, _exponent, _cutoff);
				
				
				// Set the default attenuation.
				m_deferredShader[_shaderNumber]->m_spotlight[_lightIndex]->setAttenuation(0.0f, 0.0f, 0.05f);

				// Push into the vector the new cone Volume information
				m_deferredShader[_shaderNumber]->m_coneVolume.push_back(new ConeLightVolume(_lightPosition, _diffuseColor, _Radius, _Height, _lightLookAt));
			}
		else if (_lightType == ngl::POINTLIGHT)
			{

				//Seting up our light
				//increment the size of the vector to keep the new size. store the value in a intermediate variable _lightIndex.
				int _lightIndex = m_deferredShader[_shaderNumber]->m_light.size();

				//resize the vector
				m_deferredShader[_shaderNumber]->m_light.resize(_lightIndex + 1);
				m_deferredShader[_shaderNumber]->m_light_original.resize(_lightIndex + 1);
		
				// Calculate the new RGB values for intensity 1.0.
				_diffuseColor.m_r = _diffuseColor.m_r /255;
				_diffuseColor.m_g = _diffuseColor.m_g /255;
				_diffuseColor.m_b = _diffuseColor.m_b /255;

				_specularColor.m_r = _specularColor.m_r /255;
				_specularColor.m_g = _specularColor.m_g /255;
				_specularColor.m_b = _specularColor.m_b /255;

				// Apply the light intensity.
				_diffuseColor.m_r *= _diffuseColor.m_a;
				_diffuseColor.m_g *= _diffuseColor.m_a;
				_diffuseColor.m_b *= _diffuseColor.m_a;

				_specularColor.m_r *= _specularColor.m_a; 
				_specularColor.m_g *= _specularColor.m_a;
				_specularColor.m_b *= _specularColor.m_a;
				
				//create new light
				m_deferredShader[_shaderNumber]->m_light[_lightIndex] = new ngl::Light(_lightPosition ,_diffuseColor, _specularColor ,_lightType );
				m_deferredShader[_shaderNumber]->m_light_original[_lightIndex] = new ngl::Light(_lightPosition ,_diffuseColor, _specularColor ,_lightType );
				m_deferredShader[_shaderNumber]->m_lightType.push_back(_lightType);

				ngl::Mat4 iv=m_camera->getViewMatrix();
				iv.transpose();
				m_deferredShader[_shaderNumber]->m_light[_lightIndex]->setTransform(iv);

				// Push into the vector the new cone Volume information
				m_deferredShader[_shaderNumber]->m_sphereVolume.push_back(new SphereLightVolume(_diffuseColor, _Radius));
			}
		else if (_lightType == ngl::DIRECTIONALLIGHT)
		{
			// Directional Light
			//Seting up our light
			//increment the size of the vector to keep the new size. store the value in a intermediate variable _lightIndex.
			int _lightIndex = m_deferredShader[_shaderNumber]->m_light.size();
			//resize the vector
			m_deferredShader[_shaderNumber]->m_light.resize(_lightIndex + 1);

			
			// Calculate the new RGB values for intensity 1.0.
			_diffuseColor.m_r = _diffuseColor.m_r /255;
			_diffuseColor.m_g = _diffuseColor.m_g /255;
			_diffuseColor.m_b = _diffuseColor.m_b /255;

			_specularColor.m_r = _specularColor.m_r /255;
			_specularColor.m_g = _specularColor.m_g /255;
			_specularColor.m_b = _specularColor.m_b /255;

			// Apply the light intensity.
			_diffuseColor.m_r *= _diffuseColor.m_a;
			_diffuseColor.m_g *= _diffuseColor.m_a;
			_diffuseColor.m_b *= _diffuseColor.m_a;

			_specularColor.m_r *= _specularColor.m_a; 
			_specularColor.m_g *= _specularColor.m_a;
			_specularColor.m_b *= _specularColor.m_a;
			
			//create new light
			m_deferredShader[_shaderNumber]->m_light[_lightIndex] = new ngl::Light(_lightPosition ,_diffuseColor, _specularColor ,_lightType );
			m_deferredShader[_shaderNumber]->m_lightType.push_back(_lightType);

			// Set the default attenuation.
			m_deferredShader[_shaderNumber]->m_light[_lightIndex]->setAttenuation(0, 0, 0.05);

			ngl::Mat4 iv=m_camera->getViewMatrix();
			iv.transpose();
			m_deferredShader[_shaderNumber]->m_light[_lightIndex]->setTransform(iv);
			}
		}
		else
		{
			std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Nothing was done! \n";
		}
}
//----------------------------------------------------------------------------------------------------------------------

// Sets Constant, Linear and Quadratic light attenuations for the light.
void DeferredShading::setLightAttenuation (
																					GLint _shaderNumber,
																					ngl::Real _constant,
																					ngl::Real _linear,
																					ngl::Real _quadratic
																					)
{
	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		m_deferredShader[_shaderNumber]->setShaderAsActive();
		// get the last light that was added.
		GLint m_lightIndex = getLightIndex(_shaderNumber);
		// get the light type
		ngl::LIGHTMODES m_lightType = getLightType(_shaderNumber, m_lightIndex);

		if (m_lightIndex >= 0)
		{
			if ((m_lightType == ngl::POINTLIGHT) ||(m_lightType == ngl::DIRECTIONALLIGHT))
			{
				m_deferredShader[_shaderNumber]->m_light[m_lightIndex]->setAttenuation(_constant, _linear, _quadratic);
			}
			else
				if (m_lightType == ngl::SPOTLIGHT)
				{
					m_deferredShader[_shaderNumber]->m_spotlight[m_lightIndex]->setAttenuation(_constant, _linear, _quadratic);
				}
		}
	}
	else
	{
		std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot use an unknown shader! \n";
	}
}
//----------------------------------------------------------------------------------------------------------------------

void DeferredShading::setPointLightAttenuation (
														GLint _shaderNumber,
														ngl::Real _LightRadius,
														ngl::Real _lightCutoff
														)
{
	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		m_deferredShader[_shaderNumber]->setShaderAsActive();
		GLint m_lightIndex = getLightIndex(_shaderNumber);
		if (m_lightIndex >= 0)
		{
			m_deferredShader[_shaderNumber]->m_sphereVolume[m_lightIndex]->setLightRadius(_LightRadius);
			m_deferredShader[_shaderNumber]->m_sphereVolume[m_lightIndex]->setLightCutOff(_lightCutoff);
		}
	}
	else
	{
		std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot use an unknown shader! \n";
	}
}
//----------------------------------------------------------------------------------------------------------------------

	// Sets the CutOff and Exponent for a spot light.
void DeferredShading::setSpotLightCutOffAndExponent(
																										GLint _shaderNumber,
																										ngl::Real _exponent,
																										ngl::Real _cutOffAngle
																										)
{
	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		m_deferredShader[_shaderNumber]->setShaderAsActive();
		GLint m_lightIndex = getLightIndex(_shaderNumber);
		if (m_lightIndex >= 0)
		{
			m_deferredShader[_shaderNumber]->m_spotlight[m_lightIndex]->setLightExponent(_exponent);
			m_deferredShader[_shaderNumber]->m_spotlight[m_lightIndex]->setLightCutOff(_cutOffAngle);
		}
	}
	else
	{
		std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot use an unknown shader! \n";
	}
}
//----------------------------------------------------------------------------------------------------------------------

GLint DeferredShading::getLightIndex(
																		GLint &_shaderNumber
																		) const
{
	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		m_deferredShader[_shaderNumber]->setShaderAsActive();
		// Get the ammount of lights added to this shader
		if ((m_deferredShader[_shaderNumber]->m_light.size() - 1) != -1)
		{
			return  (m_deferredShader[_shaderNumber]->m_light.size() -1);
		}
		else
		{
			return (m_deferredShader[_shaderNumber]->m_spotlight.size() -1);
		}
	}
	else
	{
		std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot use an unknown shader! \n";
	}
}
//----------------------------------------------------------------------------------------------------------------------

ngl::LIGHTMODES DeferredShading::getLightType(
																							GLint &_shaderNumber,
																							GLint &_lightIndex
																							) const
{
	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		m_deferredShader[_shaderNumber]->setShaderAsActive();
		// Return the light Type
		return m_deferredShader[_shaderNumber]->m_lightType[_lightIndex];
	}
	else
	{
		std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot use an unknown shader! \n";
	}
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief Get the Color of the light attached to _shaderNumber.
	ngl::Colour DeferredShading::getLightColor(GLint &_shaderNumber,
														GLint &_lightIndex
														) const
	{
		return m_deferredShader[_shaderNumber]->m_light[_lightIndex]->getColour();
	}
//----------------------------------------------------------------------------------------------------------------------
	/// @brief Get the Color of the light attached to _shaderNumber.
	ngl::Colour DeferredShading::getOriginalLightColor(GLint &_shaderNumber,
																											GLint &_lightIndex
																										) const
	{
		return m_deferredShader[_shaderNumber]->m_light_original[_lightIndex]->getColour();
	}
//----------------------------------------------------------------------------------------------------------------------
	/// @brief Set the Color of the light attached to _shaderNumber.
	void DeferredShading::setLightColor(GLint &_shaderNumber,
														GLint &_lightIndex,
														ngl::Colour &_color
														)
	{
		m_deferredShader[_shaderNumber]->m_light[_lightIndex]->setColour(_color);
	}
	//----------------------------------------------------------------------------------------------------------------------


// Usefull for moving the light position around with the camera.
void DeferredShading::addToLightPosition(
																				GLint _shaderNumber,
																				GLint _lightIndex,
																				float _valueX, 
																				float _valueY,
																				float _valueZ
																				)
{
	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		m_deferredShader[_shaderNumber]->setShaderAsActive();
		// Get the ammount of lights added to this shader
		{
			ngl::Vec4 m_currPos = m_deferredShader[_shaderNumber]->m_light[_lightIndex]->getPos();
				float m_posX = m_currPos.m_x;
				float m_posY = m_currPos.m_y;
				float m_posZ = m_currPos.m_z;
			if (_valueX != 0.0f)
			{
				m_posX += _valueX;
		}
			if (_valueY != 0.0f)
			{
				m_posY += _valueY;
		}
			if (_valueZ != 0.0f)
			{
				m_posZ += _valueZ;
			}

			if((_valueX != 0.0f) ||(_valueY != 0.0f) || (_valueZ != 0.0f))
			{
				ngl::Vec3 m_newPos (m_posX, m_posY, m_posZ);
				m_deferredShader[_shaderNumber]->m_light[_lightIndex]->setPosition(m_newPos);
			}
			else
			{
				std::cout<<"All input values were zero. Nothing was done. \n";
			}
		}
	}
	else
	{
		std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot use an unknown shader! \n";
	}
}
//----------------------------------------------------------------------------------------------------------------------

// Usefull for moving the light position around with the camera.
void DeferredShading::subFromLightPosition(
																					GLint _shaderNumber,
																					GLint _lightIndex,
																					float _valueX,
																					float _valueY,
																					float _valueZ
																					)
{
	if((_shaderNumber <= m_shaderNumber) && (_shaderNumber >= 0))
	{
		m_deferredShader[_shaderNumber]->setShaderAsActive();
		ngl::Vec4 m_currPos = m_deferredShader[_shaderNumber]->m_light[_lightIndex]->getPos();
		float m_posX = m_currPos.m_x;
		float m_posY = m_currPos.m_y;
		float m_posZ = m_currPos.m_z;
		if (_valueX != 0.0f)
			{
				m_posX -= _valueX;
			}
		if (_valueY != 0.0f)
			{
				m_posY -= _valueY;
			}
		if (_valueZ != 0.0f)
			{
				m_posZ -= _valueZ;
			}

		if((_valueX != 0.0f) ||(_valueY != 0.0f) || (_valueZ != 0.0f))
			{
				ngl::Vec3 m_newPos (m_posX, m_posY, m_posZ);
				m_deferredShader[_shaderNumber]->m_light[_lightIndex]->setPosition(m_newPos);
			}
		else
			{
			std::cout<<"All input values were zero. Nothing was done. \n";
			}
		}
	else
	{
		std::cout<<"The shader with number "<< _shaderNumber<<" doesn't exist. Cannot use an unknown shader! \n";
	}
}
//----------------------------------------------------------------------------------------------------------------------
