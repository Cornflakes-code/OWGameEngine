#include "SoundManager.h"
#include <iostream>
#include <fstream>


#include "../Sound/miniAud.h"
#include "../Core/ErrorHandling.h"

#include "../Core/ResourcePathFactory.h"

// https://miniaud.io/docs/manual/
// https://www.reddit.com/r/miniaudio/comments/ry3vjz/getting_started_with_miniaudio/
std::map<int, SoundManager::SoundType> SoundManager::mUniqueSounds;

SoundManager::SoundManager()
{
	//name("SoundManager");

	ma_init();
	registerSound(SoundType::Bloop, "bloop_x.wav");
	registerSound(SoundType::Boing, "boing_x.wav");
}

void SoundManager::registerSound(SoundType soundType, const std::string& s)
{
	int ndx = ma_initSound(getSound(s).data(), mUniqueSounds.size());
	mUniqueSounds[ndx] = soundType;
}

/*
int SoundManager::registerForSound(OWObject* o, SoundType soundType)
{
	OwnedSoundsType::iterator iter = mOwnedSounds.find(soundType);
	if (iter == mOwnedSounds.end())
	{
		std::set<OWObject*> s; s.insert(o);
		mOwnedSounds[soundType] = s;
	}
	else
	{
		iter->second.insert(o);
	}		
}
*/

void SoundManager::setLocation(int soundId, const glm::vec3& v)
{
}

void SoundManager::setVolume(int soundId, float volume)
{
}

void SoundManager::playSound(int soundId, const glm::vec3& soundPosition, const glm::vec3& listenerPosition)
{
	ma_playSound(soundId);
}

SoundManager::SoundCache SoundManager::mLoadedFiles;

const std::string& SoundManager::getSound(const std::string& fileName)
{
	// TODO: add unique ptr and test for cache.
	// TODO: Do not store istream* as this keeps the file locked.
	if (!fileName.size())
		throw NMSLogicException(std::stringstream()
			<< "Empty string passed to SoundManager::getSound()");

	//std::lock_guard<std::mutex> guard(mut);


	std::filesystem::path path =
		ResourcePathFactory().appendPath(fileName,
			ResourcePathFactory::ResourceType::Sound);

	SoundCache::iterator iter = mLoadedFiles.begin();
	while (iter != mLoadedFiles.end())
	{
		if (std::filesystem::equivalent(iter->first, path))
			break;
		++iter;
	}
	if (iter != mLoadedFiles.end())
	{
		return iter->second;
	}
	std::ifstream f(path, std::ios::in | std::ios::binary);

	// ensure ifstream objects can throw exceptions:
	f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// https://cpluspluspedia.com/en/tutorial/496/file-i-o
	// https://stackoverflow.com/questions/1042940/writing-directly-to-stdstring-internal-buffers
	// https://stackoverflow.com/questions/11149665/c-vector-that-doesnt-initialize-its-members?noredirect=1&lq=1
	// https://stackoverflow.com/questions/17888569/how-can-i-switch-between-fstream-files-without-closing-them-simultaneous-output
	std::string fileContents;
	f.seekg(0, std::ios::end);
	std::streampos sz = f.tellg();
	fileContents.reserve(sz);
	f.seekg(0, std::ios::beg);

	// apparently read is a LOT faster than wholeFile.assign but 
	// it does not read the whole file
	fileContents.assign(std::istreambuf_iterator<char>(f),
		std::istreambuf_iterator<char>());
	//f.read(&(wholeFile[0]), sz);

	if (!f.is_open())
		throw NMSException(std::stringstream() <<
			"Could not find Sound [" << path.string() << "].\n");
	auto ret = mLoadedFiles.insert({ path, fileContents });
	return ret.first->second;
}

