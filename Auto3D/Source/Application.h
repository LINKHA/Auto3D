#pragma once
#include "AutoConfig.h"
#include "Container/String.h"
#include "Platform/PlatformDef.h"
#include "Engine/Engine.h"

#include <bx/mutex.h>
#include <bx/thread.h>

#include <memory>

extern "C" int Auto3D_main(int argc, char** argv);

#define AUTO3D_IMPLEMENT_MAIN(_App, ...)\
int Auto3D_main(int argc, char** argv)\
{\
	_App app(__VA_ARGS__);\
	return Auto3D::runApp(&app, argc, argv);\
}

namespace Auto3D
{

class __declspec(novtable) IAppInstance
{
public:
	///
	IAppInstance(const char* _name, const char* _description, const char* _url = "https://bkaradzic.github.io/bgfx/index.html");

	///
	virtual ~IAppInstance() = 0;

	///
	virtual void init(uint32_t _width, uint32_t _height) = 0;

	///
	virtual int  shutdown() = 0;

	///
	virtual bool update() = 0;

	///
	const char* getName() const;

	///
	const char* getDescription() const;

	///
	const char* getUrl() const;

	///
	IAppInstance* getNext();

	IAppInstance* m_next;

	static IAppInstance* getFirstApp()
	{
		return s_apps;
	}

	static uint32_t getNumApps()
	{
		return s_numApps;
	}

	static IAppInstance*    s_currentApp;
	static IAppInstance*    s_apps;
	static uint32_t s_numApps;


	static char s_restartArgs[1024];
private:
	const char* m_name;
	const char* m_description;
	const char* m_url;
};

///
int RunAppInstance(IAppInstance* app, int argc, const char* const* argv);


/// The superclass implementation of the project space, where the engine is implemented
class AUTO_API FApplication
{
public:
	FApplication();
	~FApplication();

	/// This is AEngine important funcation init awake runloop and finish run
	int Run();
	/// Show an error message (last log message if empty), terminate the main loop, and set failure exit code.
	void ErrorExit(const FString& message = FString::EMPTY);

private:
	/// Auto3D AEngine
	std::unique_ptr<FEngine> _engine;

	/// Collected startup error log messages.
	FString _startupErrors;
	/// AApplication exit code.
	int _exitCode;

	static int32_t MainThreadFunc(bx::Thread* thread, void* userData);

	bx::Thread _mainThread;
};

}