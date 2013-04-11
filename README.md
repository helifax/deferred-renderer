Deferred Renderer (Shading technique) using OpenGL and NGL Library
===================================================================

This project was made for my "Animation Software Development" unit assignment part 
of the MSC of Computer Animation and Visual Effects course at Bournemouth University.


Thank you for your interest in this project.

_______________________________________

CONTENTS
_______________________________________



This file contains the following sections:

  1) SYSTEM REQUIREMENT

  2) GENERAL NOTES
	
  3) LICENSE

  4) COMPILING ON WIN32 WITH VISUAL C++ 2012 EXPRESS EDITION

  5) COMPILING ON GNU/LINUX
	
  6) FEATURES
	
  7) KNOWN ISSUES
  
  8) REFERENCES


_______________________________________

1) SYSTEM REQUIREMENTS
_______________________________________

The Git repository contains also a built version of the Renderer that can be run on Windows OS. (Sorry for not including a Linux binary also)

In order to build from code :

The project uses the NGL(NCCA Graphics Library) video library that must also be compiled and used, both under Linux and Windows.
The library is created and maintained by Jon Macey @ Bournemouth University and is for education purposes only.
All the information regarding NGL lib, how and from where to acquire it can be found at: http://nccastaff.bournemouth.ac.uk/jmacey/GraphicsLib/index.html


  Linux:
  
  - QTCreator above v.4.0
  - clang++ compiler
  - Boost Libraries abouve 1.4
  - Bazzar (BZR) subversion system in order to download the NGL lib source code.
  - A GPU and drivers capable of using OpenGL 4.0 core profile.


  Windows:
  
  - QTCreator above v.4.0
  - Visual Studio 2012 Express
  - Boost Libraries abouve 1.4
  - Bazzar (BZR) subversion system in order to download the NGL lib source code.
  - A GPU and drivers capable of using OpenGL 4.0 core profile.


_______________________________________

2) GENERAL NOTES
_______________________________________




The project is for educational purpose only. 
For a list of features that are implemeneted please see 6) FEATURES
As with every software there are bugs or things that are not working as aspected. For a list please see 7) KNOWN ISSUES



_______________________________________

3) LICENSE
_______________________________________



The source code can be copied and distributed unde the GNU General Public License. 
The GNU Public License information can be found at: http://www.gnu.org/licenses/gpl.html



_______________________________________

4) COMPILING ON WIN32 WITH VISUAL C++ 2012 EXPRESS EDITION
_______________________________________



1. The instruction on how to build the NGL lib unde Microsoft Windows can be found at:
http://portofolio.marte.ro/NGL/Tutorial/HowTo.html (Setting NGL on Windows)

2. Once the NGL library has been build and the project source code has been downloaded, add the "DeferredRenderer.vcxproj"
to the Visual C++ Solution and build it.



_______________________________________

5) COMPILING ON GNU/LINUX
_______________________________________



1. The instuction on how to build the NGL lib under Linux can be found at: http://nccastaff.bournemouth.ac.uk/jmacey/GraphicsLib/index.html
2. Once the NGL library has been build and the project source code has been downloaded, open using QTCreator the "DeferredRenderer.pro" file.
3. Select "clang++" compiler from the Project Options to be the used compiler.
4. Issue qmake and make either from cmd line or from QTCreator.



_______________________________________

6) FEATURES
_______________________________________



The project has following features implemented:
  - Simple configuration file to set up a scene.
  - Deferred Shading technique using Stencil buffer for light volume calculation.
  - Point Light, Sport Light and Directional Light shader programs.
  - Loading of OBJ meshes.
  - Ability to use Diffuse Textures, Normal Maps and Specular Maps.
  - Simple and efficient SSAO (Space Screen Ambient Occlusion).

A quick video showing the result can be seen at: http://www.youtube.com/watch?v=IjR5sTiylhU



_______________________________________

7) KNOWN ISSUES
_______________________________________



The are a couple of problems with the implementation in its current form:
  - Using only Diffuse Textures will create some visual errors. This is because the binormals and tangents are still being used
  but no NormalMap is being specified.
  - The code does not offer the posibility to specify the direction in which the Directional Light points, yet.
  - The NormalMaps exported from Zbrush do not work properly unless before export the G (Green) channel is inverted,
  The texture also needs to be flipped on Vertical before Zbrush export.
  - The SSAO shader needs to be tweaked. For demonstration purposes the "downloaded shader" from this repository is configured
  to show the existance of SSAO and the results are very exagerated.
  - For a more "natural and intended outcome" of the SSAO pass can be seen at: http://www.youtube.com/watch?v=hdSzG7DzpXM


_______________________________________

8) REFERENCES
_______________________________________



The current implementation uses alghoritms and methods by their authors and can be found in:

- Dudash B. Next Generation Shading and Rendering. Nvidia Corporation. Available from: https://developer.nvidia.com/sites/default/files/akamai/gamedev/docs/English_Advanced_Shading.pdf.[Last accessed 10 December 2012]
- Hargreaves S., Harris M., 2004. Deferred Rendering. NVIDIA Corporation. Available from: http://developer.download.nvidia.com/assets/gamedev/docs/6800_Leagues_Deferred_Shading.pdf. [Last accessed 10 December 2012]
- Koonce R. and Nguyen H., 2007. GPU Gems 3. Addison-Wesley Professional.
- Lottes T., 2009. FXAA. NVIDIA Corporation. Available from: http://www.ngohq.com/images/articles/fxaa/FXAA_WhitePaper.pdf . [Last accessed 10 December 2012]
- Meiri E., 2012. Tutorial 35 - Deferred Shading - Part 1. Available from: http://ogldev.atspace.co.uk/www/tutorial35/tutorial35.html. [Last accessed 10 December 2012]
- Meiri E., 2012. Tutorial 36 - Deferred Shading - Part 2. Available from: http://ogldev.atspace.co.uk/www/tutorial36/tutorial36.html [Last accessed 10 December 2012]
- Meiri E., 2012. Tutorial 37 - Deferred Shading - Part 3. Available from: http://ogldev.atspace.co.uk/www/tutorial37/tutorial37.html. [Last accessed 10 December 2012]
- OpenGL documentation. 2012. Common Mistakes. The Industry's Foundation for High Performance Graphics. Available from: http://www.opengl.org/wiki/Common_Mistakes#Texture_upload_and_pixel_reads. [Last accessed 10 December 2012]
- Mendez J.M., 2010 A simple and practical approach to SSAO. Available from: http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/a-simpleand-practical-approach-to-ssao-r2753 [Last accessed 28 March 2013]
- Reshetov A., 2009 MLAA. Intel Labs. Available from: http://visual-computing.intelresearch.net/publications/papers/2009/mlaa/mlaa.pdf [Last accessed 10 December 2012]Shishkovtsov O. and Pharr M., 2005. GPU Gems 2. Addison-Wesley Professional.
- Villa J.R. OpenGL Shading Language Course. TyphoonLabs Available from:http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_1.pdf [Last accessed 10 December 2012]
- Wolff. D and Iyer K., 2011. OpenGL 4.0 Shading Language Cookbook. Birmingham: Packt Publishing Ltd.
- Wolfgang E.,2008. Light Pre-Pass Renderer. Available from: http://diaryofagraphicsprogrammer.blogspot.co.uk/2008/03/light-pre-pass-renderer.html [Last accessed 10 December 2012]
