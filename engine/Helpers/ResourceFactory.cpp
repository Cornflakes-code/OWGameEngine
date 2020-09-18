#include "ResourceFactory.h"

#include <iostream>
#include <fstream>

#include "ErrorHandling.h"

static ResourceFactory* mFactory = nullptr;
unsigned int gCurrentTexture = 0;

unsigned int ResourceFactory::nextTexture()
{
	return gCurrentTexture++;
}

std::string ResourceFactory::toString(ResourceType rt)
{
	switch (rt)
	{
		case ResourceType::Shader: return "Shader";
		case ResourceType::Font: return "Font";
		case ResourceType::Texture: return "Texture";
		case ResourceType::UnknownType: return "UnknownType";
		default: return "Internal logic error";
	}	
}

ResourceFactory* ResourceFactory::getResourceFactory()
{
	if (!mFactory)
	{
		mFactory = new ResourceFactory();
		mFactory->addPath(std::experimental::filesystem::current_path(), ResourceType::UnknownType);
	}
	return mFactory;
}

ResourceFactory::ResourceFactory()
{

}

void ResourceFactory::addPath(const std::experimental::filesystem::path& path, ResourceType key)
{
	std::pair<std::map<ResourceFactory::ResourceType,
			std::set<std::experimental::filesystem::path>>::iterator, bool> iter;
	iter.first = mResourcePaths.find(key);
	if (iter.first == mResourcePaths.end())
	{
		std::pair<std::map<
			std::string, 
			std::set<std::experimental::filesystem::path>>::iterator, bool> it;
		std::set<std::experimental::filesystem::path> newSet;
		newSet.insert(path);
		mResourcePaths[key] = newSet;
	}
	else
	{
		// found map[key]. Add the path to the key
		iter.first->second.insert(path);
	}
}

std::experimental::filesystem::path 
ResourceFactory::appendPath(const std::string& fileName, ResourceType key)
{
	// First search for a set of paths identified by the key. If that is not found 
	// then get the set of paths without a key identifier.
	auto it = mResourcePaths.find(key);
	if (it == mResourcePaths.end())
	{
		if (key != ResourceType::UnknownType)
			it = mResourcePaths.find(ResourceType::UnknownType);
	}
	if (it == mResourcePaths.end())
	{
		// No path of this type, nor a path of UnknownType, found
		throw NMSException(std::stringstream() << "Fatal Error: No resource path of type ["
			<< toString(key) << "] found\n.");
	}
		// now search for the file in every found path.
	for (auto& p : it->second)
	{
		std::experimental::filesystem::path p1 = p / fileName;
		if (std::experimental::filesystem::is_regular_file(p1))
		{
			return p1;
		}			
	}
	// It may be a fully qualified path anyway.
	if (std::experimental::filesystem::is_regular_file(fileName))
	{
		return fileName;
	}
	std::experimental::filesystem::path p1 = fileName;

	p1.remove_filename();
	if (!std::experimental::filesystem::is_directory(p1))
	{
		throw NMSException(std::stringstream() << "Fatal Error: Directory ["
			<< p1 << "] not found.\n");
	}

	throw NMSException(std::stringstream() << "Fatal Error: File ["
		<< fileName << "] not found in directories:\n");
	return std::experimental::filesystem::path();
}

json ResourceFactory::get(const std::string& fileName, const std::string& jsonObj, bool cache)
{
	//auto it = loadedFiles.find(fileName);
	//if (it == loadedFiles.end())
	//{
	//	std::ifstream ifs(fileName);
	//	it->second = nlohmann::json::parse(ifs);
	//}
	//return it->second[jsonObj];
	return nullptr;
}

const std::string& ResourceFactory::getPath(const std::string& fileName, ResourceType rt)
{
	// TODO: add unique ptr and test for cache.
	// TODO: Do not store istream* as this keeps the file locked.
	if (!fileName.size())
		throw NMSLogicException(std::stringstream() << "Empty string passed to ResourceFactory::get()");

	//std::lock_guard<std::mutex> guard(mut);
	

	std::experimental::filesystem::path p = appendPath(fileName, rt);
	std::map<std::experimental::filesystem::path, std::string>::iterator it = mLoadedFiles.find(p);
	if (it == mLoadedFiles.end())
	{
		std::ifstream f(p, std::ios::in | std::ios::binary);

		// ensure ifstream objects can throw exceptions:
		f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// https://cpluspluspedia.com/en/tutorial/496/file-i-o
		// https://stackoverflow.com/questions/1042940/writing-directly-to-stdstring-internal-buffers
		// https://stackoverflow.com/questions/11149665/c-vector-that-doesnt-initialize-its-members?noredirect=1&lq=1
		// https://stackoverflow.com/questions/17888569/how-can-i-switch-between-fstream-files-without-closing-them-simultaneous-output
		std::string wholeFile;
		f.seekg(0, std::ios::end);
		std::streampos sz = f.tellg();
		wholeFile.reserve(sz);
		f.seekg(0, std::ios::beg);

		// apparently read is a lot faster than wholeFile.assign but it does not read the whole file
		wholeFile.assign(std::istreambuf_iterator<char>(f),	std::istreambuf_iterator<char>());
		//f.read(&(wholeFile[0]), sz);

		if (!f.is_open())
			throw NMSException(std::stringstream() << 
					"Could not find Resource [" << p.string() << "].\n");
		auto ret = mLoadedFiles.insert(std::pair<std::experimental::filesystem::path, std::string>(p, wholeFile));
		return ret.first->second;
	}
	return it->second;
}

std::istream* ResourceFactory::readFile(const std::string& path)
{
	if (!path.size())
		throw NMSException(std::stringstream() << "Resource [" << path << "] not found");
	std::ifstream* f = new std::ifstream(path);

	// ensure ifstream objects can throw exceptions:
	f->exceptions(std::ifstream::failbit | std::ifstream::badbit);

	f->open(path);
	return f;
}

const FreeTypeFontAtlas::FontDetails* ResourceFactory::loadFreeTypeFont(
				const std::string& fileName, unsigned int fontHeight)
{
	std::experimental::filesystem::path p
		= appendPath(fileName, ResourceFactory::ResourceType::Font);
	return mFreeType.loadFont(p, fontHeight);
}

std::string& ResourceFactory::boilerPlateVertexShader()
{
	static std::string s = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 pvm;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = pvm * vec4(aPos, 1.0);\n"
		"}\0";
	return s;
}

std::string& ResourceFactory::boilerPlateFragmentShader()
{
	static std::string s = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 colour = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"void main()\n"
		"{\n"
		"   FragColor = colour;\n"
		"}\n\0";
	return s;
}

std::string& ResourceFactory::boilerPlateGeometryShader()
{
	static std::string s = "";
	return s;
}
