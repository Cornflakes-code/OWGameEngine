#pragma once

#include <vector>
#include <string>

#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>


#include "MovableObject.h"
#include "ErrorHandling.h"
#include "BoundingBox.h"
#include "Renderable.h"

#include "../Renderers/RendererBase.h"

class SceneGraphNode;

class OWActor;
typedef std::function<bool(SceneGraphNode* o)> SceneGraphNodeCallbackType;

class OWENGINE_API SceneGraphNode
{
public:
	SceneGraphNode(std::string& _name, SceneGraphNode* _parent = nullptr)
		: mName(_name), mParent(_parent) {}
	SceneGraphNode(SceneGraphNode* _parent = nullptr)
	: mParent(_parent){}
	void parent(SceneGraphNode* newParent) { mParent = newParent; }
	std::string mName;
	SceneGraphNode* addChild(SceneGraphNode* newChild);
	SceneGraphNode* findChild(const std::string& _name);
	void scale(const glm::vec3& scaleFactor)
	{
		mScaleFactor = scaleFactor;
	}
	void translate(const glm::vec3& translateVector)
	{
		mTranslateVector = translateVector;
	}
	size_t add(OWActor* toAdd)
	{
		mActors.push_back(toAdd);
		return mActors.size() - 1;
	}
	void readyForRender() { mReadyForRender = true; }
	void render(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4& model,
		const glm::vec3& cameraPos,
		RenderTypes::ShaderMutator renderCb = nullptr,
		RenderTypes::ShaderResizer resizeCb = nullptr);
	bool traverse(SceneGraphNodeCallbackType proc);
protected:
	glm::quat mQuat = glm::quat();
	glm::vec3 mScaleFactor = { 1,1,1 }; // Should be in mQuat
	glm::vec3 mTranslateVector = { 0,0,0 }; // Should be in mQuat
	SceneGraphNode* mParent = nullptr;
	std::vector<SceneGraphNode*> mChildren;
private:
	std::vector<OWActor*> mActors;
	bool mReadyForRender = false;
};




