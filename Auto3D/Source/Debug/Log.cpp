#include "../Base/ProcessUtils.h"
#include "../IO/File.h"
#include "../Thread/Thread.h"
#include "../Time/Time.h"
#include "../Engine/ModuleManager.h"

#include "Log.h"

#include <cstdio>
#include <ctime>

#include "DebugNew.h"

namespace Auto3D
{

const char* logLevelPrefixes[] =
{
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    nullptr
};

Log::Log() :
#ifdef _DEBUG
    _level(LOG_DEBUG),
#else
    _level(LOG_INFO),
#endif
    _timeStamp(false),
    _inWrite(false),
    _quiet(false)
{
    RegisterModule(this);
}

Log::~Log()
{
    Close();
    RemoveModule(this);
}

void Log::Open(const String& fileName)
{
    if (fileName.IsEmpty())
        return;
    
    if (_logFile && _logFile->IsOpen())
    {
        if (_logFile->Name() == fileName)
            return;
        else
            Close();
    }

    _logFile = new File();
    if (_logFile->Open(fileName, FileMode::WRITE))
        InfoString("Opened log file " + fileName);
    else
    {
        _logFile.Reset();
        ErrorString("Failed to create log file " + fileName);
    }
}

void Log::Close()
{
    if (_logFile && _logFile->IsOpen())
    {
        _logFile->Close();
        _logFile.Reset();
    }
}

void Log::SetLevel(int newLevel)
{
    assert(newLevel >= LOG_DEBUG && newLevel < LOG_NONE);

    _level = newLevel;
}

void Log::SetTimeStamp(bool enable)
{
    _timeStamp = enable;
}

void Log::SetQuiet(bool enable)
{
    _quiet = enable;
}

void Log::EndFrame()
{
    MutexLock lock(_logMutex);

    // Process messages accumulated from other threads (if any)
    while (!_threadMessages.IsEmpty())
    {
        const StoredLogMessage& stored = _threadMessages.Front();

        if (stored._level != LOG_RAW)
            Write(stored._level, stored._message);
        else
            WriteRaw(stored._message, stored._error);

        _threadMessages.PopFront();
    }
}

void Log::Write(int msgLevel, const String& message)
{
    assert(msgLevel >= LOG_DEBUG && msgLevel < LOG_NONE);
    
    Log* instance = ModuleManager::Get().LogModule();
    if (!instance)
        return;

    // If not in the main thread, store message for later processing
    if (!Thread::IsMainThread())
    {
        MutexLock lock(instance->_logMutex);
        instance->_threadMessages.Push(StoredLogMessage(message, msgLevel, false));
        return;
    }

    // Do not log if message level excluded or if currently sending a log _event
    if (instance->_level > msgLevel || instance->_inWrite)
        return;

    String formattedMessage = logLevelPrefixes[msgLevel];
    formattedMessage += ": " + message;
    instance->_lastMessage = message;

    if (instance->_timeStamp)
        formattedMessage = "[" + TimeStamp() + "] " + formattedMessage;
    
    if (instance->_quiet)
    {
        // If in quiet mode, still print the error message to the standard error stream
        if (msgLevel == LOG_ERROR)
            PrintUnicodeLine(formattedMessage, true);
    }
    else
        PrintUnicodeLine(formattedMessage, msgLevel == LOG_ERROR);

    if (instance->_logFile)
    {
        instance->_logFile->WriteLine(formattedMessage);
        instance->_logFile->Flush();
    }

    instance->_inWrite = true;

    LogMessageEvent& event = instance->_logMessageEvent;
    event._message = formattedMessage;
    event._level = msgLevel;
    instance->SendEvent(event);

    instance->_inWrite = false;
}

void Log::WriteRaw(const String& message, bool error)
{
	Log* instance = ModuleManager::Get().LogModule();
    if (!instance)
        return;

    // If not in the main thread, store message for later processing
    if (!Thread::IsMainThread())
    {
        MutexLock lock(instance->_logMutex);
        instance->_threadMessages.Push(StoredLogMessage(message, LOG_RAW, error));
        return;
    }
    
    // Prevent recursion during log _event
    if (instance->_inWrite)
        return;

    instance->_lastMessage = message;

    if (instance->_quiet)
    {
        // If in quiet mode, still print the error message to the standard error stream
        if (error)
            PrintUnicode(message, true);
    }
    else
        PrintUnicode(message, error);

    if (instance->_logFile)
    {
        instance->_logFile->Write(message.CString(), message.Length());
        instance->_logFile->Flush();
    }

    instance->_inWrite = true;

    LogMessageEvent& event = instance->_logMessageEvent;
    event._message = message;
    event._level = error ? LOG_ERROR : LOG_INFO;
    instance->SendEvent(event);

    instance->_inWrite = false;
}

}
