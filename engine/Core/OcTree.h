#pragma once

#include <vector>
#include "BoundingBox.h"
#include "Particle.h"

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

class   Octree;
typedef bool (*callback)(const Octree& o, void* data);

class Octree
{
public:
    Octree();
    virtual ~Octree();

    const std::vector<Physical*> points() const { return mPoints; }

    void build(const std::vector<Physical*>& points,
        unsigned int threshold,
        unsigned int maximumDepth,
        const AABB& bounds,
        unsigned int currentDepth = 0);
    const bool traverse(callback proc, void* data) const;
protected:
    Octree* mChildren[8];
    std::vector<Physical*> mPoints;
    AABB mBounds;
    unsigned int mDepth = 0;
};
