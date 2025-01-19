#pragma once

#include <filesystem>

#include <glm/glm.hpp>

#include "../OWEngine/OWEngine.h"

#include "CommonUtils.h"

class SaveAndRestore;
class Movie;
class MacroRecorder;
class Logger;
class GLApplication;
class Camera;
class UserInput;
/*
	Wrapper singleton class providing const getter access to commonally 
	needed const global data.
	Initialised with User Config File settings and, in turn, initialises other classes
	when they are added via various init() methods. A singleton class
	constructed by GLApplication.
*/

class OWENGINE_API GlobalSettings
{
public:
	GlobalSettings();
	void loadSettings(const std::filesystem::path& configFile);
	// convenience methods. If the host exe does not create GLApplication then
	// these may be invalid.
	float aspectRatio() const
	{
		return static_cast<float>(globals->physicalWindowSize().x / globals->physicalWindowSize().y);
	}
	bool aspectRatioChanged() const { return mAspectRatioChanged; }
	void clearAspectRatioChangedFlag() { mAspectRatioChanged = false; }
	float secondsSinceLoad();
	const glm::uvec2& physicalWindowSize() const { return mPhysicalWindowSize; }

	// Handy for converting mouse position to World Space
	// If we want to calculate a direction (ie ray) then calcPoint = false
	// If we want to calculate a point then calcPoint = true
	glm::vec3 mouseToWorld(const glm::vec3& mouseCoord, bool calcPoint) const;
	glm::vec3 toNormalisedDeviceCoordinates(const glm::vec3& mouseCoord) const;
	glm::vec4 toEyeCoords(const glm::vec4& clipCoords, bool calcPoint) const;
	glm::vec3 toWorldCoords(const glm::vec4& eyeCoords) const;
	void physicalWindowSize(const glm::uvec2& newValue)
	{
		mAspectRatioChanged = true;
		mPhysicalWindowSize = newValue;
	}
	void minimised(bool newValue) { mMinimised = newValue; }

	// const Getters. 
	const SaveAndRestore* saveAndRestore() const { return mSaveAndRestore; }
	const Movie* movie() const { return mMovie; }
	const MacroRecorder* recorder() const { return mRecorder; }
	const Logger* logger() const { return mLogger; };
	const Camera* camera() const { return mCamera; };
	const GLApplication* application() const { return mApplication; }
	bool minimised() const { return mMinimised; }

	// Getters. 
	GLApplication* application() { return mApplication; }

	// Setters. Quick and dirty applications do not need to call all of these
	//  Linkage stills applies so you need to include OWEngine.dll.
	void configAndSet(SaveAndRestore* sr, Movie* m, MacroRecorder* mr,
		Logger* log, Camera* c, GLApplication* app);
	void configAndSet(UserInput* ui);
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	const MacroRecorder* mRecorder = nullptr;
	const Movie* mMovie = nullptr;
	const Logger* mLogger = nullptr;
	GLApplication* mApplication = nullptr;
	SaveAndRestore* mSaveAndRestore = nullptr;
	Camera* mCamera = nullptr;
	glm::uvec2 mPhysicalWindowSize;
	static OWUtils::Time::time_point mLoadTime; 
	bool mAspectRatioChanged = false;
	bool mMinimised = false;
	friend class GLApplication;
#pragma warning( pop )
};