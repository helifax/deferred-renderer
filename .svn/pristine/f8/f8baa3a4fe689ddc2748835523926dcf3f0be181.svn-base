/// \file Spotlight.h
/// \Class that contains all the attributes specific for a Spotlight light type
/// \author Octavian Mihai Vasilovici
/// \version 0.4
/// \date 28/03/2013 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 28/03/2013
/// \todo

#pragma once //Mostly for Visual Studio compiler
#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_


#include <ngl/Colour.h>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>

class Spotlight
{

public:
	/// @brief ctor for creating a new spotlight.
	/// @brief Reguired parameters:
	/// @brief light position
	/// @brief light diffuse color
	/// @brief light specular color
	Spotlight(
						ngl::Vec3 &_lightPosition,
						ngl::Colour &_diffuseColor,
						ngl::Colour &_specularColor
						);
	
	/// @brief default dtor
	~Spotlight();

	/// @brief Sets a new diffuseColor for the spotlight
	void setDiffuseColour(
												ngl::Colour &_diffuseColor
												);

	/// @brief Sets a new specularColor for the spotlight
	void setSpecularColour(
												ngl::Colour &_specularColor
												);

	/// @brief Sets a new position for the spotlight
	void setPosition(
									ngl::Vec3 &_lightPosition
									);

	/// @brief Sets the light direction for the spotlight
	void setLightDirection(
												ngl::Vec3 &_lightDirection
												);

	/// @brief Returns the diffuse Color of the spotlight
	ngl::Colour getDiffuseColour() const;

	/// @brief Returns the specular Color of the spotlight
	ngl::Colour getSpecularColour() const;

	/// @brief Returns the position of the spotlight
	ngl::Vec4 getPosition() const;

	/// @brief Returns the light direction of the spotlight
	ngl::Vec3 getLightDirection() const;

	/// @brief Sets the light exponent value
	void setLightExponent(
												const float & _exponent
												);

	/// @brief Sets the light cutoff value in degrees. Maximum value is 90.0f
	void setLightCutOff(
											const float & _cutoff
											);

	/// @brief loads the light information to the shader.
	/// @brief Expects the uniform value
	void loadToShader(
										std::string _uniform
										);

	/// @brief Sets the light attenuation factors.
	/// @brief Expected:
	/// @brief Constant attenuation
	/// @brief Linear attenuation
	/// @brief Quadratic attenuation
	void setAttenuation (
											ngl::Real _constant,
											ngl::Real _linear,
											ngl::Real _quadratic
											);

protected: //attributes
	ngl::Vec3 m_position;

	ngl::Vec3 m_lightDirection;

	ngl::Colour m_diffuseColour;

	ngl::Colour m_specularColor;

	// Angular attenuation exponent
	float m_exponent;
	// Cutoff angle (between 0 and 90)
	float m_cuttoff;
	// Light attenuation parameters
	ngl::Real m_constantAtten;
	ngl::Real m_linearAtten;
	ngl::Real m_quadraticAtten;

private: //atributes

};

#endif //_SPOTLIGHT_H_
