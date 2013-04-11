How to use it:
[UPDATED]

1. The renderer can be seen as a black box. All objects, lights and textures used in the scene can be configured
from data/initscene.ini file.
2. All the documentation needed is found in the file in order to configure it.
3. The SSAO is very strong on purpose in order to better show it.
4. In order to set it to normal ammounts modify the g_intensity parameter in shaders/SSAOFragment.glsl shader.

KNOWN ISSUE:
1. The directional light cannot be positioned due to the lack of the code supporting this feature.
It will always be located at 0,0,0 pointing down the Z-axis.
All the other lights work as expected.
In order to obtain good results all the parameters need to be tweaked.



//------------------------------------------------------------------------------------------------------------
[OLDER VERSION]
In GlWindow.cpp in the initializeGL() method use the following methods:

1. Load a deferred shader to be used for light calculation:
"g_renderer->loadDeferredShader(
								_shaderNumber,
								"ShaderName",
								"ShaderVertexName file location",
								"ShaderFragmentName file location"
								);"
First value for _shaderNumber must be 0. If it is not 0 the application will give an exception at runtime.("vector subscript out of range")


2. Add a light in scene:
"g_renderer->setLightInScene(
							_shaderNumber, 
							ngl::Vec3 _lightPostion,
							ngl::Colour _lightColor,
							ngl::Colour _lightSpecularcolor,
							_lightType,
							_lightVolumeRadius,
							_lightVolumeHeight,
							_lightRotationInDegrees
							);"
_lightType, can be ngl::SPOTLIGHT, ngl::POINTLIGHT, ngl::DIRECTIONALLIGHT.
_lightVolumeRadius, _lightVolumeHeight are ngl::Real. _lightVolumeHeight is taken into consideration only for ngl::SPOTLIGHT.
_lightRotationInDegrees is a ngl::Vec3. For example a -90 rotation on the X axis for a ngl::SPOTLIGHT will make the spot light look down.
!!!At the moment only rotation of the X axis is taken into consideration for ngl::SPOTLIGHT.

3. Add light attenuation for the above created light (works only for the previously created light):
"g_renderer->setLightAttenuation(
								_shaderNumber,
								_constantAttenuation,
								_linearAttenuation,
								_quadraticAttenuation
								);"
If no light attenuation is declared it is considered _constantAttenuation = 1.

4. Set Exponent and cutoff for previously created ngl::SPOTLIGHT:
"g_renderer->setSpotLightCutOffAndExponent(
											_shaderNumber,,
											_lightExponent,
											_lightCutOff
											);"
At the moment this method is required. If it not used for a spotlight both _lightExponent and _lightCutOff will be considered 0.0 making the light not appear.

5. Other useful info:
- Based on the orientation of the ngl::SPOTLIGHT it might be required to increase the _lightVolumeHeight in order for the cone to intersect
with the plane and geometry	if this is wanted.
- The spotlight doesn't require an AIM vector. It is automatically calculated based on the axis rotation. it will always point in the same direction
as the cone's base.
								