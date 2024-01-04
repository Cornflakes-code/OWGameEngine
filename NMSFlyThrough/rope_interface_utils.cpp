#include "rope_interface_utils.h"

RCstringlistpointer_free rcstringlistpointer_free_imp = nullptr;
RCstring_free rcstring_free_imp = nullptr;

inline HMODULE dllHandle()
{	
	static HMODULE hImportLibrary = nullptr;
	if (hImportLibrary == nullptr)
	{
		hImportLibrary = LoadLibraryA("RCRopeEx2.dll");
	}
	return hImportLibrary;
}

void rcstringlistpointer_free(RopeBuf* allocatedByDLL)
{
	rcstringlistpointer_free_imp(allocatedByDLL);
}

void rcstring_free(RopeBuf* allocatedByDLL)
{
	rcstring_free_imp(allocatedByDLL);
}

int toRemote(RCConstructionSet rs)
{
	int retval = 0;
	for (RCConstruction v : rs)
	{
		switch (v)
		{
		case RCConstruction::RCUnknown: retval |= 1; break;
		case RCConstruction::RCCore: retval |= 2; break;
		case RCConstruction::RCFirst: retval |= 4; break;
		case RCConstruction::RCSeale: retval |= 8; break;
		case RCConstruction::RCFiller: retval |= 16; break;
		case RCConstruction::RCWarrington: retval |= 32; break;
		case RCConstruction::RCXLaid: retval |= 64; break;
		case RCConstruction::RCTriWire: retval |= 128; break;
		}
	}
	return retval;
}

RCConstructionSet toConstructionSet(int manyEnums)
{
	RCConstructionSet retval;
	if ((manyEnums & 1) != 0)
		retval.push_back(RCConstruction::RCUnknown);
	if ((manyEnums & 2) != 0)
		retval.push_back(RCConstruction::RCCore);
	if ((manyEnums & 4) != 0)
		retval.push_back(RCConstruction::RCFirst);
	if ((manyEnums & 8) != 0)
		retval.push_back(RCConstruction::RCSeale);
	if ((manyEnums & 16) != 0)
		retval.push_back(RCConstruction::RCFiller);
	if ((manyEnums & 32) != 0)
		retval.push_back(RCConstruction::RCWarrington);
	if ((manyEnums & 64) != 0)
		retval.push_back(RCConstruction::RCXLaid);
	if ((manyEnums & 128) != 0)
		retval.push_back(RCConstruction::RCTriWire);
	return retval;
}
int toRemoteEditMode(RCEditMode value)
{
	return static_cast<int>(value);
}

int toRemoteConstruction(RCConstruction value)
{
	return static_cast<int>(value);
}

RCConstruction toConstruction(int value)
{
	return static_cast<RCConstruction>(value);
}

RCEditMode toEditMode(int value)
{
	return static_cast<RCEditMode>(value);
}

std::string toString(RopeBuf* value, bool auto_free)
{
	std::string s;
	if (value != nullptr)
	{
		s = toStringAnsi(value);
		if (auto_free)
			rcstring_free(value);
	}
	return s;
}

std::string toStringAnsi(RopeBuf* value)
{
	std::string retval = (char*)value;
	return retval;
}

RopeBuf* toRemoteStringList(std::vector<KeyValuePair> value, RopeBuf*& buffer)
{
	// Allocate room for every string pointer in the list ...
	size_t len = value.size();
	// ... as well as room to store the number of string pointers
	buffer = new RopeBuf[len + 1];
	buffer[0] = len;
	RopeBuf* p = buffer + 1;
	
	for (int i = 0; i < len; i++)
	{
		RopeBuf* strbuf = nullptr;
		strbuf = toRemoteString(value[i].asKeyValue(), strbuf);
		p[i] = reinterpret_cast<RopeBuf>(strbuf);
	}
	return buffer;
}

RopeBuf* toRemoteString(const std::string& value, RopeBuf*& strbuf)
{
	// do not forget space for the terminating zero
	char* p = new char[value.size() + 1];
	for (int i = 0; i < value.size(); i++)
	{
		p[i] = value[i];
	}
	p[value.size()] = 0;
	strbuf = (RopeBuf*)p;
	return strbuf;
}

void freeStringListBuffer(RopeBuf* aStringList)
{
	if (aStringList != nullptr)
	{
		size_t len = aStringList[0];
		for (size_t i = 0; i < len; i++)
		{
			RopeBuf* strbuf = reinterpret_cast<RopeBuf*>(aStringList[i + 1]);
			delete [] strbuf;
		}
		delete [] aStringList;
	}
}

std::vector<KeyValuePair> toKeyValueList(RopeBuf* value)
{
	std::vector<KeyValuePair> retval;
	if (value != nullptr)
	{
		RopeBuf* p = value;
		size_t len = *(int*)p;
		for (size_t i = 0; i < len; i++)
		{
			p++;
			std::string s = toString((RopeBuf*)(*p), false);
			retval.push_back(KeyValuePair(s));
		}
		rcstringlistpointer_free_imp(value);
	}
	return retval;
}
bool toBool(int value)
{
	return value == 0 ? false : true;
}

bool initInterfaceUtils()
{
	HMODULE h = dllHandle();
	if (!h)
		return false;
	rcstringlistpointer_free_imp = (RCstringlistpointer_free)GetProcAddress(h, "rcstringlistpointer_free");
	if (rcstringlistpointer_free_imp == nullptr)
		return false;
	rcstring_free_imp = (RCstring_free)GetProcAddress(h, "rcstring_free");
	if (rcstring_free_imp == nullptr)
		return false;
	return true;
}
