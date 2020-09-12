#pragma once

class ResourceFactory;
/*
	A work in progress. May be removed in future iterations.
*/
class ResourceSource
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