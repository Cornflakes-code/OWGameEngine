#include <string>
#include <vector>
#include <Windows.h>

#include "../NMSFlyThrough/rope_interface_test.h"
#include "../NMSFlyThrough/rope_interface_utils.h"

typedef int(__cdecl* Test_constructionset)(int constructionSet);
typedef int(__cdecl* Test_islibrary)();
typedef int(__cdecl* Test_enums)(LPINT rs, LPINT em, int& rc, int& incrme);
typedef int(__cdecl* Test_allocmemcount)();
typedef RopeBuf*(__cdecl* Test_libraryname)();
typedef RopeBuf*(__cdecl* Test_stringlist_get)();
typedef void(__cdecl* Test_stringlist_set)(RopeBuf* newValue);
typedef void(__cdecl* Test_string_free)(RopeBuf* allocatedByDLL);

Test_constructionset test_constructionset = nullptr;
Test_islibrary test_islibrary = nullptr;
Test_enums test_enums = nullptr;
Test_allocmemcount test_allocmemcount = nullptr;
Test_libraryname test_libraryname = nullptr;
Test_stringlist_get test_stringlist_get = nullptr;
Test_stringlist_set test_stringlist_set = nullptr;
Test_string_free test_string_free = nullptr;

bool initTestFunctions()
{
	HMODULE h = dllHandle();
	if (!h)
		return false;
	test_constructionset = (Test_constructionset)GetProcAddress(h, "test_constructionset");
	if (test_constructionset == nullptr)
		return false;
	test_islibrary = (Test_islibrary)GetProcAddress(h, "test_islibrary");
	if (test_islibrary == nullptr)
		return false;
	test_enums = (Test_enums)GetProcAddress(h, "test_enums");
	if (test_enums == nullptr)
		return false;
	test_allocmemcount = (Test_allocmemcount)GetProcAddress(h, "test_allocmemcount");
	if (test_allocmemcount == nullptr)
		return false;
	test_libraryname = (Test_libraryname)GetProcAddress(h, "test_libraryname");
	if (test_libraryname == nullptr)
		return false;
	test_stringlist_get = (Test_stringlist_get)GetProcAddress(h, "test_stringlist_get");
	if (test_stringlist_get == nullptr)
		return false;
	test_stringlist_set = (Test_stringlist_set)GetProcAddress(h, "test_stringlist_set");
	if (test_stringlist_set == nullptr)
		return false;
	test_string_free = (Test_string_free)GetProcAddress(h, "test_string_free");
	if (test_string_free == nullptr)
		return false;
	return true;
}

bool testConstructionSet()
{
	RCConstructionSet testSetIn;
	testSetIn.push_back(RCConstruction::RCUnknown);
	int manyEnums = test_constructionset(toRemote(testSetIn));
	RCConstructionSet testSetOut = toConstructionSet(manyEnums);
	bool retval = testSetIn == testSetOut;
	if (!retval)
		return false;
	testSetIn.push_back(RCConstruction::RCCore);
	testSetIn.push_back(RCConstruction::RCFirst);
	testSetIn.push_back(RCConstruction::RCSeale);
	//testSetIn.push_back(RCConstruction::RCFiller);
	testSetIn.push_back(RCConstruction::RCWarrington);
	testSetIn.push_back(RCConstruction::RCXLaid);
	testSetIn.push_back(RCConstruction::RCTriWire);
	testSetOut.clear();
	manyEnums = test_constructionset(toRemote(testSetIn));
	testSetOut = toConstructionSet(manyEnums);
	retval = testSetIn == testSetOut;
	return retval;
}

bool testEnums()
{
	RCRopeState rs = RCRopeState::RCDrawing;
	RCEditMode em = RCEditMode::rcGod;
	RCConstruction rc = RCConstruction::RCTriWire;
	int rs1 = static_cast<int>(rs);

	int em1 = toRemoteEditMode(em);
	int rc1 = toRemoteConstruction(rc);
	int incrMe = 42;
	int incrMe1 = incrMe;
	int sumOfOrds = rs1 + em1 + rc1 + incrMe;
	int sumOfOrds1 = test_enums(&rs1, &em1, rc1, incrMe);
	bool retval = (rs == (RCRopeState)rs1)
		&& (em == toEditMode(em1))
		&& (rc == toConstruction(rc1))
		&& (sumOfOrds == sumOfOrds1)
		&& (incrMe == (incrMe1 + 1));
	return retval;
}

bool testLibraryName()
{
	bool retval = false;
	RopeBuf* p = test_libraryname();
	std::string libname = toString(p);
	retval = libname == "RCRopeEx2.dll";
	return retval;
}

bool testStringList()
{
	std::vector<KeyValuePair> sl1;
	sl1.push_back({ "one", "1" });
	sl1.push_back({ "two", "20" });
	sl1.push_back({ "three", "3" });
	RopeBuf* buffer = nullptr;
	RopeBuf* p = toRemoteStringList(sl1, buffer);
	test_stringlist_set(p);
	freeStringListBuffer(buffer);

	std::vector<KeyValuePair> sl2 =
		toKeyValueList(test_stringlist_get());
	bool retval = sl1.size() == sl2.size();
	if (retval)
	{
		for (int i = 0; i < sl1.size(); i++)
		{
			std::string s1 = sl1[i].asKeyValue();
			std::string s2 = sl2[i].asKeyValue();
			if (s1 != s2)
				retval = false;
		}
	}
	return retval;
}


bool runImportTests()
{
	int allocc_start = test_allocmemcount();
	bool retval = testLibraryName();

	int allocc_curr = test_allocmemcount();
	if (retval)
		retval = testStringList();
	allocc_curr = test_allocmemcount();
	if (retval)
		retval = toBool(test_islibrary());
	allocc_curr = test_allocmemcount();
	if (retval)
		retval = testConstructionSet();
	if (retval)
		retval = testEnums();
	allocc_curr = test_allocmemcount();
	return retval;
}
