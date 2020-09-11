#include "utils.h"
#include <algorithm>
#include <cctype>
// trim from start (in place)

//std::vector<std::string> split(const std::string& str, std::string& delims)
//{
//	std::regex reg("\\s+");
//
//	// Get an iterator after filtering through the regex
//	std::sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
//
//	// Keep a dummy end iterator - Needed to construct a vector
//	// using (start, end) iterators.
//	std::sregex_token_iterator end;
//
//	return std::vector<std::string>(iter, end);
//}

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

