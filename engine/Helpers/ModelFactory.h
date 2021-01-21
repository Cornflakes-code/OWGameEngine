#pragma once
#include <string>
#include <vector>
#include <map>
#include <experimental/filesystem>

#include "../OWEngine/OWEngine.h"

struct ModelData;
class OWENGINE_API ModelFactory
{
//	typedef std::map<std::experimental::filesystem::path, 
//		std::shared_ptr<MeshDataHeavy*>> ModelCache;
public:
	ModelFactory();
	ModelData create(const std::string& modelFileName, bool cache);
private:
};