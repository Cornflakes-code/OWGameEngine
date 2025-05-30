#pragma once

#include <vector>

#include "../OWEngine/OWEngine.h"
#include "../Actor/ColliderBounds.h"

class OWCollider;
class OWRay;
class OWActor;

class OWENGINE_API OWCollisionSystem
{
public:
	OWCollisionSystem(Scene* _scene);
	void addCollider(OWCollider* coll, OWActor* a, OWSize componentId);
	void addRay(OWCollider* coll, OWActor* a, OWSize componentId);
	void deleteRay(OWActor* r);

    void sort();
    void collide();

private:
    class BodyEx
    {
    public:
        std::string id;
        BodyEx(const OWCollider* _o)
            :o(_o) {}
        const OWCollider* o;
        glm::vec3 left() const { return o->left(); }
        glm::vec3 right() const { return o->right(); }
    };

    struct BodyPairEx
    {
        BodyEx b1;
        BodyEx b2;
        friend auto operator<=>(const BodyPairEx& lhs, const BodyPairEx& rhs)
        {
            if (lhs.b1.o == rhs.b1.o)
            {
                if (lhs.b2.o == rhs.b2.o)
                    return 0;
                return lhs.b2.o->componentIndex() < rhs.b2.o->componentIndex() ? -1 : 1;
            }
            return lhs.b1.o->componentIndex() < rhs.b1.o->componentIndex() ? -1 : 1;
        }
    };

    typedef std::set<BodyPairEx> BodyCollisionsEx;
    typedef std::vector<BodyEx> SortedBodiesEx;
    typedef std::set<BodyPairEx> BodyCollisionsEx;

    glm::uvec3 gPriorityAxis = { 0,1,2 };
    SortedBodiesEx bodies;

    Scene* mScene = nullptr;
	OWColliderBounds mVisualBounds;
	void buildSweepAndPrune(OWCollider* o);
    void doInsertionSortEx(SortedBodiesEx& edges, unsigned int primAxis);
    void collideSweepAndPruneEx(std::vector<BodyEx>& sortedBodies, const glm::uvec3& priAxis, BodyCollisionsEx& collisions);
};
