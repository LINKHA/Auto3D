#pragma once
namespace Auto3D
{
struct CmdContext;
typedef int(*ConsoleFn)(CmdContext* context, void* userData, int argc, char const* const* _rgv);

///
void CmdInit();

///
void CmdShutdown();

///
void CmdAdd(const char* name, ConsoleFn fn, void* userData = NULL);

///
void CmdExec(const char* format, ...);


}