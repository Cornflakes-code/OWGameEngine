#include "utils.h"
#include <algorithm>
#include <cctype>

void OWENGINE_API ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
	{
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
inline void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

inline void trim(std::string &s)
{
	// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
	ltrim(s);
	rtrim(s);
}


template <typename OutputIterator>
void extractWords(std::string const& s, char delim, OutputIterator out)
{
	auto const isDelim = [delim](char letter) { return letter == delim; };

	auto beginWord = std::find_if_not(begin(s), end(s), isDelim);
	while (beginWord != end(s))
	{
		auto const endWord = std::find_if(beginWord, end(s), isDelim);
		*out = std::string(beginWord, endWord);
		++out;
		beginWord = std::find_if_not(endWord, end(s), isDelim);
	}
}

std::vector<std::string>split(const std::string& s, char delim)
{
	std::vector<std::string> results;
	extractWords(s, delim, back_inserter(results));
	return results;
}

