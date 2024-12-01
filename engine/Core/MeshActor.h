#pragma once
#include <vector>

#include "Actor.h"
#include "Helpers/MeshDataHeavy.h"

struct MeshDataHeavy;
struct MeshDataLight;

class OWENGINE_API MeshActor : public Actor
{
private:
public:
	MeshActor(Actor* _owner) : Actor(_owner) {}
	void setup(const MeshDataHeavy* md, Shader* shader, unsigned int vertexMode, unsigned int vertexLocation);
	void setup(const MeshDataLight* md, Shader* shader);
	void setup(const std::vector<glm::vec4>& v, Shader* shader, unsigned int vertexMode, unsigned int vertexLocation);
private:
};
