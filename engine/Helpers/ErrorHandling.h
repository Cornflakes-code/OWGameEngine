#pragma once

#include <exception>
#include <sstream>
#include <string>

#include "../OWEngine/OWEngine.h"

enum class NMSErrorLevel
{ NMSInfo, NMSWarning, NMSCritical };

/*
	Errors in the Game Engine are handled by exceptions. All of these should
	be caught during development.
*/
#pragma warning( push )
#pragma warning( disable : 4275 )
class OWENGINE_API NMSException : public std::exception
{
public:
	NMSException(std::stringstream ss): NMSException(ss.str())
	{}

	NMSException(const std::string& msg): std::exception(msg.c_str())
	{}
};
#pragma warning( pop )

class OWENGINE_API NMSOpenGlException : public NMSException
{
public:
	NMSOpenGlException(const std::string& msg): NMSException(msg)
	{}
};

class OWENGINE_API NMSLogicException : public NMSException
{
public:
	NMSLogicException(std::stringstream ss) : NMSException(ss.str())
	{}

	NMSLogicException(const std::string& msg) : NMSException(msg)
	{}
};

class OWENGINE_API NMSNotYetImplementedException : public NMSException
{
public:
	NMSNotYetImplementedException(std::stringstream ss) : NMSException(ss.str())
	{}

	NMSNotYetImplementedException(const std::string& msg) : NMSException(msg)
	{}
};

void OWENGINE_API dumpMessage(const std::string& msg, NMSErrorLevel err);
void OWENGINE_API dumpMessage(const std::stringstream& ss, NMSErrorLevel err);

#define DEBUG_OPENGL

#ifdef DEBUG_OPENGL
void OWENGINE_API debugCheckGLError(const char *file, int line);

#define checkGLError() debugCheckGLError(__FILE__, __LINE__)
#else
#define checkGLError()
#endif
/*
	https://codereview.stackexchange.com/questions/245653/a-simple-error-messaging-and-logging-system-via-macros-in-c
class MessageDumpStream
{
public:
};

class MessageDump
{
	template<typename T>
	MessageDumpStream operator<<(T const& item)
	{
		std::cout << item;
		return *this;
	}
};
class LokiFileWriter;
class LokiFileWriterStream
{
	std::ofstream   file;
	friend class LokiFileWriter;

	// Private so only LokiFileWriter can create one.
	LokiFileWriterStream(std::ofstream&& output)
		: file(std::move(output))
	{}
public:
	LokiFileWriterStream(LokiFileWriterStream&& move) = default;
	template<typename T>
	LokiFileWriterStream& operator<<(T const& item)
	{
		// Send the T to the file stream.
		// Then return a reference to allow chaining
		file << item;
		return *this;
	}
	~LokiFileWriterStream()
	{
		// When the expression is closed
		// We will close the file stream.
		//
		// But remember that the move constructor is available
		// So objects that have been moved move the stream object
		// an object that has been moved from has a file object that
		// is no longer valid (calling close() will fail in some way)
		// but it is a valid object that we are allowed to call close on
		file.close();
	}
};
class LokiFileWriter
{
	std::string filename;
public:
	LokiFileWriter(std::string const& filename)
		: filename(filename)
	{}
	template<typename T>
	LokiFileWriterStream operator<<(T const& item)
	{
		// We create a stream object.
		LokiFileWriterStream stream(std::ofstream(filename, std::ios::app));
		stream << item;

		// The stream object is returned forcing a move
		// of the object to external calling frame.
		// This means the object inside this function may be
		// destroyed but the file object it contains has already been
		// moved and thus not destroyed.
		return stream;
	}
};

int main()
{
	LokiFileWriter  out("MyLogFile");
	// The first << creates the `LokiFileWriterStream`
	//    Each subsequent << returns a reference to the same object.
	out << "Test" << 123 << " Plop";
	//    At the end of the expression `LokiFileWriterStream` goes
	//    out of scope and we destroy the object which calls the
	//    destructor which then calls the close method.
}
*/