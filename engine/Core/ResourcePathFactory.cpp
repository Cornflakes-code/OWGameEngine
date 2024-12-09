#include "ResourcePathFactory.h"

#include <iostream>
#include <fstream>

#include "ErrorHandling.h"
#include "LogStream.h"

std::map<ResourcePathFactory::ResourceType, 
	std::set<std::filesystem::path>> ResourcePathFactory::mResourcePaths;

std::string ResourcePathFactory::toString(ResourceType rt)
{
	switch (rt)
	{
		case ResourceType::Shader: return "Shader";
		case ResourceType::Font: return "Font";
		case ResourceType::Texture: return "Texture";
		case ResourceType::Model: return "Model";
		case ResourceType::SaveFile: return "SaveFile";
		case ResourceType::UnknownType: return "UnknownType";
		default: return "Internal logic error";
	}	
}

ResourcePathFactory::ResourceType 
	ResourcePathFactory::resourceTypeFromString(const std::string& rt)
{
	if (rt == "Shader") return ResourceType::Shader;
	if (rt == "Font") return ResourceType::Font;
	if (rt == "Texture") return ResourceType::Texture;
	if (rt == "Model") return ResourceType::Model;
	if (rt == "SaveFile") return ResourceType::SaveFile;
	if (rt == "UnknownType") return ResourceType::UnknownType;
	throw NMSException(std::stringstream()
		<< "Unknown ResourceType [" << rt << "]/n");
}

void ResourcePathFactory::addPath(const std::filesystem::path& path, ResourceType key)
{
	std::pair<std::map<ResourcePathFactory::ResourceType,
			std::set<std::filesystem::path>>::iterator, bool> iter;
	iter.first = mResourcePaths.find(key);
	if (iter.first == mResourcePaths.end())
	{
		std::pair<std::map<
			std::string, 
			std::set<std::filesystem::path>>::iterator, bool> it;
		std::set<std::filesystem::path> newSet;
		newSet.insert(path);
		mResourcePaths[key] = newSet;
	}
	else
	{
		// found map[key]. Add the path to the key
		iter.first->second.insert(path);
	}
}

std::filesystem::path 
ResourcePathFactory::appendPath(const std::string& fileName, ResourceType key)
{
	// First search for a set of paths identified by the key. If that is not found 
	// then get the set of paths without a key identifier.
	auto it = mResourcePaths.find(key);
	if (it == mResourcePaths.end())
	{
		if (key != ResourceType::UnknownType)
			it = mResourcePaths.find(ResourceType::UnknownType);
	}
	if (it == mResourcePaths.end())
	{
		// No path of this type, nor a path of UnknownType, found
		throw NMSException(std::stringstream() 
			<< "Fatal Error: No resource path of type ["
			<< toString(key) << "] found\n.");
	}
		// now search for the file in every found path.
	for (auto& p : it->second)
	{
		std::filesystem::path p1 = p / fileName;
		if (std::filesystem::is_regular_file(p1))
		{
			return p1;
		}			
	}
	// It may be a fully qualified path anyway.
	if (std::filesystem::is_regular_file(fileName))
	{
		return fileName;
	}
	std::filesystem::path p1 = fileName;

	p1.remove_filename();
	if (!std::filesystem::is_directory(p1))
	{
		p1 = fileName;
//		const std::filesystem::path p2 
//			= std::filesystem::u8path(p1.string());
		std::stringstream ss;
		//ss.imbue(std::locale());
		ss << "Fatal Error: Directory ["
			<< p1.string() << "] not found. File ["
			<< fileName << "] not loaded/n";
		throw NMSException(ss.str());
	}

	throw NMSException(std::stringstream() 
			<< "Fatal Error: File ["
			<< fileName << "] not found in directories:\n");
//	return std::filesystem::path();
}

