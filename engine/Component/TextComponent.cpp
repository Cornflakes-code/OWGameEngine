#include "TextComponent.h"

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/GlobalSettings.h"
#include "../Helpers/Shader.h"
#include "../Core/ErrorHandling.h"

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
