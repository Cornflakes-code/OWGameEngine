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
	const std::string& boilerPlateVertexShader();
	const std::string& boilerPlateFragmentShader();
	const std::string& boilerPlateGeometryShader();

private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	//std::mutex mut;
	// A cache of accessed files with their contents stored as a std::string
	static ShaderCache mLoadedFiles;
#pragma warning( pop )
};

