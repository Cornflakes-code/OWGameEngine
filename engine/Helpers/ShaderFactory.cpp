#include "ShaderFactory.h"

#include <iostream>
#include <fstream>

#include "ErrorHandling.h"
#include "ResourcePathFactory.h"


#define GLSL(src) "#version 330 core\n" #src

ShaderFactory::ShaderCache ShaderFactory::mLoadedFiles;

const std::string& ShaderFactory::getShader(const std::string& fileName)
{
	// TODO: add unique ptr and test for cache.
	// TODO: Do not store istream* as this keeps the file locked.
	if (!fileName.size())
		throw NMSLogicException(std::stringstream()
			<< "Empty string passed to ResourcePathFactory::getShader()");

	//std::lock_guard<std::mutex> guard(mut);


	std::experimental::filesystem::path path =
		ResourcePathFactory().appendPath(fileName,
			ResourcePathFactory::ResourceType::Shader);

	ShaderCache::iterator iter = mLoadedFiles.begin();
	while (iter != mLoadedFiles.end())
	{
		if (std::experimental::filesystem::equivalent(iter->first, path))
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
			"Could not find Resource [" << path.string() << "].\n");
	auto ret = mLoadedFiles.insert({ path, fileContents });
	return ret.first->second;
}

const std::string& ShaderFactory::boilerPlateVertexShader()
{
	static std::string s =
		GLSL(layout(location = 0) in vec3 aPos;
	uniform mat4 pvm;
	void main()
	{
		gl_Position = pvm * vec4(aPos, 1.0);
	});
	return s;
}

const std::string& ShaderFactory::boilerPlateFragmentShader()
{
	static std::string s =
		GLSL(out vec4 FragColor;
	uniform vec4 colour = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	void main()
	{
		FragColor = colour;
	});
	return s;
}

const std::string& ShaderFactory::boilerPlateGeometryShader()
{
	static std::string s = "";
	return s;
}
