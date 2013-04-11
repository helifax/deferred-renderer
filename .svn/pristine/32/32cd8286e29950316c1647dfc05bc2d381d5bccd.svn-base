/// \file ConeLightVolume.h
/// \Creating the atributes and methods required for the Spotlight type light volumes
/// \It will store all the necesary information per spotlight, per shader
/// \author Octavian Mihai Vasilovici
/// \version 0.3
/// \date 28/03/2013 Updated to NCCA Coding standard
/// Revision History :
/// Initial Version 28/03/2013


#pragma once //Mostly for Visual Studio compiler
#ifndef _CONELIGHTVOLUME_H_
#define _CONELIGHTVOLUME_H_

#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>

class ConeLightVolume
{
public:
	/// @brief ctor for creating a Cone volume for the light.
	/// @brief Reguired parameters:
	/// @brief light position
	/// @brief light intensity
	/// @brief cone radius
	/// @brief cone height
	/// @brief cone orientation in degrees
	ConeLightVolume (
										ngl::Vec3 _lightPosition,
										ngl::Colour _lightIntensity,
										ngl::Real _coneRadius,
										ngl::Real _coneHeight,
										ngl::Vec3 _lightLookAt
										);
	/// @brief default dtor
	~ConeLightVolume(){;}

public:		
	
		/// @brief Returns the New Cone Orientation.
	ngl::Vec3 getLightLookAt() const;
	/// @brief Returns the Radius and Height.
	ngl::Vec2 getRadiusAndHeigth() const;

private: //attributes
		/// @brief Vector containing the Radius and Height.
	ngl::Vec2 m_coneParams;
		/// @brief Vector containing the lookAt location.
	ngl::Vec3 m_lightLookAt;

protected: //protected attributes.

private: //methods
		/// @brief Returns Radius and Height. Default is a scale of 1.0f.
	ngl::Vec2 calcConeParamsDefault ();
		/// @brief Returns custom Radius and Height
	ngl::Vec2 calcConeParamsCustom (
																	ngl::Real _coneRadius,
																	ngl::Real _coneHeight
																	);
};

#endif //_CONELIGHTVOLUME_H_
