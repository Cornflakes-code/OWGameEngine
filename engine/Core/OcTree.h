#pragma once

#include <functional>
#include <vector>

#include "BoundingBox.h"

// This has some seriously good links
// https://stackoverflow.com/questions/5963954/fast-templated-c-octree-implementation

// https://github.com/OpenGL-Graphics/octree/blob/master/src/main.cpp

// https://stackoverflow.com/questions/12191802/use-octree-to-organize-3d-volume-data-in-gpu

// This code converted from the C# code found at
//https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/introduction-to-octrees-r3529/

// This class copied from 
// https://www.flipcode.com/archives/Octree_Implementation.shtml

// -----------------------------------------------------------------------------
// This defines a callback for traversal
// -----------------------------------------------------------------------------

class OcTree;
class Actor;

typedef std::function<bool(OcTree* o)> OctreeCallbackType;

class OWENGINE_API OcTree
{
public:
    OcTree();
    virtual ~OcTree();

    const std::vector<Actor*> points() const { return mPoints; }

    void build(const std::vector<Actor*>& points,
        unsigned int threshold,
        unsigned int maximumDepth,
        const AABB& bounds,
        unsigned int currentDepth = 0);
    bool traverse(OctreeCallbackType proc);
//protected:
public:
    OcTree* mChildren[8];
    std::vector<Actor*> mPoints;
    AABB mBounds;
    unsigned int mDepth = 0;
};
