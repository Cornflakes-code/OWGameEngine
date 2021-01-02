#pragma once
#include <string>
#include <map>
#include <experimental/filesystem>
#include "../Helpers/MeshData.h"
#include "../Helpers/ModelData.h"


class ModelFactory
{
	typedef std::map<std::experimental::filesystem::path, 
		std::shared_ptr<MeshData>> ModelCache;
public:
	ModelFactory();
	ModelData* create(const std::string& modelFileName, bool cache);
private:
};