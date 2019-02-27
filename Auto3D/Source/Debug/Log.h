#pragma once

#include "../Base/AutoPtr.h"
#include "../Base/List.h"
#include "../Thread/Mutex.h"
#include "../Object/GameManager.h"

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
    StoredLogMessage(const String& message, int level, bool error) :
        _message(message),
        _level(level),
        _error(error)
    {
    }
    
    /// Message text.
    String _message;
    /// Message level. -1 for raw messages.
    int _level;
    /// Error flag for raw messages.
    bool _error;
};

/// %Log message _event.
class AUTO_API LogMessageEvent : public Event
{
public:
    /// Message.
    String _message;
    /// Message level.
    int _level;
};

/// Logging subsystem.
class AUTO_API Log : public Subsystem
{
	REGISTER_OBJECT_CLASS(Log, Subsystem)
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
    int Level() const { return _level; }
    /// Return whether log messages are timestamped.
    bool HasTimeStamp() const { return _timeStamp; }
    /// Return last log message.
    String LastMessage() const { return _lastMessage; }

    /// Write to the log. If logging level is higher than the level of the message, the message is ignored.
    static void Write(int msgLevel, const String& message);
    /// Write raw output to the log.
    static void WriteRaw(const String& message, bool error = false);

    /// %Log message _event.
    LogMessageEvent _logMessageEvent;

private:
    /// Mutex for threaded operation.
    Mutex _logMutex;
    /// %Log messages from other threads.
    List<StoredLogMessage> _threadMessages;
    /// %Log file.
    AutoPtr<File> _logFile;
    /// Last log message.
    String _lastMessage;
    /// Logging level.
    int _level;
    /// Use timestamps flag.
    bool _timeStamp;
    /// In write flag to prevent recursion.
    bool _inWrite;
    /// Quite mode flag.
    bool _quiet;
};

}

#ifdef AUTO_LOGGING

#define LogString(message) Auto3D::Log::Write(Auto3D::LOG_DEBUG, message)
#define InfoString(message) Auto3D::Log::Write(Auto3D::LOG_INFO, message)
#define WarinningString(message) Auto3D::Log::Write(Auto3D::LOG_WARNING, message)
#define ErrorString(message) Auto3D::Log::Write(Auto3D::LOG_ERROR, message)
#define LogRawString(message) Auto3D::Log::WriteRaw(message)
#define LogStringF(format, ...) Auto3D::Log::Write(Auto3D::LOG_DEBUG, Auto3D::String::Format(format, ##__VA_ARGS__))
#define InfoStringF(format, ...) Auto3D::Log::Write(Auto3D::LOG_INFO, Auto3D::String::Format(format, ##__VA_ARGS__))
#define WarnningStringF(format, ...) Auto3D::Log::Write(Auto3D::LOG_WARNING, Auto3D::String::Format(format, ##__VA_ARGS__))
#define ErrorStringF(format, ...) Auto3D::Log::Write(Auto3D::LOG_ERROR, Auto3D::String::Format(format, ##__VA_ARGS__))
#define LogRawStringF(format, ...) Auto3D::Log::WriteRaw(Auto3D::String::Format(format, ##__VA_ARGS__))

#else

#define LogString(_message)
#define InfoString(_message)
#define WarinningString(_message)
#define ErrorString(_message)
#define LogRawString(_message)
#define LogStringF(_format, ...)
#define InfoStringF(_format, ...)
#define WarnningStringF(_format, ...)
#define ErrorStringF(_format, ...)
#define LogRawStringF(_format, ...)

#endif
