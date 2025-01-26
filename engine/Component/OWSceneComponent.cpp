#include "OWSceneComponent.h"

#include "../Geometry/Ray.h"

OWSceneComponent::OWSceneComponent(OWActor* _owner, OWSceneComponentData* _data)
	: OWComponent(_owner)
{
	setData(_data);
}

bool OWSceneComponent::canCollide()
{
	return true;
}

bool OWSceneComponent::canCollide(OWCollisionData* other) 
{
	if (other->component == this)
		return false;
	// called before testing for couldCollide
	return true;

}
bool OWSceneComponent::collides(OWCollisionData* other)
{
	return false;
}

void OWSceneComponent::collided(OWCollisionData* other) 
{
	// https://gamedev.stackexchange.com/questions/47888/find-the-contact-normal-of-rectangle-collision?noredirect=1&lq=1
	const glm::vec3 otherRelVel = other->component->data()->velocity;
	glm::vec3 relVel = data()->velocity - otherRelVel;
	glm::vec3 compoundSize(data()->boundingBox.size() + other->boundingBox.size());
	AABB compoundAABB(compoundSize);
	glm::vec3 jfw = data()->boundingBox.center();
	compoundAABB.moveTo(data()->boundingBox.center());
	glm::vec3 normal1, normal2;
	float distance1, distance2;

	OWRay r1(data()->boundingBox.center(), relVel);
	if (!r1.intersects(data()->boundingBox, normal1, distance1))
	{
		return;
	}
	OWRay r31(data()->boundingBox.center(), relVel);
	if (!r1.intersects(compoundAABB, normal2, distance2))
	{
		return;
	}
	// Various collision detection algorythmns
	// https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-32-broad-phase-collision-detection-cuda
	// https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
	glm::vec3 v = data()->velocity + other->component->data()->velocity;
	glm::vec3 normal;
	float distance;
	if (glm::all(glm::epsilonEqual(v, glm::vec3(0), OWUtils::epsilon())))
		return;
	glm::vec3 c1 = other->component->data()->boundingBox.minPoint() - glm::vec3(2, 2, 2);
	OWRay r2(data()->boundingBox.center(), v);
	if (!r2.intersects(other->component->data()->boundingBox, normal, distance))
	{
		return;
		// throw NMSLogicException("Object [" + name() + "] collided by ray intersection failed");
	}
	v = glm::normalize(v);
	// https://stackoverflow.com/questions/573084/how-to-calculate-bounce-angle
	// https://3dkingdoms.com/weekly/weekly.php?a=2
	constexpr float notPerfectBounce = 1.0f; // A perfect bounce
	glm::vec3 reboundDir = notPerfectBounce * (v - 2 * glm::dot(v, normal) * normal);
	glm::vec3 ourCenter = data()->boundingBox.center();
	glm::vec3 otherCenter = other->component->data()->boundingBox.center();
	//float dist = glm::length(ourCenter - otherCenter);
	//float fullTimeStep = glm::length(previousPosition() - position()) / glm::length(mCurrent.mVelocity);
	//float curtailedTimeStep = glm::length(distance) / glm::length(mCurrent.mVelocity);
	// jfw prorataDistance is wrong.
	float len = glm::length(ourCenter - otherCenter);
	//float len2 = glm::length(ourCenter - position());
	float prorataTimeStep = distance / len;// curtailedTimeStep / fullTimeStep;
	data()->position += prorataTimeStep * glm::length(data()->velocity) * glm::normalize(reboundDir);
	data()->velocity += reboundDir * glm::length(data()->velocity);
}

void OWSceneComponent::render(const glm::mat4& proj,
	const glm::mat4& view,
	const glm::mat4& model,
	const glm::vec3& cameraPos,
	RenderTypes::ShaderMutator renderCb,
	RenderTypes::ShaderResizer resizeCb) 
{
	const glm::mat4 I(1.0f);
	//glm::mat4 r = glm::rotate(model, mRotateRadians, mRotateAxis);
	//glm::mat4 s = glm::scale(model, mScale);
	//glm::mat4 t = glm::translate(I, position());
	glm::mat4 _model;// = t * r * s;
	mRenderer->render(proj, view, _model, cameraPos, renderCb, resizeCb);

}

