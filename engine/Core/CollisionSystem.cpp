#include "CollisionSystem.h"

#include <set>
#include <algorithm>

#include "../Actor/OWActor.h"
#include "../Helpers/Collider.h"
#include "../Core/LogStream.h"

// spring mass system hookes law
// Soft body bouncing pv = nRT (ideal gas law
// p = F / A
// => F = A * c/v

// collisions - explicit Euler method 
// vel += accell * deltaT
// pos += vel * deltaT
// Runge - Kutta 4


// Awesome series of posts about optimised collision systems
// https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det
// https://www.reddit.com/r/gameenginedevs/comments/jp30c6/efficient_and_well_explained_implementation_of_a/
// This looks very good
// https://github.com/loosegrid/DragonSpace-Demo

// https://github.com/bepu/bepuphysics2/blob/master/Documentation/ContinuousCollisionDetection.md
// Good discussion about different types of collision optimisations
// https://www.gamedev.net/forums/topic/328022-what-collision-method-is-better/
// https://leanrada.com/notes/sweep-and-prune-2/#final-code
// http://www.codercorner.com/SAP.pdf
// 
// https ://stackoverflow.com/questions/47466358/what-is-the-spaceship-three-way-comparison-operator-in-c
// 
// https://github.com/bepu/bepuphysics2/blob/master/Documentation/ContinuousCollisionDetection.md

// https://gamedev.stackexchange.com/questions/211322/sweep-and-prune-algorithm-performance
// https://github.com/YulyaLesheva/Sweep-And-Prune-algorithm/blob/main/SAP_algorithm.cpp
// https://github.com/grynca/SAP/blob/master/include/SAP/SAP_internal.h
// https://leanrada.com/notes/sweep-and-prune-2/
// https://www.youtube.com/watch?v=MKeWXBgEGxQ
// Seriously good post about fixed sized grids
// https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det

OWCollisionSystem::OWCollisionSystem(Scene* _scene)
    :mVisualBounds(_scene, "AABB Bounds")
{
}

void OWCollisionSystem::addCollider(OWCollider * coll, OWActor * a, OWSize componentId)
{
    buildSweepAndPrune(coll);
}

void OWCollisionSystem::addRay(OWCollider * coll, OWActor * a, OWSize componentId)
{
}

void OWCollisionSystem::deleteRay(OWActor * r)
{
}

void OWCollisionSystem::sort()
{
    static bool once = true;
    if (true)
    {
        once = false;
        //std::sort(bodiesX.begin(), bodiesX.end(), sortfunctionX);
        //std::sort(bodiesY.begin(), bodiesY.end(), sortfunctionY);
        //std::sort(bodiesZ.begin(), bodiesZ.end(), sortfunctionZ);
        //doInsertionSort(bodiesX);
        //doInsertionSort(bodiesY);
        //doInsertionSort(bodiesZ);

        doInsertionSortEx(bodies, gPriorityAxis[0]);
    }
}
void OWCollisionSystem::collide()
{
    BodyCollisionsEx collisions;
    collideSweepAndPruneEx(bodies, gPriorityAxis, collisions);
}

void OWCollisionSystem::doInsertionSortEx(SortedBodiesEx& edges, unsigned int primAxis)
{
    // Plain Insertion sort
    for (int i = 1; i < edges.size(); i++)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            if (edges[j].left()[primAxis] < edges[j + 1].left()[primAxis])
                break;

            // Swap
            std::swap(edges[j], edges[j + 1]);
        }
    }
}

void OWCollisionSystem::buildSweepAndPrune(OWCollider* o)
{
    bodies.push_back(BodyEx(o));
}

void OWCollisionSystem::collideSweepAndPruneEx(std::vector<BodyEx>& sortedBodies, const glm::uvec3& priAxis, BodyCollisionsEx& collisions)
{
    unsigned int primary = priAxis[0];
    unsigned int secondary = priAxis[1];
    unsigned int tertiary = priAxis[2];
    for (int i = 0; i < sortedBodies.size(); i++)
    {
        const BodyEx& b1 = sortedBodies[i];
        float b1l = b1.left()[primary];
        float b1r = b1.right()[primary];
        for (int j = i + 1; j < sortedBodies.size(); j++)
        {
            const BodyEx& b2 = sortedBodies[j];
            float b2l = b2.left()[primary];
            float b2r = b2.right()[primary];
            if (b2.left()[primary] > b1.right()[primary])
                break;
            if (b2.left()[primary] > b1.left()[primary])
            {
                // Overlaps in the primary axis ...
                if ((b2.left()[secondary] > b1.right()[secondary]) ||
                    (b2.right()[secondary] < b1.left()[secondary]))
                    break;
                // ... and Overlaps in the secondary axis ...
                if ((b2.left()[tertiary] > b1.right()[tertiary]) ||
                    (b2.right()[tertiary] < b1.left()[tertiary]))
                    break;
                // ... and Overlaps in the tertiary axis
                collisions.insert({ b1, b2 });
            }
        }
    }
}
