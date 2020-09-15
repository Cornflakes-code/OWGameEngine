#include "ResourceSource.h"

ResourceFactory* ResourceSource::mFactory = nullptr;

ResourceSource::ResourceSource()
{}

ResourceSource::~ResourceSource()
{}

void ResourceSource::factory(ResourceFactory* newFactory)
{
	mFactory = newFactory;
}
