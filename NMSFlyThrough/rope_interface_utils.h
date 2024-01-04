#pragma once
#include <vector>
#include <string>
#include <Windows.h>

#include "PolygonBuilder.h"

enum class RCConstruction
{
	RCUnknown, RCCore, RCFirst, RCSeale, RCFiller,
	RCWarrington, RCXLaid, RCTriWire
};

enum class RCRopeState
{
	RCNormal, RCCalculating, RCDrawing
};

enum class RCEditMode
{
	rcGod, rcEdit, rcBrowse, rcView, rcNoMode
};
typedef void(__cdecl* RCstringlistpointer_free)(RopeBuf* allocatedByDLL);
typedef void(__cdecl* RCstring_free)(RopeBuf* allocatedByDLL);

typedef std::vector<RCConstruction> RCConstructionSet;
struct KeyValuePair
{
	std::string Key;
	std::string Value;
	KeyValuePair(std::string const& k, std::string const& v) : Key(k), Value(v) {}
	KeyValuePair(std::string const& s)
	{
		size_t pos = s.find('=');
		if (pos > 0)
		{
			Key = s.substr(0, pos);
			Value = s.substr(pos+1);
		}
		else
		{
			Key = s;
		}
	}
	std::string asKeyValue() const
	{
		return Key + "=" + Value;
	}
};
void rcstringlistpointer_free(RopeBuf* allocatedByDLL);
void rcstring_free(RopeBuf* allocatedByDLL);

int toRemote(RCConstructionSet rs);
RCConstructionSet toConstructionSet(int manyEnums);
int toRemoteEditMode(RCEditMode value);
int toRemoteConstruction(RCConstruction value);
RCConstruction toConstruction(int value);
RCEditMode toEditMode(int value);
std::string toString(RopeBuf* value, bool auto_free = true);
std::string toStringAnsi(RopeBuf* value);
inline HMODULE dllHandle();
RopeBuf* toRemoteStringList(std::vector<KeyValuePair> value, RopeBuf*& buffer);
RopeBuf* toRemoteString(const std::string& value, RopeBuf*& strbuf);
void freeStringListBuffer(RopeBuf* aStringList);
std::vector<KeyValuePair> toKeyValueList(RopeBuf* value);
bool toBool(int value);
bool initInterfaceUtils();
