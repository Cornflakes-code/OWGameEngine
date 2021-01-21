#pragma once
#include <vector>
#include <string>
#include <functional>

class RendererBase;
struct ModelData;
struct MeshDataHeavy;

void testTraverse();

struct ModelData;
typedef std::function<void(ModelData* m)> navFunction;
typedef std::function<void(const ModelData* m)> constNavFunction;
struct ModelData
{
	std::vector<ModelData> children;
	std::vector<RendererBase*> renderers;
	std::vector<MeshDataHeavy*> meshes;
	void traverse(navFunction pfn)
	{
		pfn(this);
		for (auto& child : children)
		{
			child.traverse(pfn);
		}
	}
	void traverse(constNavFunction pfn) const
	{
		pfn(this);
		for (auto& child : children)
		{
			child.traverse(pfn);
		}
	}
};
