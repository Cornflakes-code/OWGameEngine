#pragma once
#include <vector>
#include <string>
#include <functional>

class Mesh;
class Shader;
struct ModelData;
typedef std::function<void(ModelData* m)> navFunction;

void testTraverse();

struct ModelData
{
	ModelData* parent;
	std::vector<ModelData*> children;
	std::vector<Mesh> meshes;
	void dfsTraverse(navFunction pfn);
};
