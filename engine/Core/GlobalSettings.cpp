#include "GlobalSettings.h"

OWUtils::Time::time_point GlobalSettings::mLoadTime;

GlobalSettings::GlobalSettings()
{
	mLoadTime = OWUtils::Time::now();
}

float GlobalSettings::secondsSinceLoad()
{
	OWUtils::Time::time_point t = OWUtils::Time::now();
	auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(t - mLoadTime);
	return milli.count() / 1000.0f;
}

