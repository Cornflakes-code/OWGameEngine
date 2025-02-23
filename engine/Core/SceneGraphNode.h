#pragma once

#include <vector>
#include <string>

#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>


#include "ErrorHandling.h"
#include "Renderable.h"
#include "../Component/MovableObject.h"
#include "../Geometry/BoundingBox.h"
#include "../Renderers/RendererBase.h"

class SceneGraphNode;

class OLDActor;
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
	size_t add(OLDActor* toAdd)
	{
		mActors.push_back(toAdd);
		return mActors.size() - 1;
	}
	void readyForRender(bool newValue) { mReadyForRender = newValue; }
	bool readyForRender() const { return mReadyForRender; }
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
	std::vector<OLDActor*> mActors;
	bool mReadyForRender = false;
};




