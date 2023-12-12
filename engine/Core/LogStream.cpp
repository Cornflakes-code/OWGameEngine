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

