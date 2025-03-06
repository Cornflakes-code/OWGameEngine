#include "TextComponent.h"

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/GlobalSettings.h"
#include "../Helpers/Shader.h"
#include "../Core/ErrorHandling.h"
#include "../Helpers/FontFactory.h"

OWTextComponent::OWTextComponent(OWActor* _owner, const std::string& _name,
	const OWTextComponentData& _data)
	: OWMeshComponentBase(_owner, _name), mData(_data)
{
}


OWTextComponent::OWTextComponent(OWActor* _owner, const std::string& _name,
	const std::string& textFileName)
	: OWMeshComponentBase(_owner, _name)
{
	// Load mData from textFileName;
}

AABB adjustPosition(std::vector<glm::vec4>& v4, unsigned int mReferencePos)
{
	AABB bounds(v4);

	glm::vec4 displacement = glm::vec4(0);
	if (mReferencePos & OWTextComponentData::PositionType::Left)
	{
		displacement.x = -bounds.minPoint().x;
	}
	else if (mReferencePos & OWTextComponentData::PositionType::Right)
	{
		displacement.x = -bounds.maxPoint().x;
	}
	else
	{
		displacement.x = -bounds.center().x;
	}
	if (mReferencePos & OWTextComponentData::PositionType::Bottom)
	{
		displacement.y = -bounds.minPoint().y;
	}
	else if (mReferencePos & OWTextComponentData::PositionType::Right)
	{
		displacement.y = -bounds.maxPoint().y;
	}
	else
	{
		displacement.y = -bounds.center().y;
	}
	for (auto& v : v4)
	{
		v += displacement;
	}
	bounds.move(displacement);
	return bounds;
}

void OWTextComponent::prepareMutators()
{
	if (mData.tdt == OWTextComponentData::TextDisplayType::Dynamic)
		mutator([](const glm::mat4& proj, const glm::mat4& view,
			const glm::mat4& model, const glm::vec3& cameraPos,
			const Shader* shader)
			{
				glm::vec3 CameraRight_worldspace = { view[0][0], view[1][0], view[2][0] };
				shader->setVector3f("CameraRight_worldspace", CameraRight_worldspace);
				glm::vec3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };
				shader->setVector3f("CameraUp_worldspace", CameraUp_worldspace);
				shader->setVector3f("BillboardPos", model[3]);
			}
		);
	else if (mData.tdt == OWTextComponentData::TextDisplayType::Static)
	{
		mutator([](const glm::mat4& proj, const glm::mat4& view,
			const glm::mat4& model, const glm::vec3& cameraPos, const Shader* shader)
			{
				shader->setVector3f("BillboardPos", model[3]);
			}
		);
	}
	else
	{
		throw NMSLogicException("Error: Unkown OWTextComponentData::TextDisplayType.");
	}
}

const std::vector<OWMeshData> OWTextComponent::simpleMesh(AABB& bounds) const
{
	OWMeshData md;

	const FreeTypeFontAtlas::FontDetails* fontData
		= FontFactory().loadFreeTypeFont(mData.fontName, mData.fontHeight);
	md.meshData.v4 = fontData->createText(mData.text, mData.fontSpacing.x, mData.fontSpacing.y);
	if (md.meshData.v4.empty())
	{
		throw NMSLogicException(std::stringstream()
			<< "No Triangles generated for Text ["
			<< mData.text << "] is empty\n");
	}
	bounds = adjustPosition(md.meshData.v4, mData.referencePos);
	md.meshData.setColour(mData.colour, "textcolor");
	std::vector<OWMeshData> retval;
	retval.push_back(md);
	return retval;
}


void OWTextComponent::doSetup()
{
	prepareMutators();
	OWTextComponentData& td = mData;

	// If both left and right then remove them
	if (((td.referencePos & OWTextComponentData::PositionType::Left) &&
		(td.referencePos & OWTextComponentData::PositionType::Right)))
	{
		td.referencePos = OWTextComponentData::PositionType(td.referencePos & 0xC);
	}

	// If both Top and Bottom then remove them
	if (((td.referencePos & OWTextComponentData::PositionType::Top) &&
		(td.referencePos & OWTextComponentData::PositionType::Bottom)))
	{
		td.referencePos = OWTextComponentData::PositionType(td.referencePos & 0x3);
	}
}

AABB calcOrthogonalAABB(const glm::vec3& right, const glm::vec3& up,
	const glm::vec3& inOut, const AABB& orig, float distanceToEye)
{
	if (OWUtils::isZero(distanceToEye))
		distanceToEye = 0.001f;
	float dist = 20000 / distanceToEye;
	std::vector<glm::vec3> v;
	glm::vec3 sz = orig.size() * 0.5f; // mult by half extent.
	v.push_back(sz * right); //rhs
	v.push_back(-sz * right); // lhs
	v.push_back(sz * up); // up
	v.push_back(-sz * up); // down
	v.push_back(-sz * inOut); // in
	v.push_back(sz * inOut); // out
	for (glm::vec3& a : v)
	{
		a.x *= dist;
		a.y *= dist;
	}
	AABB bb(v);
	return bb;
}
