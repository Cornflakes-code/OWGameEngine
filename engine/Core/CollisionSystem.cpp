#include "CollisionSystem.h"

#include <set>
#include <algorithm>
#include "OWActor.h"
#include "OWMovableComponent.h"
#include "../Geometry/Box.h"
#include "../Core/LogStream.h"

#define SWEEP_AND_PRUNE
//#define BASIC_COLLISIONS


namespace CollisionSystem
{
#ifdef SWEEP_AND_PRUNE
	// https://leanrada.com/notes/sweep-and-prune-2/#final-code
	struct Edge
	{
		Edge(OWMovableComponent* _o, unsigned int _ndx, bool _isLeft)
			:o(_o), ndx(_ndx), isLeft(_isLeft)
		{ }
		OWMovableComponent* o;
		mutable float jfw;
		unsigned int ndx;
		bool isLeft;
		float val() const
		{
			jfw = isLeft ? o->bounds().minPoint()[ndx] : o->bounds().maxPoint()[ndx];
			return jfw;
		}
	};

	struct EdgePair
	{
		const Edge e1;
		const Edge e2;
		EdgePair(const Edge& _e1, const Edge& _e2)
			:e1(_e1), e2(_e2)
		{
		}
	};
	struct EdgePairSorter
	{
		bool operator()(const EdgePair& lhs, const EdgePair& rhs) const
		{
			if (lhs.e1.o == rhs.e1.o)
				return lhs.e2.o < rhs.e2.o;
			return lhs.e1.o < rhs.e1.o;
		}
	};
	typedef std::set<EdgePair, EdgePairSorter> OverlappingEdges;
	std::vector<Edge> gEdgesX;
	std::vector<Edge> gEdgesY;
	std::vector<Edge> gEdgesZ;
	// https://github.com/bepu/bepuphysics2/blob/master/Documentation/ContinuousCollisionDetection.md

	// https://gamedev.stackexchange.com/questions/211322/sweep-and-prune-algorithm-performance
	// https://github.com/YulyaLesheva/Sweep-And-Prune-algorithm/blob/main/SAP_algorithm.cpp
	// https://github.com/grynca/SAP/blob/master/include/SAP/SAP_internal.h
	void sweepAndPrune(std::vector<Edge>& edges, OverlappingEdges& overlapping, bool firstTimeCalled)
	{
		// Insertion sort
		for (int i = 1; i < edges.size(); i++)
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (edges[j].val() < edges[j + 1].val())
					break;

				// Swap
				Edge temp = edges[j];
				edges[j] = edges[j + 1];
				edges[j + 1] = temp;

				// --- Code up until this point is plain insertion sort ---

				// These two edges have just swapped places, process it...
				const Edge& edge1 = edges[j];
				const Edge& edge2 = edges[j + 1];

				if (edge1.isLeft && (!edge2.isLeft)) // case R-L ? L-R
				{ 
					// Mark as overlapping
					if (firstTimeCalled)
					{
						if (edge1.o != edge2.o)
						{
							EdgePair overlap(edge1, edge2);
							if (edge1.o->canCollide() && edge2.o->canCollide())
							{
								overlapping.insert(overlap);
							}
						}
					}
					else
					{
						// do nothing. If it is not already in there then it is not a candidate for a collision.
					}
				}
				else if ((!edge1.isLeft) && edge2.isLeft) // case L-R ? R-L
				{ 
					// Unmark as overlapping
					overlapping.erase({ edge1, edge2 });
				}
			}
		}
	}

	void buildSweepAndPrune(std::vector<OWMovableComponent*>& objects)
	{
		for (OWMovableComponent* o : objects)
		{
			gEdgesX.push_back({ o, 0, true });
			gEdgesX.push_back({ o, 0, false });

			gEdgesY.push_back({ o, 1, true });
			gEdgesY.push_back({ o, 1, false });

			gEdgesZ.push_back({ o, 2, true });
			gEdgesZ.push_back({ o, 2, false });
		}
		std::sort(gEdgesX.begin(), gEdgesX.end(), [](const Edge& a, const Edge& b) { return a.val() < b.val(); });
		std::sort(gEdgesY.begin(), gEdgesY.end(), [](const Edge& a, const Edge& b) { return a.val() < b.val(); });
		std::sort(gEdgesZ.begin(), gEdgesZ.end(), [](const Edge& a, const Edge& b) { return a.val() < b.val(); });
	}

	void collideSweepAndPrune()
	{
		OverlappingEdges colliding;
		sweepAndPrune(gEdgesX, colliding, true);
		sweepAndPrune(gEdgesY, colliding, false);
		sweepAndPrune(gEdgesZ, colliding, false);
		if (colliding.size() > 0)
		{
			char s = 'p';
		}
		for (const EdgePair& const_pr : colliding)
		{
			OWMovableComponent* a1 = const_cast<EdgePair&>(const_pr).e1.o;
			OWMovableComponent* a2 = const_cast<EdgePair&>(const_pr).e2.o;
			if (a1->collides(a2))
			{
				LogStream(LogStreamLevel::Info) << "collided [" + a1->name() + "] [" + a2->name() + "].\n";
				a1->collided(a2);
				a2->collided(a1);
			}
		}
	}

	void sweepAndPruneTick(float timeStep)
	{
		auto ticker = [timeStep](OWMovableComponent* a)
			{
				a->tick(timeStep);
			};
		for (Edge& e : gEdgesX)
		{
			ticker(e.o);
		}
	}

#endif
	// https://leanrada.com/notes/sweep-and-prune-2/
	// 
	// 
	// 1D sweep and prune
	//void swseepAndPrune()
	//{
	//	for (int i = 0; i < numSpansX; i++)
	//	{
	//		for (int j = i + 1; j < numSpansX; j++)
	//		{
	//			/* always false */
	//			if (span.xmax < span[j].xmin /*|| span[j].xmax < span.xmin*/)
	//				break;
	//			// check other axes of bounding box            
	//			// if(span.ymax < span[j].ymin || span[j].ymax < span.ymin)            
	//			// continue;         
	//			// 
	//			// // test collision

	//		}
	//	}
	//}
#ifdef BASIC_COLLISIONS
	std::vector <OWMovableComponent*> gStaticObjects;
	std::vector <OWMovableComponent*> gMoveableObjects;

	Box* bbp = nullptr;
	static int ii = 10000;

	void basicTick(float timeStep)
	{
		auto ticker = [timeStep](OWMovableComponent* a)
			{
				a->tick(timeStep);
			};
		if (ii == 5000)
		{
			if (bbp)
				bbp->scale(glm::vec3(2));
		}
		if (ii == 1000)
		{
			if (bbp)
				bbp->rotate(45, { 1,1,1 });
		}
		if (ii > 0)
		{
		}
		//ii--;
		for (OWMovableComponent* o : gMoveableObjects)
		{
			ticker(o);
		}
		for (OWMovableComponent* o : gStaticObjects)
		{
			ticker(o);
		}
	}

	void buildBasic(const std::vector<OWMovableComponent*>& objects)
	{
		for (OWMovableComponent* o : objects)
		{
			if (o->name().find("Plane ") != std::string::npos)
			{
				gStaticObjects.push_back(o);
			}
			else
			{
				gMoveableObjects.push_back(o);
			}
		}
	}

	void collidBasic()
	{
		for (OWMovableComponent* a1 : gStaticObjects)
		{
			if (a1->canCollide())
			{
				for (int j = 0; j < gMoveableObjects.size(); j++)
				{
					OWMovableComponent* a2 = gMoveableObjects[j];
					if (a2->canCollide())
					{
						if (a1->canCollide(a2))
						{
							if (a1->bounds().intersects(a2->bounds()))
							{
								if (a1->collides(a2))
								{
									//a1->collided(a2);
									a2->collided(a1);
								}
							}
						}
					}
				}
			}
		}
		int inc = 0;
		for (OWMovableComponent* a1 : gMoveableObjects)
		{
			inc++;
			if (a1->canCollide())
			{
				for (int j = inc; j < gMoveableObjects.size(); j++)
				{
					OWMovableComponent* a2 = gMoveableObjects[j];
					if (a2->canCollide())
					{
						if (a1->canCollide(a2))
						{
							if (a1->bounds().intersects(a2->bounds()))
							{
								if (a1->collides(a2))
								{
									a1->collided(a2);
									a2->collided(a1);
								}
							}
						}
					}
				}
			}
		}
	}
	/*
		auto collider = [](OcTree* o)
			{
				int inc = 0;
				for (int i = 0; i < o->mPoints.size(); i++)
				{
					inc++;
					OWMovableComponent* a1 = o->mPoints[i];
					if (a1->canCollide())
					{
						if (a1->name() == "box")
						{
							char s = 'a';
						}
						for (int j = inc; j < o->mPoints.size(); j++)
						{
							OWMovableComponent* a2 = o->mPoints[j];
							if (a2->canCollide())
							{
								if (a1->canCollide(a2))
								{
									if (a1->bounds().intersects(a2->bounds()))
									{
										if (a1->collides(a2))
										{
											a1->collided(a2);
											a2->collided(a1);
										}
									}
								}
							}
						}
					}
				}
				return true;
			};
		mOctTree->traverse(collider);
	*/
#endif

	void build(std::vector<OWMovableComponent*>& objects)
	{
#ifdef BASIC_COLLISIONS
		buildBasic(objects);
#endif
#ifdef SWEEP_AND_PRUNE
		buildSweepAndPrune(objects);
#endif
	}

	void collide()
	{
#ifdef BASIC_COLLISIONS
		collidBasic();
#endif
#ifdef SWEEP_AND_PRUNE
		collideSweepAndPrune();
#endif
	}

	void tick(float timeStep)
	{
#ifdef BASIC_COLLISIONS
		basicTick(timeStep);
#endif
#ifdef SWEEP_AND_PRUNE
		sweepAndPruneTick(timeStep);
#endif
	}

	void refresh()
	{

	}
}