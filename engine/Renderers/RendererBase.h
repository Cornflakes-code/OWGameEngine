#pragma once
#include <functional>
#include <string>
#include <algorithm>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "../Renderers/RenderTypes.h"
#include "../Geometry/BoundingBox.h"
#include "../Helpers/RenderData.h"

class Shader;

class OWENGINE_API OWRenderer
{
	bool mSetup = false;
public:
	enum RenderType { DRAW_NONE, DRAW_MULTI, DRAW_PRIMITIVE };
	struct SSBO
	{
		void* data = nullptr; // Owned by original creator of the SSBO
		size_t size = 0;
	};
	OWRenderer(const std::string& shaderFileName, RenderType rt);
	OWRenderer(Shader* _shader, RenderType rt)
		: mShader(_shader), mDrawType(rt)
	{
	}
	void setup(const OWRenderData& renderData)
	{
		if (!mSetup)
		{
			mSetup = true;
			doSetup(renderData);
		}
	}
	void render(std::vector<glm::mat4> models, const glm::mat4& proj,
		const glm::mat4& view, const glm::vec3& cameraPos);

	// OpenGL state functions
	void drawModes(unsigned int indices, unsigned int vertices)
	{
		// Used by drawElements
		mIndicesMode = indices;
		// used by drawArrays
		mVertexMode = vertices;
	}
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
	void ssbo(const SSBO& _data)
	{
		mSSBO = _data;
	}

	virtual ~OWRenderer() {}
protected:
	virtual void doSetup(const OWRenderData& renderData) = 0;
	virtual void doRender() = 0;
	virtual void validateBase() const;
	Shader* shader() { return mShader; }
	unsigned int indicesMode() const { return mIndicesMode; }
	unsigned int vertexMode() const { return mVertexMode; }
	SSBO mSSBO;
	RenderType mDrawType = DRAW_NONE;
private:
	Shader* mShader = nullptr;

	// OpenGL state variables. The default values are used as flags 
	// in the various RIAA Utility classes in CommonUtils.h

	float mLineWidth = -1.0f;
	unsigned int mPolygonFace = GL_FRONT_AND_BACK;
	unsigned int mPolygonMode = UINT_MAX;

	unsigned int mIndicesMode = GL_TRIANGLES; // drawElements
	unsigned int mVertexMode = GL_TRIANGLES; // drawArrays

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
