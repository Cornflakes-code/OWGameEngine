#include "GlobalSettings.h"
#include <fstream>

#include <json/single_include/nlohmann/json.hpp>

#include "UserInput.h"
#include "ResourcePathFactory.h"
#include "ErrorHandling.h"
#include "LogStream.h"
#include "Movie.h"
#include "Camera.h"

OWUtils::Time::time_point GlobalSettings::mLoadTime;
extern OWENGINE_API GlobalSettings* globals;

GlobalSettings::GlobalSettings(const std::filesystem::path& configFile)
{
	if (globals)
		throw NMSException("GlobalSettings ctor called a second time");
	mLoadTime = OWUtils::Time::now();
	readFile(configFile);
}

float GlobalSettings::secondsSinceLoad()
{
	OWUtils::Time::time_point t = OWUtils::Time::now();
	auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(t - mLoadTime);
	return milli.count() / 1000.0f;
}

struct ConfigFileStruct
{
	struct Directory
	{
		std::string directory;
		ResourcePathFactory::ResourceType resType;
	};
	struct WindowCoord
	{
		int x;
		int y;
	};
	struct Window
	{
		WindowCoord size = { 800,600 };
		WindowCoord position = { 100,100 };
	};
	struct OpenGL
	{
		unsigned int swapInterval = 1;
		Window window;
		int monitor = 1;
	};
	struct Joystick
	{
		int colour = 2;
	};
	struct Mouse
	{
		int colour = 3;
	};
	struct PointingDevices
	{
		Mouse mouse = { {2} };
		Joystick joystick = { {3} };
	};
	struct Camera
	{
		float moveScale = 1.0f;
		float scale = 1.5f;
		float fov = 45;
	};
	struct KeyMapping
	{
		std::string key;
		int mods;
		UserInput::BaseUserCommand logical;
	};

	std::string version = "0.0";
	std::vector<Directory> directories;

	OpenGL openGL = {
		{1}, // swapInterval
		{ {800, 600}, {100,100} }, // size, position
		{1} // index into glfwGetMonitors
	};
	PointingDevices pointingDevices = {
		{{1}}, // mouse
		{{1}}, // Joystick
	};
	Camera camera;

	std::vector<KeyMapping> keyMap{

	};
};
ConfigFileStruct gConfigFile;

void to_json(json& j, const ConfigFileStruct::Directory& d)
{
	j = json{ {"Directory", d.directory},{"ResourceType",
		ResourcePathFactory::toString(d.resType)} };
}

void from_json(const json& j, ConfigFileStruct::Directory& d)
{
	j.at("Directory").get_to(d.directory);
	std::string s;
	j.at("ResourceType").get_to(s);
	d.resType = ResourcePathFactory::resourceTypeFromString(s);
}

void to_json(json& j, const ConfigFileStruct::WindowCoord& d)
{
	j = json{ {"x", d.x},{"y", d.y} };
}

void from_json(const json& j, ConfigFileStruct::WindowCoord& d)
{
	j.at("X").get_to(d.x);
	j.at("Y").get_to(d.y);
}

void to_json(json& j, const ConfigFileStruct::Window& d)
{
	j = json{ {"Size", d.size},{"Position", d.position} };
}

void from_json(const json& j, ConfigFileStruct::Window& d)
{
	j.at("Size").get_to(d.size);
	j.at("Position").get_to(d.position);
}

void to_json(json& j, const ConfigFileStruct::OpenGL& d)
{
	j = json{ {"SwapInterval", d.swapInterval},
			{"Window", d.window},
			{"Monitor", d.monitor} };
}

void from_json(const json& j, ConfigFileStruct::OpenGL& d)
{
	j.at("SwapInterval").get_to(d.swapInterval);
	j.at("Window").get_to(d.window); 
	j.at("Monitor").get_to(d.monitor);
}

void to_json(json& j, const ConfigFileStruct::Mouse& d)
{
	j = json{ {"Colour", d.colour} };
}

void from_json(const json& j, ConfigFileStruct::Mouse& d)
{
	j.at("Colour").get_to(d.colour);
}

void to_json(json& j, const ConfigFileStruct::Joystick& d)
{
	j = json{ {"Colour", d.colour} };
}

void from_json(const json& j, ConfigFileStruct::Joystick& d)
{
	j.at("Colour").get_to(d.colour);
}

void to_json(json& j, const ConfigFileStruct::PointingDevices& d)
{
	j = json{ {"Mouse", d.mouse}, {"Joystick", d.joystick} };
}

void from_json(const json& j, ConfigFileStruct::PointingDevices& d)
{
	j.at("Mouse").get_to(d.mouse);
	j.at("Joystick").get_to(d.joystick);
}

void to_json(json& j, const ConfigFileStruct::Camera& d)
{
	j = json{ "MoveScale", d.moveScale};
	j = json{ "Scale", d.scale};
	j = json{ "FOV", d.fov};
}

void from_json(const json& j, ConfigFileStruct::Camera& d)
{
	j.at("MoveScale").get_to(d.moveScale);
	j.at("Scale").get_to(d.scale);
	j.at("FOV").get_to(d.fov);
}

void to_json(json& j, ConfigFileStruct::KeyMapping& d)
{
	std::string s = j.dump(4);
	std::vector<std::string> mods;
	j = json{ {"Key", d.key},
			{"Mods", mods},
			{"Logical", d.logical} };
	for (auto& var: mods)
	{
		UserInput::InputMod im = UserInput::to_InputMod(var);
		d.mods |= (int)im;
	}
}

void from_json(const json& j, ConfigFileStruct::KeyMapping& d)
{
	j.at("Key").get_to(d.key);
	j.at("Mods").get_to(d.mods);
	j.at("Logical").get_to(d.logical);
}

void to_json(json& j, ConfigFileStruct& d)
{
//	j = json{ {"Directories", d.directories},
//			{"OpenGL", d.openGL},
//			{"PointingDevices", d.pointingDevices},
//			{"KeyMapping", d.keyMap} };
}

void from_json(const json& j, ConfigFileStruct& d)
{
	try { j.at("Version").get_to(d.version); }
	catch (const std::exception& ex)
	{
		d.version = "0.0";
		LogStream(LogStreamLevel::Error) << "Cannot parse config file Version"
			<< "Exception [" << ex.what() << "]\n";
	}
	try { j.at("Directories").get_to(d.directories); }
	catch (const std::exception& ex)
	{
		d.directories = {
			{"../engine/Resources/shaders", ResourcePathFactory::ResourceType::Shader},
			{"../engine/Resources/fonts", ResourcePathFactory::ResourceType::Font},
			{"../../engine/Resources/shaders", ResourcePathFactory::ResourceType::Shader},
			{"../../engine/Resources/fonts", ResourcePathFactory::ResourceType::Font}
		};		
		LogStream(LogStreamLevel::Error) << "Cannot parse config file Directories "
			<< "Exception [" << ex.what() << "]\n";
	}
	try { j.at("OpenGL").get_to(d.openGL); }
	catch (const std::exception& ex)
	{
		d.openGL = ConfigFileStruct::OpenGL();
		LogStream(LogStreamLevel::Error) << "Cannot parse config file OpenGL"
			<< "Exception [" << ex.what() << "]\n";
	}
	try { j.at("PointingDevices").get_to(d.pointingDevices); }
	catch (const std::exception& ex)
	{
		d.pointingDevices = ConfigFileStruct::PointingDevices();
		LogStream(LogStreamLevel::Error) << "Cannot parse config file PointingDevices"
			<< "Exception [" << ex.what() << "]\n";
	}
	try { j.at("Camera").get_to(d.camera); }
	catch (const std::exception& ex)
	{
		d.camera = ConfigFileStruct::Camera();
		LogStream(LogStreamLevel::Error) << "Cannot parse config file Camera"
			<< "Exception [" << ex.what() << "]\n";
	}
	try { j.at("KeyMapping").get_to(d.keyMap); }
	catch (const std::exception& ex)
	{
		LogStream(LogStreamLevel::Error) << "Cannot parse config file KeyMapping"
			<< "Exception [" << ex.what() << "]\n";
	}
}

void GlobalSettings::readFile(const std::filesystem::path& configFile)
{
	json j;
	float version = 0;
	if (!configFile.empty())
	{
		std::ifstream ifs(configFile);
		if (!ifs)
		{
			LogStream(LogStreamLevel::Error) << "Cannot open config file\n";
			// Even is there is no config file we continue because 
			// there are numerous default settings stored in the default
			// ConfigFileStruct* gConfigFile structure
		}
		else
		{
			j = json::parse(ifs);
			// Not sure what to with 'version' atm
			try 
			{ 
				version = j["Version"]; 
			}
			catch (const std::exception& ex)
			{
				LogStream(LogStreamLevel::Error) << "cannot parse config file ['Version' not found]"
					<< "Exception [" << ex.what() << "]\n";
			}
		}
	}

	gConfigFile = j.get<ConfigFileStruct>();
	gConfigFile.version = std::to_string(version);
	physicalWindowSize({ gConfigFile.openGL.window.size.x,
						gConfigFile.openGL.window.size.y });
}

void GlobalSettings::configAndSet(SaveAndRestore* sr, Movie* mov, MacroRecorder* mr,
	Logger* log, Camera* cam, GLApplication* app, UserInput* OW_UNUSED(ui))
{
	mSaveAndRestore = sr;

	mov->swapInterval(gConfigFile.openGL.swapInterval);
	mMovie = mov;

	mRecorder = mr;

	mLogger = log;

	cam->aspectRatio(physicalWindowSize().x /
		(1.0f * physicalWindowSize().y));
	cam->FOV(glm::radians(gConfigFile.camera.fov));
	// newValue->moveScale ??
	//  newValue->scale ??
	mCamera = cam;

	ResourcePathFactory paths;
	for (auto& d : gConfigFile.directories)
	{
		paths.addPath(d.directory, d.resType);
	}

	mApplication = app;
}
