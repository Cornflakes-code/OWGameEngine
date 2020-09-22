#pragma once

#include <glm/glm.hpp>
#include "../OWEngine/OWEngine.h"
#include "../Helpers/CommonUtils.h"

/*
	const singleton object wrapping access to commanly need global data.
*/
class SaveAndRestore;
class Movie;
class MacroRecorder;
class Logger;
class ResourceFactory;
class GLApplication;

class OWENGINE_API GlobalSettings
{
public:
	// convenience methods. If tyhe host exe does not create GLApplication then
	// these may be invalid.
	glm::vec2 pointingDevicePosition() const { return mPointingDevicePosition; }
	float secondsSinceLoad();
	const glm::uvec2& physicalWindowSize() const { return mPhysicalWindowSize; }
	void physicalWindowSize(const glm::uvec2& newValue) { mPhysicalWindowSize = newValue; }

	// Getters. May be null depending on what the exe does
	const SaveAndRestore* saveAndRestore() { return mSaveAndRestore; }
	const Movie* movie() const { return mMovie; }
	const MacroRecorder* recorder() const { return mRecorder;  }
	const Logger* logger() const { return mLogger; };
	const ResourceFactory* resourceCache() const { return mResFactory; }
	ResourceFactory* resourceCache() { return mResFactory; }
	GLApplication* application() { return mApplication; }
	const GLApplication* application() const { return mApplication; }

	// Setters. Quick and dirty applications do not need to call all of these
	//  Linkage stills applies so you need to include OWEngine.dll.
	void saveAndRestore(SaveAndRestore* newValue) { mSaveAndRestore = newValue; }
	void movie(Movie* newValue) { mMovie = newValue; }
	void recorder(MacroRecorder* newValue) { mRecorder = newValue; }
	void logger(Logger* newValue) { mLogger = newValue; };
	void resourceCache(ResourceFactory* newValue) { mResFactory = newValue; }
	void application(GLApplication* newValue) { mApplication = newValue; }
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	GlobalSettings();
	const MacroRecorder* mRecorder = nullptr;
	const Movie* mMovie = nullptr;
	const Logger* mLogger = nullptr;
	GLApplication* mApplication = nullptr;
	ResourceFactory* mResFactory;
	glm::uvec2 mPhysicalWindowSize;
	glm::vec2 mPointingDevicePosition;
	SaveAndRestore* mSaveAndRestore = nullptr;
	static OWUtils::Time::time_point mLoadTime;
	friend class GLApplication;
#pragma warning( pop )
};