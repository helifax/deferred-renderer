/// \file SphereLightVolume.h
/// \Creating the atributes and methods required for the pointlight type light volumes
/// \author Octavian Mihai Vasilovici
/// \version 0.5
/// \date 28/03/2013 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 28/03/2013
/// \todo

#pragma once //Mostly for Visual Studio compiler
#ifndef _SPHERELIGHTVOLUME_H_
#define _SPHERELIGHTVOLUME_H_

#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

class SphereLightVolume
{
public:

	/// @brief ctor for creating a Sphere volume for the light.
	/// @brief Reguired parameters:
	/// @brief light intensity
	/// @brief cone radius
	SphereLightVolume(
										ngl::Colour _lightIntensity,
										ngl::Real _sphereRadius
										);
	/// @brief default dtor
	~SphereLightVolume();

	/// @brief Returns the sphere radius
	ngl::Real getSphereRadius() const;

	
	/// @brief Sets the custom cutoff
	void setLightCutOff(
																	ngl::Real _cutoff_factor
																	);

	/// @brief Returns custom cutoff
	ngl::Real getLightCutOff();

	/// @brief Sets custom Light entity Radius
	void setLightRadius(
																	ngl::Real _LightRadius
																	);

	/// @brief Get custom Light entity Radius
	ngl::Real getLightRadius();

private: //attributes

	/// @brief Real number containing the sphere radius.
	ngl::Real m_sphereRadius;
	/// @brief Real number containing the Light cutoff.
	ngl::Real m_cuttoff;
	/// @brief Real number containing the Light radius.
	ngl::Real m_lightRadius;

private: //methods

	/// @brief Returns Radius based on the light Intensity.
	ngl::Real calcSphereRadiusDefault(
																		ngl::Colour _lightIntensity
																		);
	/// @brief Returns custom Radius
	ngl::Real calcSphereRadiusCustom(
																	ngl::Real _sphereRadius
																	);
};

#endif //_SPHERELIGHTVOLUME_H_
