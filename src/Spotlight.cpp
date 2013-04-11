#include <ngl/ShaderLib.h>
#include "SpotLight.h"

Spotlight::Spotlight(
						ngl::Vec3 &_lightPosition,
						ngl::Colour &_diffuseColor,
						ngl::Colour &_specularColor
						)
{
	m_position = _lightPosition;
	setDiffuseColour(_diffuseColor);
	setSpecularColour(_specularColor);
}
//----------------------------------------------------------------------------------------------------------------------

// Returns light's diffuse color
ngl::Colour Spotlight::getDiffuseColour() const
{
	return m_diffuseColour;
}
//----------------------------------------------------------------------------------------------------------------------

// Returns light's specular color
ngl::Colour Spotlight::getSpecularColour() const
{
	return m_specularColor;
}
//----------------------------------------------------------------------------------------------------------------------

// Returns light position
ngl::Vec4 Spotlight::getPosition() const
{
	return ngl::Vec4(m_position.m_x, m_position.m_y, m_position.m_z, float(1.0));
}
//----------------------------------------------------------------------------------------------------------------------

// Returns light direction
ngl::Vec3 Spotlight::getLightDirection() const
{
	return m_lightDirection;
}
//----------------------------------------------------------------------------------------------------------------------

// Set light's color
void Spotlight::setDiffuseColour(
																ngl::Colour &_diffuseColor
																)
{
	// Calculate the new RGB values for intensity 1.0.
	_diffuseColor.m_r = _diffuseColor.m_r /255;
	_diffuseColor.m_g = _diffuseColor.m_g /255;
	_diffuseColor.m_b = _diffuseColor.m_b /255;

	// Apply the light intensity.
	_diffuseColor.m_r *= _diffuseColor.m_a;
	_diffuseColor.m_g *= _diffuseColor.m_a;
	_diffuseColor.m_b *= _diffuseColor.m_a;
	
	m_diffuseColour = _diffuseColor;
}
//----------------------------------------------------------------------------------------------------------------------

// Set light's specular color
void Spotlight::setSpecularColour(
																	ngl::Colour &_specularColor
																	)
{
	// Calculate the new RGB values for intensity 1.0.
	_specularColor.m_r = _specularColor.m_r /255;
	_specularColor.m_g = _specularColor.m_g /255;
	_specularColor.m_b = _specularColor.m_b /255;

	// Apply the light intensity.
	_specularColor.m_r *= _specularColor.m_a; 
	_specularColor.m_g *= _specularColor.m_a;
	_specularColor.m_b *= _specularColor.m_a;

	m_specularColor = _specularColor;
}
//----------------------------------------------------------------------------------------------------------------------

// Set light position
void Spotlight::setPosition(
														ngl::Vec3 &_lightPosition
														)
{
	m_position = _lightPosition;
}
//----------------------------------------------------------------------------------------------------------------------

// Set light direction
void Spotlight::setLightDirection(
																	ngl::Vec3 &_lightDirection
																	)
{
	m_lightDirection = _lightDirection;
}
//----------------------------------------------------------------------------------------------------------------------

// Set light exponent
void Spotlight::setLightExponent(
																const float & _exponent
																)
{
	m_exponent = _exponent;
}
//----------------------------------------------------------------------------------------------------------------------

// Set light cuttoff
void Spotlight::setLightCutOff(
															const float & _cutoff
															)
{
	m_cuttoff = _cutoff;
}
//----------------------------------------------------------------------------------------------------------------------

// Load to shader the uniforms
void Spotlight::loadToShader(
														std::string _uniform
														)
{
	ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  shader->setShaderParam4f(_uniform+".position",m_position.m_x,m_position.m_y, m_position.m_z, float(1));
    shader->setShaderParam3f(_uniform+".direction",m_lightDirection.m_x,m_lightDirection.m_y,m_lightDirection.m_z);
    shader->setShaderParam4f(_uniform+".diffuse",m_diffuseColour.m_r,m_diffuseColour.m_g,m_diffuseColour.m_b,m_diffuseColour.m_a);
    shader->setShaderParam4f(_uniform+".specular",m_specularColor.m_r,m_specularColor.m_g,m_specularColor.m_b,m_specularColor.m_a);
    shader->setShaderParam1f(_uniform+".cutoff",m_cuttoff);
    shader->setShaderParam1f(_uniform+".exponent",m_exponent);
    shader->setShaderParam1f(_uniform+".constantAttenuation",m_constantAtten);
    shader->setShaderParam1f(_uniform+".linearAttenuation",m_linearAtten);
    shader->setShaderParam1f(_uniform+".quadraticAttenuation",m_quadraticAtten);
}
//----------------------------------------------------------------------------------------------------------------------

// set the constant, linear and quadratic light attenuations factors for the light
void Spotlight::setAttenuation (
																ngl::Real _constant,
																ngl::Real _linear,
																ngl::Real _quadratic
																)
{
	m_constantAtten = _constant;
	m_linearAtten = _linear;
	m_quadraticAtten = _quadratic;
}
//----------------------------------------------------------------------------------------------------------------------
