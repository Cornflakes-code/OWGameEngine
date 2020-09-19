#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

#include "../OWEngine/OWEngine.h"

// trim (in place)
void OWENGINE_API ltrim(std::string &s);
void OWENGINE_API rtrim(std::string &s);
void OWENGINE_API trim(std::string &s);

std::vector<std::string> OWENGINE_API split(const std::string& s, char delim);
