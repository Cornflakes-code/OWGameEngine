#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

static inline void ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
	{
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) 
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

static inline void trim(std::string &s) 
{
	// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
	ltrim(s);
	rtrim(s);
}

std::vector<std::string>split(const std::string& s, char delim);
