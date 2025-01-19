#include "OcTree.h"
#include <bitset>

#include "OWActor.h"

// See also
// https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det

OcTree::OcTree()
{
}

OcTree::~OcTree()
{
}

void OcTree::add(OWMovableComponent* mc, unsigned int threshold,
    unsigned int maximumDepth)
{
    std::vector<OWMovableComponent*> toAdd;
    toAdd.push_back(mc);
}

void OcTree::add(const std::vector<OWMovableComponent*>& toAdd, unsigned int threshold,
    unsigned int maximumDepth)
{
    build(toAdd, threshold, maximumDepth, mBounds, 0);
}

const AABB negative(glm::vec3(-1), glm::vec3(-1));

void OcTree::addToBin(OWMovableComponent* a, std::vector<std::vector<OWMovableComponent*>>& childBin)
{
    // Center of this node
    glm::vec3 center = mBounds.center();

    // Current point

    // Push the objects into the relevant child bucket. The bucTo
    // do this, we build an index into the child bucket using the
    // relative position of the point to the center of the current
    // node
    if (a->bounds() == negative)
    {
        throw NMSLogicException("Bounds for particle not set.");
    }
    const glm::vec3 pc = a->bounds().center();
    // 'p' can be in multiple areas
    // If a part of Actor is in an area then add it to the area.
    // Y
    // |  _________
    // | / 6 /  7 / |
    // |/---/----/|7|
    // | 2 | 3 |3|/|
    // |---|---|/|5/
    // | 0 | 1 |1|/___X
    // 

    std::vector<int> code(8, 1);
    // If all of a->bounds is > than the center then p cannot be in bins 0, 2, 4, 6
    if (a->bounds().minPoint().x > center.x)
        code[0] = code[2] = code[4] = code[6] = 0;
    // If all of a->bounds is < than the center p then cannot be in bins 1, 3, 5, 7
    if (a->bounds().maxPoint().x < center.x)
        code[1] = code[3] = code[5] = code[7] = 0;

    // ... and same logic for y and z.
    if (a->bounds().minPoint().y > center.y)
        code[2] = code[3] = code[6] = code[7] = 0;
    if (a->bounds().maxPoint().y < center.y)
        code[0] = code[1] = code[4] = code[5] = 0;

    if (a->bounds().minPoint().z > center.z)
        code[0] = code[1] = code[2] = code[3] = 0;
    if (a->bounds().maxPoint().z < center.z)
        code[4] = code[5] = code[6] = code[7] = 0;

    for (unsigned int i = 0; i < 8; i++)
    {
        if (code[i])
            childBin[i].push_back(a);
    }
}

void OcTree::addBinToChildren(const std::vector<std::vector<OWMovableComponent*>>& bin,
                                unsigned int threshold, unsigned int maximumDepth,
                                const AABB& bounds, unsigned int currentDepth)
{
    const glm::vec3 boundsOffsetTable[8] =
    {
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {1, 1, 0},
        {0, 0, 1},
        {1, 0, 1},
        {0, 1, 1},
        {1, 1, 1}
    };

    // Recursively call build() for each of the 8 children
    for (unsigned int i = 0; i < bin.size(); i++)
    {
        // Don't bother going any further if there aren't any points for this child
        if (!bin[i].size())
            continue;

        glm::vec3 minPoint = boundsOffsetTable[i] * bounds.extent();
        glm::vec3 maxPoint = minPoint + bounds.extent();
        AABB childBounds(minPoint, maxPoint);

        // Allocate the child
        mChildren[i] = new OcTree;

        mChildren[i]->build(bin[i], threshold, maximumDepth,
            childBounds, currentDepth + 1);
    }
}

void OcTree::build(const std::vector<OWMovableComponent*>& points,
    unsigned int threshold,
    unsigned int maximumDepth,
    const AABB& bounds,
    unsigned int currentDepth)
{
    mBounds = bounds;
    mDepth = currentDepth;
    // You know you're a leaf when...
    //
    // 1. The number of points is <= the threshold
    // 2. We've recursed too deep into the tree
    //    (currentDepth >= maximumDepth)
    //
    //    NOTE: We specifically use ">=" for the depth comparison so that we
    //          can set the maximumDepth depth to 0 if we want a tree with
    //          no depth.

    if (!mPoints.size() && 
        (points.size() <= threshold || currentDepth >= maximumDepth))
    {
        mPoints = points;
        return;
    }

    std::vector<std::vector<OWMovableComponent*>> bin(8);

    for (OWMovableComponent* a : points)
        addToBin(a, bin);
    addBinToChildren(bin, threshold, maximumDepth, bounds, currentDepth + 1);
}

bool OcTree::traverse(OctreeCallbackType proc)
{
    // Call the callback for this node (if the callback returns false, then
    // stop traversing.

    if (!proc(this)) return false;

    // If I'm a node, recursively traverse my children
    if (!mPoints.size())
    {
        for (unsigned int i = 0; i < 8; i++)
        {
            // We store incomplete trees (i.e. we're not guaranteed
            // that a node has all 8 children)
            if (!mChildren[i]) 
                continue;

            if (!mChildren[i]->traverse(proc)) 
                return false;
        }
    }

    return true;
};
