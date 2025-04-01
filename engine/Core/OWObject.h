#pragma once
#include <string>

#include "../OWEngine/OWEngine.h"

class OWENGINE_API OWObject
{
	std::string mName;
protected:
public:
	OWObject() {}
	virtual ~OWObject() {}
	void name(const std::string& s) { mName = s; }
	const std::string& name() const { return mName; }
	// Serialisation functions?
	void load(const std::istream& in, unsigned int fileVersion);
	void write(const std::ostream& in, unsigned int fileVersion);
};
