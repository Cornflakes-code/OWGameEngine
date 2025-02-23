#pragma once

#include <functional>
#include <vector>

#include "../Geometry/BoundingBox.h"


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
struct OLDCollisionData;

typedef std::function<bool(OcTree* o)> OctreeCallbackType;

class OWENGINE_API OcTree
{
public:
    OcTree();
    virtual ~OcTree();

    const std::vector<OLDCollisionData*> points() const { return mPoints; }
    void add(const std::vector<OLDCollisionData*>&, unsigned int threshold,
        unsigned int maximumDepth);
    void add(OLDCollisionData* mc, unsigned int threshold,
        unsigned int maximumDepth);
    void build(const std::vector<OLDCollisionData*>& points,
        unsigned int threshold,
        unsigned int maximumDepth,
        const AABB& bounds,
        unsigned int currentDepth = 0);
    bool traverse(OctreeCallbackType proc);
//protected:
public:
    std::vector<OcTree*> mChildren = std::vector<OcTree*>(8, nullptr);
    std::vector<OLDCollisionData*> mPoints;
    AABB mBounds;
    unsigned int mDepth = 0;
private:
    void addToBin(OLDCollisionData* a, std::vector<std::vector<OLDCollisionData*>>& childBin);
    void addBinToChildren(const std::vector<std::vector<OLDCollisionData*>>& bin,
        unsigned int threshold, unsigned int maximumDepth,
        const AABB& bounds, unsigned int currentDepth);
};
