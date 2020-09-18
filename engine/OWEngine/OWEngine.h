#pragma once

#if defined(_MSC_VER)
	#if defined(OWENGINE_EXPORTS)
		#define OWENGINE_API __declspec(dllexport)
	#else
		#define OWENGINE_API __declspec(dllimport)
	#endif
#endif