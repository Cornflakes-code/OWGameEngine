#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"
#include "../Helpers/ResizeHelper.h"

class Shader;

/*
	Basically a struct that contains the data to be rendered.
	Passed to VertexRenderer to be rendered.
	Can be used as is or as a base class for more complex graphics.
	No getters. Values only accessible by VertexRenderer and derived classes.
	Does this means it cannot be inherited from outside of the DLL?
*/
class OWENGINE_API VertexSource
{
public:
	typedef std::function<void(const glm::mat4& proj, const glm::mat4& view,
		const glm::mat4& model, Shader* shader)> SourceCallbackType;

	VertexSource() {}
	void shader(Shader* newValue, const std::string& pvmName) 
	{ 
		mShader = newValue; 
		mPVMName = pvmName;
	}
	void vertices(const std::vector<glm::vec3>& v,
				unsigned int location,
				unsigned int drawMode);
	void vertices(const std::vector<glm::vec4>& v,
				unsigned int location,
				unsigned int drawMode);
	void indices(const std::vector<unsigned int>& newValue,
				 unsigned int drawMode) 
	{ 
		mIndices = newValue; 
		mIndicesMode = drawMode;
	}
	void colour(const glm::vec4& newValue, const std::string& colourName) 
	{ 
		mColour = newValue; 
		mColourName = colourName;
	}
	void texture(unsigned int newValue) { mTexture = newValue; }
protected:
#pragma warning( push )
#pragma warning( disable : 4251 )
	Shader* mShader = nullptr;
	std::vector<glm::vec3> mVec3;
	std::vector<glm::vec4> mVec4;
	std::vector<unsigned int> mIndices;
	glm::vec4 mColour;
	std::string mPVMName;
	std::string mColourName;
	unsigned int mVertexLocation = GL_INVALID_INDEX;
	unsigned int mVertexMode = GL_INVALID_ENUM;
	unsigned int mIndicesMode = GL_INVALID_ENUM;
	unsigned int mTexture = 0;
	SourceCallbackType mRenderCallback = nullptr;
	ResizeHelper::ResizeCallbackType mResizeCallback = nullptr;
	friend class VertexRenderer;
#pragma warning( pop )
};