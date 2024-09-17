#include <map>
#include <fstream>
#include <experimental/filesystem>

#include <Core/GLApplication.h>
#include <Core/GlobalSettings.h>
#include <Core/SaveAndRestore.h>

#include <Core/ErrorHandling.h>
#include <Core/Logger.h>
#include <Core/LogStream.h>
#include <Core/ResourcePathFactory.h>

#include <Helpers/MacroRecorder.h>

#include <Cameras/CameraMazhar.h>
#include <Cameras/CameraOW.h>

#include "NMSMovie.h"
#include "NMSUserInput.h"

// nms2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
See below for good example on how to use complex classes 
E:\Apps\OpenGL\modern-tutorials\palette-emu-sdl2\sprites.cpp
https://www.reddit.com/r/opengl/comments/3515bi/rendering_multiple_objects_from_multiple_vaos/
https://www.khronos.org/opengl/wiki/Example_Code
https://stackoverflow.com/questions/15395382/opengl-3-3-multitexture-gl-texture1-and-following-are-always-black-only-gl-tex
Instanced data
http://ogldev.atspace.co.uk/www/tutorial33/tutorial33.html

Using vao
https://stackoverflow.com/questions/18485381/opengl-how-many-vaos?noredirect=1&lq=1
https://paroj.github.io/gltut/Positioning/Tutorial%2005.html
https://stackoverflow.com/questions/8923174/opengl-vao-best-practices?noredirect=1&lq=1

Doing VR - Virtual Reality
E:\Apps\OpenGL\openvr
http://kondrak.info/posts/2016-05-21-rendering-in-vr-using-opengl-instancing/
https://github.com/ValveSoftware/openvr
https://www.gamedev.net/forums/topic/685690-htc-vive/
https://www.gamedev.net/forums/topic/686587-virtual-reality-for-dummies/
https://www.glfw.org/docs/3.1/rift.html
https://www.opengl.org/news/comments/new-opengl-4-virtual-reality-example-code
https://www.reddit.com/r/gamedev/comments/i4932q/is_it_difficult_rendering_a_triangle_or_cube_to_a/
http://casual-effects.blogspot.com/2016/03/opengl-sample-codeand-openvr-sample-code.html
http://forum.lwjgl.org/index.php?topic=6726.0
https://gist.github.com/VirtuosoChris/272f803966e62796b83dce2a597adcc7
https://github.com/ValveSoftware/openvr
https://github.com/ValveSoftware/openvr/wiki/API-Documentation
https://github.com/hdonk/lwjgl_vr_framework/tree/OpenGL

*/
class MacroRecorder;

extern OWENGINE_API GlobalSettings* globals;

// https://www.gamedeveloper.com/programming/behavior-trees-for-ai-how-they-work

// https://www.gamedeveloper.com/design/decision-modeling-and-optimization-in-game-design-part-1-introduction

/*
 to remove command window
 https://stackoverflow.com/questions/5995433/removing-console-window-for-glut-freeglut-glfw
 set Linker->System->SubSystem to "Windows (/SUBSYSTEM:WINDOWS)" instead of "Console (/SUBSYSTEM:CONSOLE)"
 set Linker->All Options->Entry Point = 'mainCRTStartup'
*/
int main(int argc, char* argv[])
{
	try
	{
		std::filesystem::path exePath;
		if (argc)
		{
			exePath = argv[0];
			LogStream::setLogFile(exePath);
			std::filesystem::path configFileName = exePath.filename();
			configFileName.replace_extension("json");
			exePath.remove_filename();
			exePath += configFileName;
			ResourcePathFactory paths;
			paths.addPath(exePath, ResourcePathFactory::ResourceType::UnknownType);
			paths.addPath(std::filesystem::current_path(),
						ResourcePathFactory::ResourceType::UnknownType);
		}
		// Config file is 'path of exe/exename.json'
		globals = new GlobalSettings;
		SaveAndRestore sr;
		MacroRecorder recorder;
		Logger logger;
		NMSUserInput ui;
		CameraOW camera;
		globals->loadSettings(exePath);
		GLApplication app(&ui);
		NMSMovie nms(&camera, &logger);
		globals->configAndSet(&sr, &nms, &recorder,
							&logger, &camera, &app);
		app.init(&nms, &ui, &recorder, &sr, &camera);
		app.run(&nms);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
		int c;
		std::cin >> c;
	}
	LogStream::closeLogFile();
}

