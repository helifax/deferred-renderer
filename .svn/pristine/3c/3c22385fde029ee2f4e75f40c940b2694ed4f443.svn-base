// declare our max() function based on OS and compiler
#ifdef _MSC_VER
	#define MAX max // For Visual Studio Compilers
#else
	#define MAX fmax // For Linux and possible MacOS
#endif

#include <ngl/TransformStack.h>
#include "ConeLightVolume.h"

// Reguired parameters:
// light position
// light intensity
// cone radius
// cone height
// cone orientation in degrees
ConeLightVolume::ConeLightVolume (
																	ngl::Vec3 _lightPosition,
																	ngl::Colour _lightIntensity,
																	ngl::Real _coneRadius,
																	ngl::Real _coneHeight,
																	ngl::Vec3 _lightLookAt
																	)
{
	// Create Cone Volume for spot lights.
	if ((_coneRadius <= 0.0f || _coneHeight <= 0.0f))
	{
		//If a value of 0 was specified for cone radius or height we return default radius and height
		m_coneParams = calcConeParamsDefault();
		//store the lookAt position for later usage
		m_lightLookAt = _lightLookAt;
	}
	else
	{
		// if a custom radius and height was given we calculate the parameters.
		m_coneParams = calcConeParamsCustom(_coneRadius, _coneHeight);
		//store the cone orientation for later usage
		m_lightLookAt = _lightLookAt;
	}
}
//----------------------------------------------------------------------------------------------------------------------

// Calculates and returns a Vec2( _Radius, _Heigth) base on the light intensity
ngl::Vec2 ConeLightVolume::calcConeParamsDefault ()
{
	
		// We return the default scale parameters of 1.0f
		return m_coneParams = ngl::Vec2(1.0f, 1.0f);
}
//----------------------------------------------------------------------------------------------------------------------

// Calculates and returns a Vec2( _Radius, _Heigth) based on the specified _coneHeigth and _coneRadius
ngl::Vec2 ConeLightVolume::calcConeParamsCustom (
																									ngl::Real _coneRadius,
																									ngl::Real _coneHeight
																									)
{		//we store these values as a vector 2 to be later used
		return m_coneParams = ngl::Vec2(_coneRadius, _coneHeight);
}
//----------------------------------------------------------------------------------------------------------------------

// returns the vec3 containing lookAt position
ngl::Vec3 ConeLightVolume::getLightLookAt() const
{
	return m_lightLookAt;
}
//----------------------------------------------------------------------------------------------------------------------

// returns the vec2 containing _coneRadius and _coneHeight
ngl::Vec2 ConeLightVolume::getRadiusAndHeigth() const
{
	return m_coneParams;
}
//----------------------------------------------------------------------------------------------------------------------

