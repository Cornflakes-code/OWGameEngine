#include "CommonUtils.h"

#include <string>
#include <ctime>
#include <cctype>
#include <algorithm>

#include <glm/gtx/quaternion.hpp>

#include "ErrorHandling.h"

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

OWUtils::SolidColours OWUtils::colour(const std::string& solidColourName)
{
	std::string upper = solidColourName;
	std::transform(upper.begin(), upper.end(), upper.begin(),
		[](unsigned char c) { return static_cast<char>(std::toupper(c)); });

	if (upper == "BLACK") return OWUtils::SolidColours::BLACK;
	if (upper == "BLUE") return OWUtils::SolidColours::BLUE;
	if (upper == "BRIGHT_BLACK") return OWUtils::SolidColours::BRIGHT_BLACK;
	if (upper == "BRIGHT_BLUE") return OWUtils::SolidColours::BRIGHT_BLUE;
	if (upper == "BRIGHT_CYAN") return OWUtils::SolidColours::BRIGHT_CYAN;
	if (upper == "BRIGHT_GREEN") return OWUtils::SolidColours::BRIGHT_GREEN;
	if (upper == "BRIGHT_RED") return OWUtils::SolidColours::BRIGHT_RED;
	if (upper == "BRIGHT_MAGENTA") return OWUtils::SolidColours::BRIGHT_MAGENTA;
	if (upper == "BRIGHT_WHITE") return OWUtils::SolidColours::BRIGHT_WHITE;
	if (upper == "BRIGHT_YELLOW") return OWUtils::SolidColours::BRIGHT_YELLOW;
	if (upper == "CYAN") return OWUtils::SolidColours::CYAN;
	if (upper == "GREEN") return OWUtils::SolidColours::GREEN;
	if (upper == "MAGENTA") return OWUtils::SolidColours::MAGENTA;
	if (upper == "RED") return OWUtils::SolidColours::RED;
	if (upper == "WHITE") return OWUtils::SolidColours::WHITE;
	if (upper == "YELLOW") return OWUtils::SolidColours::YELLOW;
	return OWUtils::SolidColours::UNKNOWN;
}
#include<cstdlib>
#include<ctime>

static std::vector<glm::vec4> gAllSolidColours =
{
		OWUtils::colour(OWUtils::SolidColours::BLACK),
		OWUtils::colour(OWUtils::SolidColours::BLUE),
		OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLACK),
		OWUtils::colour(OWUtils::SolidColours::BRIGHT_BLUE),
		OWUtils::colour(OWUtils::SolidColours::BRIGHT_CYAN),
		OWUtils::colour(OWUtils::SolidColours::BRIGHT_GREEN),
		OWUtils::colour(OWUtils::SolidColours::BRIGHT_RED),
		OWUtils::colour(OWUtils::SolidColours::BRIGHT_MAGENTA),
		OWUtils::colour(OWUtils::SolidColours::BRIGHT_WHITE),
		OWUtils::colour(OWUtils::SolidColours::BRIGHT_YELLOW),
		OWUtils::colour(OWUtils::SolidColours::CYAN),
		OWUtils::colour(OWUtils::SolidColours::GREEN),
		OWUtils::colour(OWUtils::SolidColours::MAGENTA),
		OWUtils::colour(OWUtils::SolidColours::RED),
		OWUtils::colour(OWUtils::SolidColours::WHITE),
		OWUtils::colour(OWUtils::SolidColours::YELLOW)
};


std::vector<glm::vec4> OWUtils::allSolidColours()
{
	return gAllSolidColours;
}

glm::vec4 OWUtils::randomSolidColour()
{
	static bool gOnceOnly = true;
	if (gOnceOnly)
	{
		gOnceOnly = false;
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
	}
	int r = std::rand() % gAllSolidColours.size();
	return gAllSolidColours[r];
}

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

glm::quat rotationBetweenVectors(const glm::vec3& start_, const glm::vec3& dest_)
{
	// https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions
	// https://github.com/opengl-tutorials/ogl/blob/master/common/quaternion_utils.cpp

	glm::vec3 start = normalize(start);
	glm::vec3 dest = normalize(dest);

	float cosTheta = dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) 
	{
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
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


std::ostream& operator<<(std::ostream& os, const glm::vec4& v)
{
	os << v.x << " " << v.y << " " << v.z << " " << v.w;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& v)
{
	os << v.x << " " << v.y << " " << v.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& v)
{
	os << v.x << " " << v.y;
	return os;
}


std::istream& operator>>(std::istream& is, glm::vec4& v)
{
	is >> v.x >> v.y >> v.z >> v.w;
	return is;
}

std::istream& operator>>(std::istream& is, glm::vec3& v)
{
	is >> v.x >> v.y >> v.z;
	return is;
}

std::istream& operator>>(std::istream& is, glm::vec2& v)
{
	is >> v.x >> v.y;
	return is;
}

std::vector<std::string> OWUtils::split(const std::string& s, char delim)
{
	std::vector<std::string> results;
	extractWords(s, delim, back_inserter(results));
	return results;
}

std::string OWUtils::to_string(const glm::vec4& v)
{
	std::stringstream ss;
	ss << v;
	return ss.str();
}

std::string OWUtils::to_string(const glm::vec3& v)
{
	std::stringstream ss;
	ss << v;
	return ss.str();
}

std::string OWUtils::to_string(const glm::vec2& v)
{
	std::stringstream ss;
	ss << v;
	return ss.str();
}

std::string OWUtils::to_string(float v)
{
	std::stringstream ss;
	ss << v;
	return ss.str();
}
