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
	OWRenderer(const std::string& shaderFileName, const std::vector<GPUBufferObject::BufferType>& orderedTypes);
	OWRenderer(Shader* _shader, const std::vector<GPUBufferObject::BufferType>& orderedTypes)
		: mShader(_shader)
	{
		mSSBO.setAllowedTypes(orderedTypes);
	}
	void setup(const OWRenderData& renderData);
	OWRenderer* setOrderedTypes(const std::vector<GPUBufferObject::BufferType>& orderedTypes);
	OWRenderer* addToSSBO(const std::vector<glm::mat4>& _data, GPUBufferObject::BufferType t);
	OWRenderer* addToSSBO(const std::vector<glm::vec4>& _data, GPUBufferObject::BufferType t);
	OWRenderer* lockSSBO(const std::vector<GPUBufferObject::BufferType>& orderedTypes);
	void render(const glm::mat4& proj,
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

	virtual ~OWRenderer() {}
	Shader* shader() { return mShader; }
	GPUBufferObject mSSBO;
protected:
	virtual void doSetup(const OWRenderData& renderData) = 0;
	virtual void doRender() = 0;
	virtual void validateBase() const;
	unsigned int indicesMode() const { return mIndicesMode; }
	unsigned int vertexMode() const { return mVertexMode; }
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
