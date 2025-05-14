#pragma once
#include <vector>
#include <set>

#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "../Geometry/BoundingBox.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shader Storage Buffer Object
struct GPUBufferObject
{
	enum BufferStyle
	{
		SSBO, // mutable, the default
		Uniform // Immutable
	};
	enum BufferType
	{
		Model, // mat4 
		Position, // vec4
		Colour, // vec4
		BillboardSize, // vec4
		Anything, // vec4
		NoType
	};

	void append(const std::vector<glm::mat4>& _data, BufferType t);
	void append(const std::vector<glm::vec4>& _data, BufferType t);
	void append(const glm::mat4& p, BufferType t);
	void append(const glm::vec4& p, BufferType t);
	// Padded to four bytes
	void append(const glm::vec3& p, BufferType t);
	// Padded to four bytes
	void append(const glm::vec2& p, BufferType t);
	// Padded to four bytes
	void append(float f, BufferType t);
	void setAllowedTypes(const std::vector<GPUBufferObject::BufferType>& _allowedTypes);

	void splice();
	bool exists(GPUBufferObject::BufferType ss) const;
	void validate(const std::string& ownerName) const;
	unsigned int instanceSpan() const;
	unsigned int populatedUnSplicedArrayCount() const;
	unsigned int instanceCount() const;

	// Do not allow any more additions for these types
	void lock(const std::vector<GPUBufferObject::BufferType>& orderedTypes);
	bool locked(GPUBufferObject::BufferType t) const;
	void bufferStyle(BufferStyle newValue) { mBufferStyle = newValue; }
	BufferStyle bufferStyle() const { return mBufferStyle; }
	bool dataExists(BufferStyle bs) const;
	void updateData(float* data, BufferType bt, OWSize ndx);
	void setWriteBuffer(void* buf)
	{
		mWriteBuffer = static_cast<char8_t*>(buf);
#ifdef _DEBUG
		ff = reinterpret_cast<float*>(mWriteBuffer);
#endif
	}
#ifdef _DEBUG
	float* ff = nullptr;
#endif
	void shaderBinding(unsigned int newValue) {
		mShaderBinding = newValue;
	}
	unsigned int shaderBinding() const {
		return mShaderBinding;
	}
private:
	void updateSplicedData(float* data, BufferType bt, OWSize ndx);
	void updateUnsplicedData(float* data, BufferType bt, OWSize ndx);
	unsigned int mShaderBinding = 1;
	char8_t* mWriteBuffer = nullptr;
	bool mLocked = false;
	bool mSplicedCalled = false;
	BufferStyle mBufferStyle = BufferStyle::SSBO;
	struct UnSplicedData
	{
		std::vector<float> data;
		unsigned int span = 0;
		bool locked = false;
		BufferType dataType = BufferType::NoType;
	};
	unsigned int typeSize(BufferType t) const;
	UnSplicedData& findUnspliced(BufferType t);
public:
	std::vector<UnSplicedData> unsplicedData;
	std::vector<float> splicedData;
};

class OWRenderData
{
public:
	std::vector<MeshData> meshes;
	std::vector<OWModelData> models;
	std::vector<Texture> textures;
	void add(const OWRenderData& toAdd, bool purgeTextures = false);
	AABB bounds() const;
};