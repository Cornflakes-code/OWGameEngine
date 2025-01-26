#include "LogStream.h"

#include <iostream>
#include <fstream>
#include <chrono>

#include "CommonUtils.h"

LogStream::ComposeBuffer LogStream::myBuffer;

std::ofstream* gLogFile = nullptr;

LogStream::~LogStream()
{
	//(*this) << std::flush; // does not work
	myBuffer.flushBuffers();
	//	(*gLogFile) << std::flush; // works but tacky
}

std::ostream* LogStream::logFile() { return gLogFile; }

void LogStream::setLogFile(const std::filesystem::path& path)
{
	if (gLogFile)
	{
		delete gLogFile;
	}
	std::filesystem::path p = path;
	// p.has_filename() only checks for trailing slash.
	if (!p.has_extension())
	{
		p.append(OWUtils::nowAsString());
	}
	gLogFile = new std::ofstream(p);
	LogStream::linkStream();
}

std::streambuf* getLogBuffer()
{
	if (!gLogFile)
	{
		std::filesystem::path p =
			std::filesystem::current_path();
		p.append(OWUtils::nowAsString());
		gLogFile = new std::ofstream(p);
	}
	return gLogFile->rdbuf();
}

void LogStream::closeLogFile()
{
	delete gLogFile;
}

//	static void linkStream(std::ostream& out)
void LogStream::linkStream()
{
	myBuffer.addBuffer(gLogFile->rdbuf());
	myBuffer.addBuffer(std::cout.rdbuf());
}

std::ostream& operator<<(std::ostream& os, const glm::vec4& v)
{
	os << "[" << v.x << ", " << v.y << ", " << v.z << v.w << "]";
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& v)
{
	os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& v)
{
	os << "[" << v.x << ", " << v.y << ", " << "]";
	return os;
}


std::istream& operator>>(std::istream& is, glm::vec4& v)
{
	char dummy;
	is >> v.x >> dummy >> v.y >> dummy >> v.z >> dummy >> v.w;
	return is;
}

std::istream& operator>>(std::istream& is, glm::vec3& v)
{
	char dummy;
	is >> v.x >> dummy >> v.y >> dummy >> v.z;
	return is;
}

std::istream& operator>>(std::istream& is, glm::vec2& v)
{
	char dummy;
	is >> v.x >> dummy >> v.y;
	return is;
}
