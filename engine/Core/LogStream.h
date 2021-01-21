#pragma once
#include <streambuf>
#include <ostream>
#include <string>
#include <functional>
#include <experimental/filesystem>

#include "../OWEngine/OWEngine.h"

// https://stackoverflow.com/questions/1760726/how-can-i-compose-output-streams-so-output-goes-multiple-places-at-once
// http://wordaligned.org/articles/cpp-streambufs
// https://stackoverflow.com/questions/1760726/how-can-i-compose-output-streams-so-output-goes-multiple-places-at-once

// And eventually I think I will go to 
// https://github.com/amrayn/easyloggingpp
// https://stackoverflow.com/questions/5028302/small-logger-class
// https://github.com/gabime/spdlog
enum LogStreamLevel {
	Error, LogicError, Warning, ImportantInfo, Info, Debug, Debug1, Debug2, Debug3, Debug4
};

class OWENGINE_API LogStream: public std::ostream
{
	struct OWENGINE_API ComposeBuffer : public std::streambuf
	{
		void addBuffer(std::streambuf* buf)
		{
			bufs.push_back(buf);
		}
		virtual int overflow(int c)
		{
#pragma warning( push )
#pragma warning( disable : 4244 )
			for (auto a : bufs)
				a->sputc(c);
#pragma warning( pop )
			return c;
		}
		void flushBuffers()
		{
			for (auto a : bufs)
				a->pubsync();
		}

	private:
#pragma warning( push )
#pragma warning( disable : 4251 )
		std::vector<std::streambuf*> bufs;
#pragma warning( pop )

	};
	static ComposeBuffer myBuffer;
public:
	LogStream(LogStreamLevel OW_UNUSED(level))
		:std::ostream(NULL)
	{
		std::ostream::rdbuf(&myBuffer);
	}
	~LogStream();
	static std::ostream* logFile();
	static void setLogFile(const std::experimental::filesystem::path& path);
	static void closeLogFile();
private:
	static void linkStream();
};