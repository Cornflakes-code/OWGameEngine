#pragma once
#include <functional>
#include <string>
#include <algorithm>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "../Core/Renderable.h"
#include "../Renderers/RenderTypes.h"
#include "../Component/OWComponent.h"
#include "../Geometry/BoundingBox.h"
#include "../Helpers/Mesh.h"
#include "../Helpers/Model.h"

class Shader;

class OWENGINE_API OWRenderer
{
	bool mSetup = false;
public:
	virtual AABB doSetup(const std::vector<OWMeshData>& meshes,
		const std::vector<OWModelData>& models) = 0;
	AABB setup(const std::vector<OWMeshData>& meshes,
		const std::vector<OWModelData>& models)
	{
		if (!mSetup)
		{
			mSetup = true;
			return doSetup(meshes, models);
		}
		return AABB();
	}
	virtual void render(std::vector<glm::mat4> models, const glm::mat4& proj,
		const glm::mat4& view, const glm::vec3& cameraPos);

	OWRenderer(const std::string& shaderFileName)
	{
	}
	OWRenderer(Shader* shader)
	{
	}
	// OpenGL state functions
	void lineWidth(float width) { mLineWidth = width; }
	void polygonMode(unsigned int mode = GL_FILL)
	{
		mPolygonMode = mode;
	}
	void blendFunction(unsigned int sfactor, unsigned int dfactor)
	{
		mSfactor = sfactor;
		mDfactor = dfactor;
	}
	virtual ~OWRenderer() {}
protected:
	virtual void doRender() = 0;
	virtual void validateBase() const;
	Shader* shader() { return mShader; }
private:
	Shader* mShader = nullptr;

	// OpenGL state variables. The default values are used as flags 
	// in the various RIAA Utility classes in CommonUtils.h

	float mLineWidth = -1.0f;
	unsigned int mPolygonFace = GL_FRONT_AND_BACK;
	unsigned int mPolygonMode = UINT_MAX;

	unsigned int mSfactor = UINT_MAX;
	unsigned int mDfactor = UINT_MAX;
};


/*
class XOW2DRenderer: public OWRenderer
{
public:
	void render(std::vector<glm::mat4> models, const glm::mat4& proj,
		const glm::mat4& view, const glm::vec3& cameraPos) override;
	virtual AABB prepare(const OWMeshComponentBase* mc,
		RenderTypes::ShaderMutator renderCb = nullptr) override;
	XOW2DRenderer(const std::string& shaderName)
		: OWRenderer(shaderName)
	{
	}
};

class XOWMeshRenderer2DStatic: public XOW2DRenderer
{
public:
	void render(std::vector<glm::mat4> models, const glm::mat4& proj,
		const glm::mat4& view, const glm::vec3& cameraPos) override;
	virtual AABB prepare(const OWMeshComponentBase* mc,
		RenderTypes::ShaderMutator renderCb = nullptr) override;
	XOWMeshRenderer2DStatic(const std::string& shaderName)
		: XOW2DRenderer(shaderName)
	{
	}
};

class XOWMeshRenderer2DDynamic: public XOW2DRenderer
{
public:
	void render(std::vector<glm::mat4> models, const glm::mat4& proj,
		const glm::mat4& view, const glm::vec3& cameraPos) override;
	virtual AABB prepare(const OWMeshComponentBase* mc,
		RenderTypes::ShaderMutator renderCb = nullptr) override;
	XOWMeshRenderer2DDynamic(const std::string& shaderName)
		: XOW2DRenderer(shaderName)
	{
	}
};
*/
