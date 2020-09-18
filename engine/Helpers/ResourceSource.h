#pragma once

#include "../OWEngine/OWEngine.h"

class ResourceFactory;
/*
	A work in progress. May be removed in future iterations.
*/
class OWENGINE_API ResourceSource
{
public:
	static void factory(ResourceFactory* newFactory);
	ResourceSource();
	~ResourceSource();
protected:
	static ResourceFactory* factory() { return mFactory; }
private:
	static ResourceFactory* mFactory;
};