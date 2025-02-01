#pragma once
#include "../OWEngine/OWEngine.h"


#include <chrono>
#include <vector>
#include <algorithm>
#include <functional>
#include <streambuf>
#include <ostream>
#include <string>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

/*
	Usefull functionality that needs a place to live. Bit of a mess atm
*/

class GLApplication;
class GlobalSettings;
class Shader;
extern OWENGINE_API GlobalSettings* globals;

struct OWENGINE_API OWUtils
{
	typedef glm::f32 Float;
	typedef std::chrono::steady_clock Time;

	enum class SolidColours
	{
		BLACK,
		BLUE,
		GREEN,
		CYAN,
		RED,
		MAGENTA,
		YELLOW,
		WHITE,
		BRIGHT_BLACK,
		BRIGHT_BLUE,
		BRIGHT_GREEN,
		BRIGHT_CYAN,
		BRIGHT_RED,
		BRIGHT_MAGENTA,
		BRIGHT_YELLOW,
		BRIGHT_WHITE
	};

	static constexpr float epsilon()
	{
		return 0.00001f;
	} 

	static glm::vec4 colour(SolidColours colour);
	static std::string nowAsString();
	static std::vector<std::string> split1(const std::string &s, char delim);
	static std::vector<std::string> split2(const std::string &text, char sep);
	// trim (in place)
	static void ltrim(std::string &s);
	static void rtrim(std::string &s);
	static void trim(std::string &s);

	static std::vector<std::string> split(const std::string& s, char delim);

	static bool nearlyEqual(Float a, Float b, Float _epsilon)
	{
		// https://floating-point-gui.de/errors/comparison/
		float absA = std::abs(a);
		float absB = std::abs(b);
		float diff = std::abs(a - b);

		if (a == b) 
		{ 
			return true;
		}
		else if (a == 0 || b == 0 || (absA + absB < std::numeric_limits<float>::denorm_min()))
		{
			// a or b is zero or both are extremely close to it
			// relative error is less meaningful here
			return diff < (_epsilon * std::numeric_limits<float>::denorm_min());
		}
		else 
		{ // use relative error
			return diff / std::min((absA + absB), std::numeric_limits<float>::max()) < _epsilon;
		}
	}

	static bool isEqual(Float f1, Float f2)
	{
		// https://possiblywrong.wordpress.com/2013/11/15/floating-point-equality-its-worse-than-you-think/
		return nearlyEqual(f1, f2, 1.e-6f);
	}

	static std::string to_string(const glm::vec4& v);
	static std::string to_string(const glm::vec3& v);
	static std::string to_string(const glm::vec2& v);
};

OWENGINE_API std::ostream& operator<<(std::ostream& os, const glm::vec4& v);
OWENGINE_API std::ostream& operator<<(std::ostream& os, const glm::vec3& v);
OWENGINE_API std::ostream& operator<<(std::ostream& os, const glm::vec2& v);

OWENGINE_API std::istream& operator>>(std::istream& is, glm::vec4& v);
OWENGINE_API std::istream& operator>>(std::istream& is, glm::vec3& v);
OWENGINE_API std::istream& operator>>(std::istream& is, glm::vec2& v);
