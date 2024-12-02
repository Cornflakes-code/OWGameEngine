#include "CommonUtils.h"

#include <ctime>
#include <cctype>

#include "ErrorHandling.h"

GLint OWUtils::PolygonModeRIAA::mOriginalMode;
GLfloat OWUtils::LineWidthRIAA::mOriginalWidth;

GLenum OWUtils::BlendFuncRIAA::mSfactor;
GLenum OWUtils::BlendFuncRIAA::mDfactor;

glm::vec4 gSolidColours[16] =
{
  {0.0f, 0.0f, 0.0f, 1.0f},     // BLACK
  {0.0f, 0.0f, 0.67f, 1.0f},    // BLUE
  {0.0f, 0.67f, 0.0f, 1.0f},    // GREEN
  {0.0f, 0.67f, 0.67f, 1.0f},   // CYAN
  {0.67f, 0.0f, 0.0f, 1.0f},    // RED
  {0.67f, 0.0f, 0.67f, 1.0f},   // MAGENTA
  {0.67f, 0.67f, 0.0f, 1.0f},   // YELLOW
  {0.67f, 0.67f, 0.67f, 1.0f},  // WHITE
  {0.33f, 0.33f, 0.33f, 1.0f},  // BRIGHT BLACK
  {0.33f, 0.33f, 1.0f, 1.0f},   // BRIGHT BLUE
  {0.33f, 1.0f, 0.33f, 1.0f},   // BRIGHT GREEN
  {0.33f, 1.0f, 1.0f, 1.0f},    // BRIGHT CYAN
  {1.0f, 0.33f, 0.33f, 1.0f},   // BRIGHT RED
  {1.0f, 0.33f, 1.0f, 1.0f},    // BRIGHT MAGENTA
  {1.0f, 1.0f, 0.33f, 1.0f},    // BRIGHT YELLOW
  {1.0f, 1.0f, 1.0f, 1.0f}      // BRIGHT WHITE
};

glm::vec4 OWUtils::colour(OWUtils::SolidColours colour)
{
	return gSolidColours[static_cast<unsigned int>(colour)];
}

std::string OWUtils::nowAsString()
{
	std::time_t now_c
		= std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm t;
	localtime_s(&t, &now_c);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "config_%H-%M-%S.json", &t);
	return std::string(buffer);
}

template <typename Out>
void splitX(const std::string &s, char delim, Out result) 
{
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) 
	{
		*result++ = item;
	}
}

std::vector<std::string> OWUtils::split1(const std::string &s, char delim)
{
	// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string?rq=1
	std::vector<std::string> elems;
	splitX(s, delim, std::back_inserter(elems));
	return elems;
}

std::vector<std::string> OWUtils::split2(const std::string &text, char sep)
{
	// https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string?rq=1
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		if (end != start) {
			tokens.push_back(text.substr(start, end - start));
		}
		start = end + 1;
	}
	if (end != start) {
		tokens.push_back(text.substr(start));
	}
	return tokens;
}

void OWUtils::ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
	{
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
void OWUtils::rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

void OWUtils::trim(std::string &s)
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

std::vector<std::string> OWUtils::split(const std::string& s, char delim)
{
	std::vector<std::string> results;
	extractWords(s, delim, back_inserter(results));
	return results;
}

OWUtils::PolygonModeRIAA::PolygonModeRIAA(GLenum face, GLenum mode)
	: mActive(face != UINT_MAX && mode != UINT_MAX)
{
	;
	if (mActive)
	{
		static bool onceOnly = true;
		if (onceOnly)
		{
			glGetIntegerv(GL_POLYGON_MODE, &mOriginalMode);
			onceOnly = false;
		}
		glPolygonMode(face, mode);
	}
}


OWUtils::LineWidthRIAA::LineWidthRIAA(float width)
	: mActive(width >= 0)
{
	if (mActive)
	{
		static bool onceOnly = true;
		if (onceOnly)
		{
			glGetFloatv(GL_LINE_WIDTH, &mOriginalWidth);
			onceOnly = false;
		}
		glLineWidth(width);
	}
}

OWUtils::BlendFuncRIAA::BlendFuncRIAA(GLenum sfactor, GLenum dfactor)
	: mActive(sfactor != UINT_MAX && dfactor != UINT_MAX)
{
	if (mActive)
	{
		static bool onceOnly = true;
		if (onceOnly)
		{
			mSfactor = GL_SRC_ALPHA;
			mDfactor = GL_ONE_MINUS_SRC_ALPHA;
			onceOnly = false;
		}
		glBlendFunc(sfactor, dfactor);
	}
}
