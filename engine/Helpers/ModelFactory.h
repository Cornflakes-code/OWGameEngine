#pragma once
#include <string>
#include <vector>
#include <map>
#include <filesystem>

#include "../OWEngine/OWEngine.h"
#include "../Helpers/Model.h"

class OWENGINE_API ModelFactory
{
//	typedef std::map<std::filesystem::path, 
//		std::shared_ptr<MeshDataHeavy*>> ModelCache;
public:
	ModelFactory();
	OWModelDataEx create(const std::string& modelFileName, bool cache);
private:
};