#pragma once
#include "../OWEngine/OWEngine.h"


#include <chrono>
#include <vector>
#include <algorithm>
#include <functional>

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

	static glm::vec4 colour(SolidColours colour);
	static std::string nowAsString();
	static std::vector<std::string> split1(const std::string &s, char delim);
	static std::vector<std::string> split2(const std::string &text, char sep);
	// trim (in place)
	static void ltrim(std::string &s);
	static void rtrim(std::string &s);
	static void trim(std::string &s);

	static std::vector<std::string> split(const std::string& s, char delim);

	static bool nearlyEqual(Float a, Float b, Float epsilon)
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
			return diff < (epsilon * std::numeric_limits<float>::denorm_min());
		}
		else 
		{ // use relative error
			return diff / std::min((absA + absB), std::numeric_limits<float>::max()) < epsilon;
		}
	}

	static bool isEqual(Float f1, Float f2)
	{
		// https://possiblywrong.wordpress.com/2013/11/15/floating-point-equality-its-worse-than-you-think/
		return nearlyEqual(f1, f2, 1.e-6f);
	}

	class OWENGINE_API PolygonModeRIAA
	{
		static GLint mOriginalMode;
		bool mActive = true;
	public:
		PolygonModeRIAA(GLenum face, GLenum mode);
		~PolygonModeRIAA()
		{
			if (mActive)
				glPolygonMode(GL_FRONT_AND_BACK, mOriginalMode);
		}
	};

	class OWENGINE_API LineWidthRIAA
	{
		static GLfloat mOriginalWidth;
		bool mActive = true;
	public:
		LineWidthRIAA(float width);
		~LineWidthRIAA()
		{
			if (mActive)
				glLineWidth(mOriginalWidth);
		}
	};

	class OWENGINE_API BlendFuncRIAA
	{
		static GLenum mSfactor;
		static GLenum mDfactor;
		bool mActive = true;
	public:
		BlendFuncRIAA(GLenum sfactor, GLenum dfactor);
		~BlendFuncRIAA()
		{
			if (mActive)
				glBlendFunc(mSfactor, mDfactor);
		}
	};
};

namespace Compass
{
	enum Direction
	{
		North,
		South,
		East,
		West,
		In,
		Out,
		NoDirection,
		NumDirections
	};
	std::string OWENGINE_API asString(Direction dir);
	extern glm::vec4 OWENGINE_API Rose[NumDirections];
};
