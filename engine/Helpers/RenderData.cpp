#include "RenderData.h"

void GPUBufferObject::splice()
{
	mSplicedCalled = true;
	if (!unsplicedData.size())
		return;
	if (splicedData.size())
		throw NMSLogicException("GPUBufferObject::splice splicedData should be empty.\n");
	if (bufferStyle() == BufferStyle::Uniform)
	{
		return;
	}
	// Splice the individual elements data into one long array
	size_t instances = unsplicedData[0].data.size() / unsplicedData[0].span;
	size_t span = instanceSpan();
	for (int i = 0; i < instances; i++)
	{
		for (int f = 0; f < unsplicedData.size(); f++)
		{
			const UnSplicedData& ud = unsplicedData[f];
			size_t offset = i * ud.span;
			splicedData.insert(splicedData.end(), ud.data.begin() + offset, ud.data.begin() + offset + ud.span);
		}
	}

	for (int f = 0; f < unsplicedData.size(); f++)
	{
		UnSplicedData& ud = unsplicedData[f];
		ud.data.clear();
	}
}

unsigned int GPUBufferObject::instanceCount() const
{
	if (!mSplicedCalled)
	{
		// splice() has been not been called. We do not know if all the calls to populate the unspliced have been done.
		throw NMSLogicException("GPUBufferObject::instanceCount() Splice has not been called. No way to determine instanceCount.\n");
	}
	if (mBufferStyle == BufferStyle::SSBO)
	{
		if (splicedData.size())
		{
			return splicedData.size() / instanceSpan();
		}
		else
		{
			// splice() has been called but no data.
			return 0;
		}
	}
	else if (mBufferStyle == BufferStyle::Uniform)
	{
		if (splicedData.size())
		{
			throw NMSLogicException("GPUBufferObject::instanceCount(). Uniforms do not need to splice data.\n");
		}
		else
		{
			if (unsplicedData.size())
			{
				if (!unsplicedData[0].data.size())
				{
					// No data.
					return 0;
				}
				else
				{
					// Nothing special about the first index;
					return unsplicedData[0].data.size() / unsplicedData[0].span;
				}
			}
			else
			{
				// No data, this is ok;
				return 0;
			}
		}
	}
	else
	{
		throw NMSLogicException("GPUBufferObject::instanceCount() Unknown BufferStyle.\n");
	}
}

unsigned int GPUBufferObject::typeSize(BufferType t) const
{
	if (t == BufferType::Model)
		return 16;
	if (t == BufferType::Position)
		return 4;
	if (t == BufferType::Colour)
		return 4;
	if (t == BufferType::BillboardSize)
		return 2;
	if (t == BufferType::Padding2)
		return 2;
	if (t == BufferType::Padding1)
		return 1;
	else
		throw NMSLogicException("GPUBufferObject::typeSize Error. Unknown type ["
			+ std::to_string(static_cast<unsigned int>(t))
			+ "]\n");
}

void GPUBufferObject::setAllowedTypes(const std::vector<GPUBufferObject::BufferType>& _allowedTypes)
{
	if (unsplicedData.size())
		throw NMSLogicException("GPUBufferObject::unspliced must be empty before setting types\n");

	for (GPUBufferObject::BufferType t : _allowedTypes)
	{
		UnSplicedData ud;
		ud.dataType = t;
		ud.span = typeSize(t);
		unsplicedData.push_back(ud);
	}
}

GPUBufferObject::UnSplicedData& GPUBufferObject::findUnspliced(BufferType t)
{
	for (auto& ut : unsplicedData)
	{
		if (ut.dataType == t)
			return ut;
	}
	throw NMSLogicException("GPUBufferObject::findUnspliced() unsplicedData not found for ["
		+ std::to_string(static_cast<int>(t)) + "].\n");
}

void GPUBufferObject::append(const std::vector<glm::mat4>& _data, BufferType t)
{
	if (splicedData.size())
		throw NMSLogicException("GPUBufferObject::append(vector<mat4>) splicedData must be empty.\n");
	UnSplicedData& usd = findUnspliced(t);
	if (usd.locked)
		return;
	const float* ff_start = reinterpret_cast<const float*> (_data.data());
	constexpr int jfw = glm::mat4::length();
	const float* ff_end = ff_start + _data.size() * glm::mat4::length();
	usd.data.insert(usd.data.end(), ff_start, ff_end);
}

void GPUBufferObject::append(const std::vector<glm::vec4>& _data, BufferType t)
{
	if (splicedData.size())
		throw NMSLogicException("GPUBufferObject::append(vector<vec4>) splicedData must be empty.\n");
	UnSplicedData& usd = findUnspliced(t);
	if (usd.locked)
		return;
	const float* ff_start = reinterpret_cast<const float*> (_data.data());
	const float* ff_end = ff_start + _data.size() * 4;
	usd.data.insert(usd.data.end(), ff_start, ff_end);
}

void GPUBufferObject::append(const std::vector<glm::vec3>& _data, BufferType t)
{
	if (splicedData.size())
		throw NMSLogicException("GPUBufferObject::append(vector<vec3>) splicedData must be empty.\n");
	std::vector<glm::vec4> v4;
	for (const glm::vec3& v3 : _data)
	{
		v4.push_back(glm::vec4(v3, 0.0f));
	}
	append(v4, t);
}

void GPUBufferObject::append(const std::vector<glm::vec2>& _data, BufferType t)
{
	if (splicedData.size())
		throw NMSLogicException("GPUBufferObject::append(vector<vec2>) splicedData must be empty.\n");
	UnSplicedData& usd = findUnspliced(t);
	if (usd.locked)
		return;
	const float* ff_start = reinterpret_cast<const float*> (_data.data());
	const float* ff_end = ff_start + _data.size() * 2;
	usd.data.insert(usd.data.end(), ff_start, ff_end);
}

void GPUBufferObject::append(const glm::mat4& p, BufferType t)
{
	if (splicedData.size())
		throw NMSLogicException("GPUBufferObject::append(mat4) splicedData must be empty.\n");
	for (int i = 0; i < 4; i++)
	{
		const glm::vec4& v = p[i];
		append(v, t);
	}
}

void GPUBufferObject::append(const glm::vec4& p, BufferType t)
{
	if (splicedData.size())
		throw NMSLogicException("GPUBufferObject::append(vec4) splicedData must be empty.\n");
	UnSplicedData& usd = findUnspliced(t);
	if (usd.locked)
		return;
	const float* f = glm::value_ptr(p);
	usd.data.insert(usd.data.end(), f, f + 4);
}

void GPUBufferObject::append(const glm::vec3& p, BufferType t)
{
	if (splicedData.size())
		throw NMSLogicException("GPUBufferObject::append(vec3) splicedData must be empty.\n");
	glm::vec4 p4 = glm::vec4(p, 0);
	append(p4, t);
}

void GPUBufferObject::append(const glm::vec2& p, BufferType t)
{
	if (splicedData.size())
		throw NMSLogicException("GPUBufferObject::append(vec2) splicedData must be empty.\n");
	glm::vec4 p4 = glm::vec4(p, 0, 0);
	append(p4, t);
}

void GPUBufferObject::append(float f, BufferType t)
{
	if (splicedData.size())
		throw NMSLogicException("GPUBufferObject::append(float) splicedData must be empty.\n");
	UnSplicedData& usd = findUnspliced(t);
	usd.data.push_back(f);
}

void OWRenderData::add(const OWRenderData& toAdd, bool purgeTextures)
{
	/*
	if (ssbo.data() != nullptr)
	{
		throw NMSLogicException("OWRenderData::add(). Cannot delete a populated and spliced SSBO.");
	}
	else if (ssbo.instanceSize() == 0)
	{
		// Replace the empty ssbo
		ssbo = toAdd.ssbo;
	}
	else
	{
		// ssbo has unspliced data
		if (toAdd.ssbo.instanceSize())
		{
			throw NMSLogicException("OWRenderData::add(). Both ssbo's have unspliced data.");
		}
		else
		{
			// Do nothing. toAdd.ssbo is empty
		}
	}
	*/
	meshes.insert(meshes.end(), toAdd.meshes.begin(), toAdd.meshes.end());
	models.insert(models.end(), toAdd.models.begin(), toAdd.models.end());
	if (purgeTextures)
	{
		// Remove identical textures
		size_t startNdx = 0;
		if (!textures.size() && toAdd.textures.size())
		{
			textures.push_back(toAdd.textures[startNdx++]);
		}
		for (size_t i = startNdx; i < toAdd.textures.size(); i++)
		{
			const Texture& tex = toAdd.textures[i];
			if (tex.samplerName() == textures.back().samplerName())
				continue;
			textures.push_back(tex);
		}
	}
	else
	{
		textures.insert(textures.end(), toAdd.textures.begin(), toAdd.textures.end());
	}
}

void GPUBufferObject::validate(const std::string& ownerName) const
{
	/*
	* before setup then splicedData should be empty and unspliced
	* should have stuff. After setup then the reverse
	*/
	if (splicedData.size() && unsplicedData.size())
		throw NMSLogicException("GPUBufferObject::validate failed\n");
}

unsigned int GPUBufferObject::instanceSpan() const
{
	unsigned int retval = 0;
	for (const auto& elm : unsplicedData)
	{
		retval += elm.span;
	}
	return retval;
}

unsigned int GPUBufferObject::populatedUnSplicedArrayCount() const
{
	unsigned int retval = 0;
	for (const auto& elm : unsplicedData)
	{
		if (elm.data.size() > 0)
			retval++;
	}
	return retval;
}

void GPUBufferObject::lock(const std::vector<GPUBufferObject::BufferType>& orderedTypes)
{
	for (const auto& t : orderedTypes)
	{
		GPUBufferObject::UnSplicedData& found = findUnspliced(t);
		found.locked = true;
	}
}

bool GPUBufferObject::locked(GPUBufferObject::BufferType t) const
{
	for (const auto& elm: unsplicedData)
	{
		if (elm.dataType == t)
			return elm.locked;
	}
	// Also could be treates as an error
	return true;
}

bool GPUBufferObject::dataExists(BufferStyle bs) const
{
	if (bs == BufferStyle::SSBO)
		return splicedData.size() > 0;
	else if (bs == BufferStyle::Uniform)
		return (unsplicedData.size() > 0 && unsplicedData[0].data.size() > 0);
	else
	{
		throw NMSLogicException("GPUBufferObject::gotRenderableData() Unknown BufferStyle.\n");
	}

}


bool GPUBufferObject::exists(GPUBufferObject::BufferType ss) const
{
	for (int i = 0; i < unsplicedData.size(); i++)
	{
		if (unsplicedData[i].dataType == ss)
			return true;
	}
	return false;
}

AABB OWRenderData::bounds() const
{
	AABB retval = AABB(0);
	for (const auto& m : meshes)
	{
		retval = retval | m.bounds();
	}
	for (const auto& m : models)
	{
		retval = retval | m.bounds();
	}
	return retval;
}
