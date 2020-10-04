#pragma once

#include <chrono>
#include <vector>
#include <algorithm>
#include <functional>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include "../OWEngine/OWEngine.h"


/*
	Usefull functionality that needs a place to live. BNit of a mess atm
*/

class GLApplication;
class GlobalSettings;
class Shader;
extern OWENGINE_API GlobalSettings* globals;

struct OWENGINE_API OWUtils
{
	typedef std::function<void(const glm::mat4& proj, const glm::mat4& view,
				const glm::mat4& model, Shader* shader)> RenderCallbackType;

	typedef std::function< glm::vec2(const glm::vec2)> ScaleByAspectRatioType;

	typedef std::function<void(Shader* shader,
					ScaleByAspectRatioType scaler,
					float aspectRatio)> ResizeCallbackType;

	struct TextureBlock
	{
		std::string name; // string in shader
		unsigned int location; // from glGenTextures
		unsigned int imageUnit; // GL_TEXTURE0 -> GL_TEXTUREN
		unsigned int target; // i.e. GL_TEXTURE_2D
	};

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
		GLint polygonMode;
	public:
		PolygonModeRIAA()
		{
			glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
		}

		~PolygonModeRIAA()
		{
			glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
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
