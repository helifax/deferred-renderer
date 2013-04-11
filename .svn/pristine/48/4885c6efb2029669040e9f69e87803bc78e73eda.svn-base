// declare our max() function based on OS and compiler
#ifdef _MSC_VER
	#define MAX max // For Visual Studio Compilers
#else
	#define MAX fmax // For Linux and possible MacOS
#endif

#include "SphereLightVolume.h"

SphereLightVolume::SphereLightVolume(
																		ngl::Colour _lightIntensity,
																		ngl::Real _sphereRadius
																		)
{
	if(_sphereRadius <= 0.0)
	{
	// Calculates the sphere radius based on the maximum value from the light intensity
		m_sphereRadius = calcSphereRadiusDefault(_lightIntensity.m_a);
	}
	else
	{
		// Returns the custom _sphereRadius
		m_sphereRadius = calcSphereRadiusCustom(_sphereRadius);
	}

	//Set initial values for lightRadius and Cutoff
	ngl::Real _lightRadius = getSphereRadius();
	setLightRadius(_lightRadius /2.0f);
	setLightCutOff(_lightRadius /10.0f);
	
		

}
//----------------------------------------------------------------------------------------------------------------------

// Returns the default radius calculated based on the light intensity
ngl::Real SphereLightVolume::calcSphereRadiusDefault(
																											ngl::Colour _lightIntensity
																										)
{
	// For light intensity of 1.0, 1.0, 1.0) the default radius is 5.0. We calculate the new radius based on this and the intensity.
		
	return m_sphereRadius = (5.0f * _lightIntensity.m_a);
}
//----------------------------------------------------------------------------------------------------------------------

// Assigns the custom radius to the sphere
ngl::Real SphereLightVolume::calcSphereRadiusCustom(
																										ngl::Real _sphereRadius
																										)
{
	return m_sphereRadius =_sphereRadius;
}
//----------------------------------------------------------------------------------------------------------------------

// Returns the sphere radius
ngl::Real SphereLightVolume::getSphereRadius() const
{
	return m_sphereRadius;
}
//----------------------------------------------------------------------------------------------------------------------


/// @brief Sets the custom cutoff
	void SphereLightVolume::setLightCutOff(
																	ngl::Real _cutoff_factor
																	)
	{
		m_cuttoff = _cutoff_factor;
	}
	//----------------------------------------------------------------------------------------------------------------------

	/// @brief Returns custom cutoff
	ngl::Real SphereLightVolume::getLightCutOff()
	{
		return m_cuttoff;
	}
	//----------------------------------------------------------------------------------------------------------------------

	/// @brief Sets custom Light entity Radius
	void SphereLightVolume::setLightRadius(
																	ngl::Real _LightRadius
																	)
	{
		m_lightRadius = _LightRadius;
	}
	//----------------------------------------------------------------------------------------------------------------------

	/// @brief Get custom Light entity Radius
	ngl::Real SphereLightVolume::getLightRadius()
	{
		return m_lightRadius;
	}
	//----------------------------------------------------------------------------------------------------------------------