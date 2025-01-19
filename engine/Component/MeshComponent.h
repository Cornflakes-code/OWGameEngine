#pragma once
#include <vector>

#include "Helpers/MeshDataHeavy.h"
#include "OWSceneComponent.h"

struct MeshDataHeavy;
struct MeshDataLight;
class MeshDataInstance;
class VAOBuffer;
class OWActor;
class OWENGINE_API MeshComponent : public OWSceneComponent
{
private:
	bool mCanCollide = true;
public:
	MeshComponent(OWActor* _owner, const glm::vec3&_position) 
		: OWSceneComponent(_owner, _position) {}
	void setup(const MeshDataHeavy* md, Shader* shader,
				unsigned int vertexMode, unsigned int vertexLocation);
	void setup(const MeshDataLight* md, Shader* shader);
	void setup(const std::vector<glm::vec4>& v, Shader* shader,
				unsigned int vertexMode, unsigned int vertexLocation);
	void setup(const MeshDataInstance* md, Shader* shader);
	void setup(VAOBuffer* vao, Shader* shader = nullptr);
	virtual bool canCollide() { return mCanCollide; }
private:
};
