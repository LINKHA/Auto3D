#include "Core/ProcessUtils.h"
#include "IO/File.h"
#include "Thread/Thread.h"
#include "Time/Time.h"
#include "Core/Modules/ModuleManager.h"
#include "Event/EventManager.h"
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

FLogModule::FLogModule() :
#ifdef _DEBUG
    _level(LOG_DEBUG),
#else
    _level(LOG_INFO),
#endif
    _timeStamp(false),
    _inWrite(false),
    _quiet(false)
{
}

FLogModule::~FLogModule()
{
    Close();
}

void FLogModule::Open(const FString& fileName)
{
    if (fileName.IsEmpty())
        return;
    
    if (_logFile && _logFile->IsOpen())
    {
        if (_logFile->GetName() == fileName)
            return;
        else
            Close();
    }

    _logFile = new FFile();
    if (_logFile->Open(fileName, EFileMode::WRITE))
        InfoString("Opened log file " + fileName);
    else
    {
        _logFile.Reset();
        ErrorString("Failed to create log file " + fileName);
    }
}

void FLogModule::Close()
{
    if (_logFile && _logFile->IsOpen())
    {
        _logFile->Close();
        _logFile.Reset();
    }
}

void FLogModule::SetLevel(int newLevel)
{
    assert(newLevel >= LOG_DEBUG && newLevel < LOG_NONE);

    _level = newLevel;
}

void FLogModule::SetTimeStamp(bool enable)
{
    _timeStamp = enable;
}

void FLogModule::SetQuiet(bool enable)
{
    _quiet = enable;
}

void FLogModule::EndFrame()
{
    FMutexLock lock(_logMutex);

    // Process messages accumulated from other threads (if any)
    while (!_threadMessages.IsEmpty())
    {
        const FStoredLogMessage& stored = _threadMessages.Front();

        if (stored._level != LOG_RAW)
            Write(stored._level, stored._message);
        else
            WriteRaw(stored._message, stored._error);

        _threadMessages.PopFront();
    }
}

void FLogModule::Write(int msgLevel, const FString& message)
{
    assert(msgLevel >= LOG_DEBUG && msgLevel < LOG_NONE);
    
    FLogModule* instance = GModuleManager::Get().LogModule();
    if (!instance)
        return;

    // If not in the main thread, store message for later processing
    if (!FThread::IsMainThread())
    {
        FMutexLock lock(instance->_logMutex);
        instance->_threadMessages.Push(FStoredLogMessage(message, msgLevel, false));
        return;
    }

    // Do not log if message level excluded or if currently sending a log _event
    if (instance->_level > msgLevel || instance->_inWrite)
        return;

    FString formattedMessage = logLevelPrefixes[msgLevel];
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

    FLogMessageEvent& event = instance->_logMessageEvent;
    event._message = formattedMessage;
    event._level = msgLevel;
	GEventManager::Get().SendEvent(instance, event);

    instance->_inWrite = false;
}

void FLogModule::WriteRaw(const FString& message, bool error)
{
	FLogModule* instance = GModuleManager::Get().LogModule();
    if (!instance)
        return;

    // If not in the main thread, store message for later processing
    if (!FThread::IsMainThread())
    {
        FMutexLock lock(instance->_logMutex);
        instance->_threadMessages.Push(FStoredLogMessage(message, LOG_RAW, error));
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

    FLogMessageEvent& event = instance->_logMessageEvent;
    event._message = message;
    event._level = error ? LOG_ERROR : LOG_INFO;
	GEventManager::Get().SendEvent(instance, event);

    instance->_inWrite = false;
}

}
