#pragma once

#include <chrono>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

#ifndef __gl_h_
#include <glad/glad.h>
#endif


class GLApplication;
extern GLApplication* theApp;

class ListenerHelper
{
	/* There does not appear to be an '==' operator for std::function, 
	 hence there is no way to remove listeners frome event generators.
	Google idicates passing a unique Id to allow this to happen. Derived 
	classes pass mId to the event manager when adding and removing listeners
	*/
public:
	size_t mUniqueId;
protected:
	ListenerHelper()
	{
		// handle == 0 is never generated. 
		static size_t handle = 1;
		mUniqueId = handle++;
	}
	ListenerHelper(const ListenerHelper&)
		: ListenerHelper()
	{}
};
struct OWUtils
{
	typedef glm::f32 Float;
	typedef std::chrono::steady_clock Time;

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

	class PolygonModeRIAA
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
	std::string asString(Direction dir);
	extern glm::vec4 Rose[NumDirections];
};
