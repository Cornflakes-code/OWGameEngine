#include "ModelData.h"

#include <string>
#include <functional>


// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Test Code
struct ModelData2;
typedef void navFunction2(ModelData2* m);

struct ModelData2
{
	ModelData2* parent;
	std::string name;
	std::vector<ModelData2*> children;
	std::vector<std::string> meshes;
	void traverse(navFunction2 pfn)
	{
		pfn(this);
		for (auto child : children)
		{
			child->traverse(pfn);
		}
	}
};

int gi = 0;
void doTraverse(ModelData2* m)
{
	m->name += "--" + std::to_string(gi++);
}

void testTraverse()
{
	ModelData2 root;
	root.name = "root";
	root.meshes.push_back("01");
	root.meshes.push_back("02");

	ModelData2 child1;
	child1.name = "child1";
	child1.meshes.push_back("11");
	child1.meshes.push_back("12");
	root.children.push_back(&child1);
	ModelData2 child11;
	ModelData2 child12;
	{
		child11.name = "child11";
		child11.meshes.push_back("111");
		child11.meshes.push_back("112");

		child1.children.push_back(&child11);

		child12.name = "child12";
		child12.meshes.push_back("121");
		child12.meshes.push_back("122");
		child1.children.push_back(&child12);
	}
	ModelData2 child2;
	child2.name = "child2";
	child2.meshes.push_back("21");
	child2.meshes.push_back("22");
	root.children.push_back(&child2);

	ModelData2 child3;
	child3.name = "child3";
	child3.meshes.push_back("31");
	child3.meshes.push_back("32");
	root.children.push_back(&child3);
	ModelData2 child31;
	ModelData2 child32;
	{
		child31.name = "child31";
		child31.meshes.push_back("311");
		child31.meshes.push_back("312");
		child3.children.push_back(&child31);

		child32.name = "child32";
		child32.meshes.push_back("321");
		child32.meshes.push_back("322");
		child3.children.push_back(&child32);
	}

	std::string f = "fred";
	std::function<void(ModelData2*)> recurse;
	recurse = [&](ModelData2* m) {
		m->name += "-" + f + std::to_string(gi++);
		for (auto child : m->children)
		{
			recurse(child);
		}
	};
	recurse(&root);
	f = "mary";
}