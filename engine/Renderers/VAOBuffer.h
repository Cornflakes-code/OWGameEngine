#pragma once
#include <vector>
#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"

#include "../Helpers/MeshDataLight.h"

#include "RendererBase.h"

class Shader;

class OWENGINE_API VAOBuffer : public RendererBase
{
public:
	enum RenderType { DRAW_NONE, DRAW_ARRAYS, DRAW_MULTI, DRAW_PRIMITIVE };
	VAOBuffer(Shader* shader, RenderType rt);
	void clear();
	void add(const MeshDataLight* meshData);
	void add(const std::vector<glm::vec3>& v,
		unsigned int vertexMode, unsigned int vertexLocation = 0);
	void add(const std::vector<glm::vec4>& v,
		unsigned int vertexMode, unsigned int vertexLocation = 0);
	void addLightSource(RendererBase* aLight);
	virtual void prepare() override;
	virtual void doRender() const override;
private:
	void validate(const MeshDataLight* mdl);
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<MeshDataLight::RenderData> mData;
	std::vector<glm::vec3> mVec3;
	std::vector<glm::vec4> mVec4;
	std::vector<GLint> mMultiArrayStartIndexes;
	std::vector<unsigned int> mPrimitiveIndices;
	static unsigned int mPrimitiveRestart;
	std::vector<GLsizei> mMultiArrayVertexCount;
	std::vector<unsigned int> mIndices;
	// mVbo[0] The VBO containing the triangles to draw
	// mVbo[1] The VBO containing the positions of the particles
	// mVbo[2] The VBO containing the colors of the particles
	unsigned int mVao = std::numeric_limits<unsigned int>::max();
	unsigned int mVbo = std::numeric_limits<unsigned int>::max();
	unsigned int mEbo = std::numeric_limits<unsigned int>::max();
	unsigned int mPrimitiveEbo = std::numeric_limits<unsigned int>::max();
	RenderType mDrawType = DRAW_NONE;
#pragma warning( pop )
};