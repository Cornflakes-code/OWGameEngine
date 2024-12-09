#include "OcTree.h"
#include <bitset>

// See also
// https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det

Octree::Octree()
{
}

Octree::~Octree()
{
}

void Octree::build(const std::vector<Physical*>& points,
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

    if (points.size() <= threshold || currentDepth >= maximumDepth)
    {
        mPoints = points;
        return;
    }

    std::vector<std::vector<Physical*>> childPoints(8);
    // Center of this node
    glm::vec3 c = bounds.center();

    // Classify each point to a child node
    for (Physical* p : points)
    {
        // Current point

        // Push the objects into the relevant child bucket. The bucTo
        // do this, we build an index into the child bucket using the
        // relative position of the point to the center of the current
        // node
        const glm::vec3 pc = p->bounds().center();
        // 'p' can be in multiple areas
        // If a part of Physical is in an area then add it to the area.
        // Y
        // |  _________
        // | / 6 /  7 / |
        // |/---/----/|7|
        // | 2 | 3 |3|/|
        // |---|---|/|5/
        // | 0 | 1 |1|/___X
        // 
        
        std::vector<char> code(1);
        // If all of p->bounds is > than the center then p cannot be in bins 0, 2, 4, 6
        if (p->bounds().minPoint().x > c.x)
            code[0] = code[2] = code[4] = code[6] = 0;
        // If all of p->bounds is < than the center p then cannot be in bins 1, 3, 5, 7
        if (p->bounds().maxPoint().x < c.x)
            code[1] = code[3] = code[5] = code[7] = 0;

        // ... and same logic for y and z.
        if (p->bounds().minPoint().y > c.y)
            code[2] = code[3] = code[6] = code[7] = 0;
        if (p->bounds().maxPoint().y < c.y)
            code[0] = code[1] = code[4] = code[5] = 0;

        if (p->bounds().minPoint().z > c.z)
            code[0] = code[1] = code[2] = code[3] = 0;
        if (p->bounds().maxPoint().z < c.z)
            code[4] = code[5] = code[6] = code[7] = 0;

        for (unsigned int i = 0; i < 8; i++)
        {
            if (code[i])
                childPoints[i].push_back(p);
        }
    }

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
    for (unsigned int i = 0; i < childPoints.size(); i++)
    {
        // Don't bother going any further if there aren't any points for
        // this child

        if (!childPoints[i].size()) 
            continue;

        glm::vec3 minPoint = boundsOffsetTable[i] * bounds.extent();
        glm::vec3 maxPoint = minPoint + bounds.extent();
        AABB childBounds(minPoint, maxPoint);

        // Allocate the child
        mChildren[i] = new Octree;

        mChildren[i]->build(childPoints[i], threshold, maximumDepth,
            childBounds, currentDepth + 1);
    }
}
const bool Octree::traverse(callback proc, void* data) const
{
    // Call the callback for this node (if the callback returns false, then
    // stop traversing.

    if (!proc(*this, data)) return false;

    // If I'm a node, recursively traverse my children
    if (!mPoints.size())
    {
        for (unsigned int i = 0; i < 8; i++)
        {
            // We store incomplete trees (i.e. we're not guaranteed
            // that a node has all 8 children)
            if (!mChildren[i]) 
                continue;

            if (!mChildren[i]->traverse(proc, data)) 
                return false;
        }
    }

    return true;
};
