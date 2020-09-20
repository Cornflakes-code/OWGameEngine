#pragma once
#include <string>
#include <istream>
#include <mutex>
#include <set>
#include <experimental/filesystem>

#include <json/single_include/nlohmann/json.hpp>

#include "../OWEngine/OWEngine.h"

#include "FreeTypeFontAtlas.h"

using json = nlohmann::json;
/*
	Caching of paths and resources. A singleton class.
*/
class OWENGINE_API ResourceFactory
{
public:
	enum class ResourceType
	{
		Shader, Font, Texture, UnknownType
	};
	static ResourceFactory* getResourceFactory();
	void addPath(const std::experimental::filesystem::path& path, 
				 ResourceType key = ResourceType::UnknownType);
	static unsigned int nextTexture();
	static std::string& boilerPlateVertexShader();
	static std::string& boilerPlateFragmentShader();
	static std::string& boilerPlateGeometryShader();
	json get(const std::string& fileName, const std::string& jsonObj, bool cache = true);
	const std::string& getPath(const std::string& fileName, ResourceType rt);
	const FreeTypeFontAtlas::FontDetails* loadFreeTypeFont(
					const std::string&, unsigned int fontHeight);
	std::experimental::filesystem::path appendPath(
					const std::string& fileName, ResourceType key);
protected:
	ResourceFactory();
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	struct FreeTypeFont
	{
		FT_Face face;
		std::string fileName;
		int fontHeight;
	};
	FreeTypeFontAtlas mFreeType;
	std::vector<FreeTypeFont> mFreeTypes;
	//std::mutex mut;
	// A cache of accessed files with their contents stored as a std::string
	std::map<std::experimental::filesystem::path, std::string> mLoadedFiles;
	// A map of file paths to search. Each ResourceType can have many paths and the sam path 
	// can exist for multiple ResourceType.
	std::map<ResourceType, std::set<std::experimental::filesystem::path>> mResourcePaths;
	std::istream* readFile(const std::string& path);
	static std::string toString(ResourceType rt);
#pragma warning( pop )
};