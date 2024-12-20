#pragma once
#include <string>

#include "../OWEngine/OWEngine.h"

class OWENGINE_API OWObject
{
	std::string mName;
public:
	virtual ~OWObject() {}
	void name(const std::string& s) { mName = s; }
	const std::string& name() const { return mName; }
};
