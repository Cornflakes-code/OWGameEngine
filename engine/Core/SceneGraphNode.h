#pragma once

#include <vector>
#include <string>

#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>


#include "MovableObject.h"
#include "ErrorHandling.h"
#include "BoundingBox.h"
#include "../Renderers/RendererBase.h"

class SceneGraphNode;

typedef std::function<bool(SceneGraphNode* o)> SceneGraphNodeCallbackType;

class OWENGINE_API Renderable
{
};

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
	void appendRenderCallback(RendererBase::RenderCallbackType pfunc);
	void appendResizeCallback(RendererBase::ResizeCallbackType pfunc);
	size_t addRenderer(RendererBase* toAdd)
	{
		mRenderers.push_back(toAdd);
		return mRenderers.size() - 1;
	}
	void readyForRender() { mReadyForRender = true; }
	void render(const glm::mat4& proj,
		const glm::mat4& view, const glm::mat4& model,
		const glm::vec3& cameraPos,
		RendererBase::RenderCallbackType renderCb = nullptr,
		RendererBase::ResizeCallbackType resizeCb = nullptr);
	bool traverse(SceneGraphNodeCallbackType proc);
	virtual int update(float dt) { return 0; }
protected:
	glm::quat mQuat = glm::quat();
	glm::vec3 mScaleFactor = { 1,1,1 }; // Should be in mQuat
	glm::vec3 mTranslateVector = { 0,0,0 }; // Should be in mQuat
	SceneGraphNode* mParent = nullptr;
	std::vector<SceneGraphNode*> mChildren;
private:
	std::vector<RendererBase*> mRenderers;
	bool mReadyForRender = false;
};




