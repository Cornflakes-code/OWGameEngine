#pragma once
#include <map>

#include <filesystem>

#include "../OWEngine/OWEngine.h"

class OWENGINE_API ShaderFactory
{
	typedef std::map<std::filesystem::path, 
			std::string> ShaderCache;
public:
	const std::string& getShader(const std::string& fileName);
	static const std::string& boilerPlateVertexShader();
	static const std::string& boilerPlateFragmentShader();
	static const std::string& boilerPlateGeometryShader();
	static const std::string& boilerPlateLightShader();
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	//std::mutex mut;
	// A cache of accessed files with their contents stored as a std::string
	static ShaderCache mLoadedFiles;
#pragma warning( pop )
};

