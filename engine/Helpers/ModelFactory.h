#pragma once
#include <string>
#include <map>
#include <experimental/filesystem>
#include "../Renderables/Mesh.h"

class ModelFactory
{
	typedef std::map<std::experimental::filesystem::path, 
		std::shared_ptr<Mesh::ModelData>> ModelCache;
public:
	ModelFactory();
	std::shared_ptr<Mesh::ModelData>
		create(const std::string& modelFileName,
				 const std::string& textureFileName = std::string(""), 
				 bool cache = true);
private:
	ModelCache::iterator loadModel(
			const std::experimental::filesystem::path& path);
	static ModelCache mLoadedFiles;

};