#pragma once
#include "AutoConfig.h"
#include "Container/String.h"
#include "Platform/PlatformDef.h"
#include "Engine/Engine.h"
#include "Container/Singleton.h"

#include <thread>
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
	IAppInstance(const char* name, const char* description, const char* url = "https://bkaradzic.github.io/bgfx/index.html");

	///
	virtual ~IAppInstance();

	///
	virtual void init(uint32_t _width, uint32_t _height) = 0;

	///
	virtual int  shutdown() = 0;

	///
	virtual bool update() = 0;

	///
	const char* getName() const { return _pathName; }

	///
	const char* getDescription() const { return _description; }

	///
	const char* getUrl() const { return _url; }

	///
	IAppInstance* getNext() { return _next; }

	static void sortApps();

	static IAppInstance* getFirstApp() { return _apps; }

	static uint32_t getNumApps() { return _numApps; }

	static IAppInstance* getNextWrap(IAppInstance* app);

	static IAppInstance* getCurrentApp(IAppInstance* set = NULL);

	IAppInstance* _next;

	static IAppInstance*    _currentApp;
	static IAppInstance*    _apps;
	static uint32_t _numApps;

	static char _restartArgs[1024];
private:
	const char* _pathName;
	const char* _description;
	const char* _url;

};




/// The superclass implementation of the project space, where the engine is implemented
class AUTO_API FApplication
{
	REGISTER_SINGLETON(FApplication)

public:
	FApplication();
	~FApplication();

	/// This is AEngine important funcation init awake runloop and finish run
	int Run();
	/// Show an error message (last log message if empty), terminate the main loop, and set failure exit code.
	void ErrorExit(const FString& message = FString::EMPTY);
	
	///
	int RunAppInstance(IAppInstance* app, int argc, const char* const* argv);

	static FString _currentDir;
private:
	/// Application main thread.Including engine workflow, app workflow
	int RunMainThread();


	/// Auto3D AEngine
	std::unique_ptr<FEngine> _engine;
	/// Collected startup error log messages.
	FString _startupErrors;
	/// AApplication exit code.
	int _exitCode;

};

}