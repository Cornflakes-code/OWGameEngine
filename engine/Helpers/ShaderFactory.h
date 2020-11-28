#pragma once
#include <map>
#include <experimental/filesystem>

#include "../OWEngine/OWEngine.h"

class OWENGINE_API ShaderFactory
{
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
	static std::map<std::experimental::filesystem::path, std::string> mLoadedFiles;
#pragma warning( pop )
};

