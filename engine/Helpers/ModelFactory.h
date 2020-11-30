#pragma once
#include <string>
#include <map>

class Model;
class ModelLoader
{
public:
	ModelLoader();
	bool create(const std::string& fileName, Model& model, bool cache = false);
private:
	std::map<std::string, Model*> mCache;

};