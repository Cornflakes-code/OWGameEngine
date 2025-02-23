#pragma once
#include <string>

#include "../OWEngine/OWEngine.h"

class OWENGINE_API OLDObject
{
	std::string mName;
protected:
public:
	OLDObject() {}
	virtual ~OLDObject() {}
	void name(const std::string& s) { mName = s; }
	const std::string& name() const { return mName; }
	// Serialisation functions?
};
