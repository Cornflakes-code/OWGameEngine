#include "CollisionSystem.h"
#include "OWActor.h"
#include "OWMovableComponent.h"
#include "../Geometry/Box.h"

namespace CollisionSystem
{

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
			//std::vector <OWMovableComponent*> gObjects;
	std::vector <OWMovableComponent*> gStaticObjects;
	std::vector <OWMovableComponent*> gMoveableObjects;

	Box* bbp = nullptr;
	static int ii = 10000;

	void tick(float timeStep)
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


	void build(const std::vector<OWMovableComponent*>& objects)
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

	void collide()
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

	void refresh()
	{

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
}