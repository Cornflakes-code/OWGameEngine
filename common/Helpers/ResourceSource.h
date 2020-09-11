#pragma once

class ResourceFactory;

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