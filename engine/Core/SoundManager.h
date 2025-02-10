#pragma once
#include <map>
#include <set>
#include <string>
#include <filesystem>

#include <glm/glm.hpp>

#include "OWObject.h"

#include "../OWEngine/OWEngine.h"

class OWENGINE_API SoundManager: public OWObject
{
	enum class SoundType
	{
		Bloop, Boing, SoundTypeCount
	};
	typedef std::map<SoundType, std::set<OWObject*>> OwnedSoundsType;
	static OwnedSoundsType mOwnedSounds;
	typedef std::map<int, SoundType> UniqueSoundsType;
	static UniqueSoundsType mUniqueSounds;
	typedef std::map<std::filesystem::path,
		std::string> SoundCache;
public:
	SoundManager();
	void registerSound(SoundType soundType, const std::string& s);
	//int registerForSound(OWObject* o, SoundType soundType);
	void setLocation(int soundId, const glm::vec3& v);
	void setVolume(int soundId, float volume);
	void playSound(int soundId, const glm::vec3& soundPosition, const glm::vec3& listenerPosition);
private:
	const std::string& getSound(const std::string& fileName);
	static SoundCache mLoadedFiles;
};