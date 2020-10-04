#include <map>
#include <fstream>

#include <Core/GLApplication.h>
#include <Core/SaveAndRestore.h>
#include <Core/GlobalSettings.h>

#include <Helpers/Logger.h>
#include <Helpers/ResourceFactory.h>
#include <Cameras/CameraMazhar.h>
#include <Cameras/CameraOW.h>

#include <Helpers/MacroRecorder.h>

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

int main()
{
	ResourceFactory* rf = ResourceFactory::getResourceFactory();
	rf->addPath("../engine/Resources/shaders", ResourceFactory::ResourceType::Shader);
	rf->addPath("../engine/Resources/fonts", ResourceFactory::ResourceType::Font);
	rf->addPath("../../engine/Resources/shaders", ResourceFactory::ResourceType::Shader);
	rf->addPath("../../engine/Resources/fonts", ResourceFactory::ResourceType::Font);
	NMSUserInput ui;
	//ui.addKeyMapping(GLFW_KEY_W, NMSUserInput::InputMods::NoMod, NMSUserInput::UserCommand::Forward);
	//ui.addKeyMapping(GLFW_KEY_A, NMSUserInput::InputMods::NoMod, NMSUserInput::UserCommand::YawLeft);
	//ui.addKeyMapping(GLFW_KEY_S, NMSUserInput::InputMods::NoMod, NMSUserInput::UserCommand::Back);
	//ui.addKeyMapping(GLFW_KEY_D, NMSUserInput::InputMods::NoMod, NMSUserInput::UserCommand::YawRight);
	//ui.addKeyMapping(GLFW_KEY_Q, NMSUserInput::InputMods::NoMod, NMSUserInput::UserCommand::Left);
	//ui.addKeyMapping(GLFW_KEY_E, NMSUserInput::InputMods::NoMod, NMSUserInput::UserCommand::Right);
	//ui.addKeyMapping(GLFW_KEY_R, NMSUserInput::InputMods::NoMod, NMSUserInput::UserCommand::PitchUp);
	//ui.addKeyMapping(GLFW_KEY_F, NMSUserInput::InputMods::NoMod, NMSUserInput::UserCommand::PitchDown);
	//ui.addKeyMapping(GLFW_KEY_F, NMSUserInput::InputMods::NoMod, NMSUserInput::UserCommand::PitchDown);
	
	// It would be nice if the following call removed base class mapping
	//ui.addKeyMapping(GLFW_KEY_W, NMSUserInput::InputMods::NoMod, UserInput::BaseUserCommand::NoCommand);
	// The following call will allow any key input to be used if the key is not used for anything else
	//ui.addKeyMapping(UserInput::AnyKey, NMSUserInput::InputMods::NoMod, NMSUserInput::NMSUserCommand::SpecialKey1);

	//	nms.addKeyMapping(ANY_KEY, KeyMods::NoMod, NMSUserCommands::AnyKey);
	//CameraMazhar camera;

	try
	{
		SaveAndRestore sr;
		MacroRecorder recorder;
		Logger logger;
		GLApplication app(&ui);
		globals->resourceCache(rf);
		globals->recorder(&recorder);
		globals->saveAndRestore(&sr);
		globals->logger(&logger);
		CameraOW camera;
		NMSMovie nms(&camera);
		app.init(&nms, &ui, &recorder, &sr);
		app.run(&nms);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
		int c;
		std::cin >> c;
	}
}
