#pragma once

#if defined(_MSC_VER)
	#if defined(OWENGINE_EXPORTS)
		#define OWENGINE_API __declspec(dllexport)
	#else
		#define OWENGINE_API __declspec(dllimport)
	#endif
#endif

#if (__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#       define OW_UNUSED         __attribute__((__unused__))
#else
#       define OW_UNUSED(x)
//template <class T> void OW_UNUSED(const T&) { }
#endif

typedef size_t OWSize;

#if _WIN32 || _WIN64
#if _WIN64
#else
#define NOMINMAX
#endif
#endif