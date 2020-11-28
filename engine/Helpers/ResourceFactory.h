#pragma once
#include <string>
#include <istream>
#include <mutex>
#include <set>
#include <experimental/filesystem>

#include <json/single_include/nlohmann/json.hpp>

#include "../OWEngine/OWEngine.h"

using json = nlohmann::json;
/*
	Caching of paths and resources. A singleton class.
*/
class OWENGINE_API ResourceFactory
{
public:
	ResourceFactory() {}
	enum class ResourceType
	{
		Shader, Font, Texture, Model, SaveFile, UnknownType
	};
	void addPath(const std::experimental::filesystem::path& path, 
				 ResourceType key = ResourceType::UnknownType);
	std::experimental::filesystem::path appendPath(
					const std::string& fileName, ResourceType key);
	static std::string toString(ResourceType rt);
	static ResourceFactory::ResourceType
		resourceTypeFromString(const std::string& rt);
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	// A map of file paths to search. Each ResourceType can have many paths and the sam path 
	// can exist for multiple ResourceType.
	static std::map<ResourceType, std::set<std::experimental::filesystem::path>> mResourcePaths;
#pragma warning( pop )
};