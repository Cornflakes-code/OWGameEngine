#include "LogStream.h"

#include <iostream>
#include <fstream>
#include <chrono>

LogStream::ComposeBuffer LogStream::myBuffer;

std::ofstream* gLogFile = nullptr;

LogStream::~LogStream()
{
	//(*this) << std::flush; // does not work
	myBuffer.flushBuffers();
	//	(*gLogFile) << std::flush; // works but tacky
}

std::string nowAsString()
{
	std::time_t now_c
		= std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm t;
	localtime_s(&t, &now_c);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "config_%H-%M-%S.json", &t);
	return std::string(buffer);
}

std::ostream* LogStream::logFile() { return gLogFile; }

void LogStream::setLogFile(const std::experimental::filesystem::path& path)
{
	if (gLogFile)
	{
		delete gLogFile;
	}
	std::experimental::filesystem::path p = path;
	// p.has_filename() only checks for trailing slash.
	if (!p.has_extension())
	{
		p.append(nowAsString());
	}
	gLogFile = new std::ofstream(p);
	LogStream::linkStream();
}

std::streambuf* getLogBuffer()
{
	if (!gLogFile)
	{
		std::experimental::filesystem::path p =
			std::experimental::filesystem::current_path();
		p.append(nowAsString());
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

