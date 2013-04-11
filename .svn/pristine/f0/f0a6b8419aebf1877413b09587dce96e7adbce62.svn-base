#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <boost/tokenizer.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "DeferredShading.h"

// create a typecast to tokenizer as it's quicker to wrie than the whole 
// line
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

void DeferredShading::loadFromInitFile()
{
	std::fstream fileIn;
  fileIn.open("data/initscene.ini",std::ios::in);

	// this is the line we wish to parse
  std::string lineBuffer;
  // say which separators should be used in this
  // case Spaces, Tabs and return \ new line
  boost::char_separator<char> sep(" \t\r\n)  (");

	while(!fileIn.eof())
  {
    getline(fileIn,lineBuffer,'\n');
		if (lineBuffer == "[Shaders]")
		{
			getline(fileIn,lineBuffer,'\n');
			while (((lineBuffer != "[Lights]") && (lineBuffer != "[Models]")) && !fileIn.eof())
			{
				// now tokenize the line
				tokenizer tokens(lineBuffer, sep);
				// and get the first token
				tokenizer::iterator  firstWord = tokens.begin();

				if(*firstWord == "Shader")
				{
					*firstWord++;	
				
					GLint _shaderNumber = boost::lexical_cast<GLint>(*firstWord++);
					std::string _typeString = boost::lexical_cast<std::string>(*firstWord++);
					SHADERTYPE _type;
					if (_typeString == "LIGHT")
					{
						_type = LIGHT;
					}
					else if (_typeString == "MESH")
					{
						_type = MESH;
					}
					else if (_typeString == "GEOMETRY")
					{
						_type = GEOMETRY;
					}
					std::string _shaderProgramName = boost::lexical_cast<std::string>(*firstWord++);
					std::string _vertexShaderFileName = boost::lexical_cast<std::string>(*firstWord++);
					std::string _fragmentShaderFileName = boost::lexical_cast<std::string>(*firstWord++);
					//we load the shader
					loadDeferredShader(_shaderNumber, _type, _shaderProgramName, _vertexShaderFileName, _fragmentShaderFileName);
					// we read the next line to see if we are still going to load shaders or we finished loading
				}
				getline(fileIn,lineBuffer,'\n');
			}
		}

		if (lineBuffer == "[Lights]")
		{
			ngl::Real _Radius = 0.0f;
			ngl::Real _Height = 0.0f;
			ngl::Vec3 _lightLookAt(0);

			getline(fileIn,lineBuffer,'\n');
			while ((lineBuffer != "[Models]") && !fileIn.eof())
			{
				// now tokenize the line
				tokenizer tokens(lineBuffer, sep);
				// and get the first token
				tokenizer::iterator  firstWord = tokens.begin();
				// parameters for light creation
				if(*firstWord == "LightParams")
				{
					*firstWord++;

					GLint _shaderNumber = boost::lexical_cast<GLint>(*firstWord++);
					float f_x = boost::lexical_cast<float>(*firstWord++);
					float f_y = boost::lexical_cast<float>(*firstWord++);
					float f_z = boost::lexical_cast<float>(*firstWord++);
					float f_w;
					ngl::Vec3 _lightPosition(f_x, f_y, f_z);

					f_x = boost::lexical_cast<float>(*firstWord++);
					f_y = boost::lexical_cast<float>(*firstWord++);
					f_z = boost::lexical_cast<float>(*firstWord++);
					f_w = boost::lexical_cast<float>(*firstWord++);
					ngl::Colour _diffuseColor(f_x, f_y, f_z, f_w);

					f_x = boost::lexical_cast<float>(*firstWord++);
					f_y = boost::lexical_cast<float>(*firstWord++);
					f_z = boost::lexical_cast<float>(*firstWord++);
					f_w = boost::lexical_cast<float>(*firstWord++);
					ngl::Colour _specularColor(f_x, f_y, f_z, f_w);

					ngl::LIGHTMODES _lightType;
					std::string _typeString = boost::lexical_cast<std::string>(*firstWord++);
					if (_typeString == "POINTLIGHT")
					{
						_lightType = ngl::POINTLIGHT;
						_Radius = boost::lexical_cast<float>(*firstWord++);
						_Height = 0.0f;
					}
					else if (_typeString == "SPOTLIGHT")
					{
						_lightType = ngl::SPOTLIGHT;
						_Radius = boost::lexical_cast<float>(*firstWord++);
						_Height = boost::lexical_cast<float>(*firstWord++);
						ngl::Vec3 _lookAt(f_x, f_y, f_z);
						float f_x = boost::lexical_cast<float>(*firstWord++);
						float f_y = boost::lexical_cast<float>(*firstWord++);
						float f_z = boost::lexical_cast<float>(*firstWord++);
						_lightLookAt.set(f_x, f_y, f_z);
					}
					else if (_typeString == "DIRECTIONALLIGHT")
					{
						_lightType = ngl::DIRECTIONALLIGHT;
						ngl::Real _Radius = boost::lexical_cast<float>(*firstWord++);
						ngl::Real _Height = 0.0;
						ngl::Vec3 _lightLookAt(0);
					}
					//Add our light.
					setLightInScene(_shaderNumber, _lightPosition, _diffuseColor, _specularColor, _lightType, _Radius, _Height, _lightLookAt);
				}
				// If we have a pointlight
				else if(*firstWord == "PointAttenParams")
				{
					*firstWord++;

					GLint _shaderNumber = boost::lexical_cast<GLint>(*firstWord++);
					ngl::Real _LightRadius = boost::lexical_cast<float>(*firstWord++);
					ngl::Real _lightCutoff = boost::lexical_cast<float>(*firstWord++);

					// Load the values to our light.
					setPointLightAttenuation(_shaderNumber,_LightRadius, _lightCutoff);
				}
				// If we have a spotlight or directional light
				else if(*firstWord == "LightAttenParams") //SpotLightParams
				{
					*firstWord++;

					GLint _shaderNumber = boost::lexical_cast<GLint>(*firstWord++);
					ngl::Real _constant = boost::lexical_cast<float>(*firstWord++);
					ngl::Real _linear = boost::lexical_cast<float>(*firstWord++);
					ngl::Real _quadratic = boost::lexical_cast<float>(*firstWord++);

					// Load the values to our light.
					setLightAttenuation(_shaderNumber, _constant, _linear, _quadratic);
				}
				// If we declare spotlight parameters
				else if(*firstWord == "SpotLightParams")
				{
					*firstWord++;

					GLint _shaderNumber = boost::lexical_cast<GLint>(*firstWord++);
				  ngl::Real _exponent = boost::lexical_cast<float>(*firstWord++);
					ngl::Real _cutOffAngle = boost::lexical_cast<float>(*firstWord++);
				

					// Load the values to our light.
					setSpotLightCutOffAndExponent(_shaderNumber,_exponent,_cutOffAngle);
				}
				// we read the next line to see if we finished loading or not
				getline(fileIn,lineBuffer,'\n');
			}
		}
		if (lineBuffer == "[Models]")
		{
			getline(fileIn,lineBuffer,'\n');
			while (!fileIn.eof())
			{
				// now tokenize the line
				tokenizer tokens(lineBuffer, sep);
				// and get the first token
				tokenizer::iterator  firstWord = tokens.begin();
				// parameters for light creation
				if(*firstWord == "Model")
				{
					*firstWord++;

					GLint _shaderNumber = boost::lexical_cast<GLint>(*firstWord++);
					std::string _modelName = boost::lexical_cast<std::string>(*firstWord++);
					ngl::Material _material;
					std::string _materialname = boost::lexical_cast<std::string>(*firstWord++);
					if (_materialname =="GOLD")
					{
						_material = ngl::GOLD;
					}
					else if (_materialname =="SILVER")
					{
						_material = ngl::SILVER;
					}
					else if (_materialname =="BLACKPLASTIC")
					{
						_material = ngl::BLACKPLASTIC;
					}
					else if (_materialname =="BRASS")
					{
						_material = ngl::BRASS;
					}
					else if (_materialname =="BRONZE")
					{
						_material = ngl::BRONZE;
					}
					else if (_materialname =="CHROME")
					{
						_material = ngl::CHROME;
					}
					else if (_materialname =="COPPER")
					{
						_material = ngl::COPPER;
					}
					else if (_materialname =="PEWTER")
					{
						_material = ngl::PEWTER;
					}
					else if (_materialname =="POLISHEDSILVER")
					{
						_material = ngl::POLISHEDSILVER;
					}
					std::string _useMaterialbool = boost::lexical_cast<std::string>(*firstWord++);
					bool _useMaterial;
					if(_useMaterialbool == "true")
					{
						_useMaterial = true;
					}
					else
					{
						_useMaterial = false;
					}
					
					// Load the model and material
					loadModel(_shaderNumber, _modelName, _material, _useMaterial);

				}
				else if(*firstWord == "Textures")
				{
					*firstWord++;

					int _modelIndex = boost::lexical_cast<int>(*firstWord++);
					const int _numberOfTextures = boost::lexical_cast<int>(*firstWord++);
					const std::string _textureName1 = boost::lexical_cast<std::string>(*firstWord++);
					const std::string _textureName2 = boost::lexical_cast<std::string>(*firstWord++);
					const std::string _textureName3 = boost::lexical_cast<std::string>(*firstWord++);

					// Load the textures to the model
					loadTextures(_modelIndex, _numberOfTextures, _textureName1, _textureName2, _textureName3);
				}
				else if(*firstWord == "PositionScaleRotation")
				{
					*firstWord++;

					GLint _modelIndex = boost::lexical_cast<GLint>(*firstWord++);
					
					float f_x = boost::lexical_cast<float>(*firstWord++);
					float f_y = boost::lexical_cast<float>(*firstWord++);
					float f_z = boost::lexical_cast<float>(*firstWord++);
					ngl::Vec3 _position(f_x, f_y, f_z);

					f_x = boost::lexical_cast<float>(*firstWord++);
					f_y = boost::lexical_cast<float>(*firstWord++);
					f_z = boost::lexical_cast<float>(*firstWord++);
					ngl::Vec3 _scale(f_x, f_y, f_z);
					
					f_x = boost::lexical_cast<float>(*firstWord++);
					f_y = boost::lexical_cast<float>(*firstWord++);
					f_z = boost::lexical_cast<float>(*firstWord++);
					ngl::Vec3 _rotation(f_x, f_y, f_z);

					// Apply the position, scale, rotation
					setModelPosScaleRot(_modelIndex, _position, _scale, _rotation);

				}
				// we read the next line to see if we finished loading or not
				getline(fileIn,lineBuffer,'\n');
			}
	}
}
// We close the file.
fileIn.close();
}
