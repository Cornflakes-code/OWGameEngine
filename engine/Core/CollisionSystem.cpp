#include "CollisionSystem.h"

#include <set>
#include <algorithm>
#include "OWActor.h"
#include "OWMovableComponent.h"
#include "../Geometry/Box.h"
#include "../Core/LogStream.h"

//#define SWEEP_AND_PRUNE
//#define SWEEP_AND_PRUNE_EX
#define BASIC_COLLISIONS
//#define DYNAMIC_SPHERE_TREE

namespace CollisionSystem
{
#ifdef SWEEP_AND_PRUNE
	// Good discussion about different types of collision optimisations
	// https://www.gamedev.net/forums/topic/328022-what-collision-method-is-better/
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

	void doInsertionSort(std::vector<Edge>& edges)
	{
		// Plain Insertion sort
		for (int i = 1; i < edges.size(); i++)
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (edges[j].val() <= edges[j + 1].val())
					break;

				// Swap
				std::swap(edges[j], edges[j + 1]);
			}
		}
	}

	void doSweepAndPrune(std::vector<Edge>& edges, OverlappingEdges& overlapping, bool firstTimeCalled)
	{
		for (int j = 0; j < edges.size()-1; j++)
		{
			const Edge& edge1 = edges[j];
			const Edge& edge2 = edges[j + 1];
			if (edge1.o == edge2.o)
				continue;

			if (edge1.isLeft && (!edge2.isLeft)) // case R-L ? L-R
			{ 
				// Mark as overlapping
				if (true)
				{
					if (edge1.o->canCollide() && edge2.o->canCollide())
					{
						const Edge* e1 = edge1.o < edge2.o ? &edge1 : &edge2;
						const Edge* e2 = edge1.o < edge2.o ? &edge2 : &edge1;
						EdgePair overlap(*e1, *e2);
						//EdgePair overlap(edge1, edge2);
						overlapping.insert(overlap);
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
				const Edge* e1 = edge1.o < edge2.o ? &edge1 : &edge2;
				const Edge* e2 = edge1.o < edge2.o ? &edge2 : &edge1;
				EdgePair unlap(*e1, *e2);
				overlapping.erase(unlap);
			}
		}
	}

	auto comp = [](const Edge& a, const Edge& b)
		{
			//				if (a.val() == b.val())
			//					return a.o->name() < b.o->name();
			return a.val() < b.val();
		};
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
		std::sort(gEdgesX.begin(), gEdgesX.end(), comp);
		std::sort(gEdgesY.begin(), gEdgesY.end(), comp);
		std::sort(gEdgesZ.begin(), gEdgesZ.end(), comp);
	}

	void collideSweepAndPrune()
	{
		OverlappingEdges colliding;
		doSweepAndPrune(gEdgesX, colliding, true);
		doSweepAndPrune(gEdgesY, colliding, false);
		doSweepAndPrune(gEdgesZ, colliding, false);
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
				//LogStream(LogStreamLevel::Info) << "collided [" + a1->name() + "] [" + a2->name() + "].\n";
				a1->collided(a2);
				a2->collided(a1);
			}
		}
		std::sort(gEdgesX.begin(), gEdgesX.end(), comp);
		std::sort(gEdgesY.begin(), gEdgesY.end(), comp);
		std::sort(gEdgesZ.begin(), gEdgesZ.end(), comp);
//		doInsertionSort(gEdgesX);
//		doInsertionSort(gEdgesY);
//		doInsertionSort(gEdgesZ);
	}

	void sweepAndPruneTick(float timeStep)
	{
		for (Edge& e : gEdgesX)
		{
			if (e.isLeft)
				e.o->tick(timeStep);
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
									a1->collided(a2);
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
	
#endif

#ifdef SWEEP_AND_PRUNE_EX


	void buildSweepAndPruneEx(std::vector<OWMovableComponent*>& objects)
	{
	}
	void collideSweepAndPruneEx()
	{
	}
	void sweepAndPruneTickEx(float timeStep)
	{
	}

#endif
	void build(std::vector<OWMovableComponent*>& objects)
	{
#ifdef BASIC_COLLISIONS
		buildBasic(objects);
#endif
#ifdef SWEEP_AND_PRUNE
		buildSweepAndPrune(objects);
#endif
#ifdef SWEEP_AND_PRUNE_EX
		buildSweepAndPruneEx(objects);
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
#ifdef SWEEP_AND_PRUNE_EX
		collideSweepAndPruneEx();
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
#ifdef SWEEP_AND_PRUNE_EX
		sweepAndPruneTickEx(timeStep);
#endif
	}

	void refresh()
	{

	}
}
