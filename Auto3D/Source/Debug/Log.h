#pragma once

#include "../Base/AutoPtr.h"
#include "../Base/List.h"
#include "../Thread/Mutex.h"
#include "../Object/Object.h"

namespace Auto3D
{

/// Fictional message level to indicate a stored raw message.
static const int LOG_RAW = -1;
/// Debug message level. By default only shown in debug mode.
static const int LOG_DEBUG = 0;
/// Informative message level.
static const int LOG_INFO = 1;
/// Warning message level.
static const int LOG_WARNING = 2;
/// Error message level.
static const int LOG_ERROR = 3;
/// Disable all log messages.
static const int LOG_NONE = 4;

class File;

/// Stored log message from another thread.
struct AUTO_API StoredLogMessage
{
    /// Construct undefined.
    StoredLogMessage()
    {
    }
    
    /// Construct with parameters.
    StoredLogMessage(const String& message_, int level_, bool error_) :
        message(message_),
        level(level_),
        error(error_)
    {
    }
    
    /// Message text.
    String message;
    /// Message level. -1 for raw messages.
    int level;
    /// Error flag for raw messages.
    bool error;
};

/// %Log message event.
class AUTO_API LogMessageEvent : public Event
{
public:
    /// Message.
    String message;
    /// Message level.
    int level;
};

/// Logging subsystem.
class AUTO_API Log : public Object
{
	REGISTER_OBJECT_CLASS(Log, Object)
public:
    /// Construct and register subsystem.
    Log();
    /// Destruct. Close the log file if open.
    ~Log();

    /// Open the log file.
    void Open(const String& fileName);
    /// Close the log file.
    void Close();
    /// Set logging level.
    void SetLevel(int newLevel);
    /// Set whether to timestamp log messages.
    void SetTimeStamp(bool enable);
    /// Set quiet mode, ie. only output error messages to the standard error stream.
    void SetQuiet(bool enable);
    /// Process threaded log messages at the end of a frame.
    void EndFrame();

    /// Return logging level.
    int Level() const { return level; }
    /// Return whether log messages are timestamped.
    bool HasTimeStamp() const { return timeStamp; }
    /// Return last log message.
    String LastMessage() const { return lastMessage; }

    /// Write to the log. If logging level is higher than the level of the message, the message is ignored.
    static void Write(int msgLevel, const String& message);
    /// Write raw output to the log.
    static void WriteRaw(const String& message, bool error = false);

    /// %Log message event.
    LogMessageEvent logMessageEvent;

private:
    /// Mutex for threaded operation.
    Mutex logMutex;
    /// %Log messages from other threads.
    List<StoredLogMessage> threadMessages;
    /// %Log file.
    AutoPtr<File> logFile;
    /// Last log message.
    String lastMessage;
    /// Logging level.
    int level;
    /// Use timestamps flag.
    bool timeStamp;
    /// In write flag to prevent recursion.
    bool inWrite;
    /// Quite mode flag.
    bool quiet;
};

}

#ifdef AUTO_LOGGING

#define LOGDEBUG(message) Auto3D::Log::Write(Auto3D::LOG_DEBUG, message)
#define LOGINFO(message) Auto3D::Log::Write(Auto3D::LOG_INFO, message)
#define LOGWARNING(message) Auto3D::Log::Write(Auto3D::LOG_WARNING, message)
#define LOGERROR(message) Auto3D::Log::Write(Auto3D::LOG_ERROR, message)
#define LOGRAW(message) Auto3D::Log::WriteRaw(message)
#define LOGDEBUGF(format, ...) Auto3D::Log::Write(Auto3D::LOG_DEBUG, Auto3D::String::Format(format, ##__VA_ARGS__))
#define LOGINFOF(format, ...) Auto3D::Log::Write(Auto3D::LOG_INFO, Auto3D::String::Format(format, ##__VA_ARGS__))
#define LOGWARNINGF(format, ...) Auto3D::Log::Write(Auto3D::LOG_WARNING, Auto3D::String::Format(format, ##__VA_ARGS__))
#define LOGERRORF(format, ...) Auto3D::Log::Write(Auto3D::LOG_ERROR, Auto3D::String::Format(format, ##__VA_ARGS__))
#define LOGRAWF(format, ...) Auto3D::Log::WriteRaw(Auto3D::String::Format(format, ##__VA_ARGS__))

#else

#define LOGDEBUG(message)
#define LOGINFO(message)
#define LOGWARNING(message)
#define LOGERROR(message)
#define LOGRAW(message)
#define LOGDEBUGF(format, ...)
#define LOGINFOF(format, ...)
#define LOGWARNINGF(format, ...)
#define LOGERRORF(format, ...)
#define LOGRAWF(format, ...)

#endif
