#include <bx/allocator.h>
#include <bx/commandline.h>
#include <bx/hash.h>
#include <bx/string.h>

#include "dbg.h"
#include "cmd.h"
#include "PlatformDef.h"

#include <tinystl/allocator.h>
#include <tinystl/string.h>
#include <tinystl/unordered_map.h>
namespace stl = tinystl;

namespace Auto3D
{

struct CmdContext
{
	CmdContext()
	{
	}

	~CmdContext()
	{
	}

	void Add(const char* name, ConsoleFn fn, void* userData)
	{
		uint32_t cmd = bx::hash<bx::HashMurmur2A>(name, (uint32_t)bx::strLen(name));
		BX_CHECK(m_lookup.end() == m_lookup.find(cmd), "Command \"%s\" already exist.", name);
		Func func = { fn, userData };
		m_lookup.insert(stl::make_pair(cmd, func));
	}

	void exec(const char* _cmd)
	{
		for (bx::StringView next(_cmd); !next.isEmpty(); _cmd = next.getPtr())
		{
			char commandLine[1024];
			uint32_t size = sizeof(commandLine);
			int argc;
			char* argv[64];
			next = bx::tokenizeCommandLine(_cmd, commandLine, size, argc, argv, BX_COUNTOF(argv), '\n');
			if (argc > 0)
			{
				int err = -1;
				uint32_t cmd = bx::hash<bx::HashMurmur2A>(argv[0], (uint32_t)bx::strLen(argv[0]));
				CmdLookup::iterator it = m_lookup.find(cmd);
				if (it != m_lookup.end())
				{
					Func& fn = it->second;
					err = fn.m_fn(this, fn.m_userData, argc, argv);
				}

				switch (err)
				{
				case 0:
					break;

				case -1:
				{
					stl::string tmp(_cmd, next.getPtr() - _cmd - (next.isEmpty() ? 0 : 1));
					DBG("Command '%s' doesn't exist.", tmp.c_str());
				}
				break;

				default:
				{
					stl::string tmp(_cmd, next.getPtr() - _cmd - (next.isEmpty() ? 0 : 1));
					DBG("Failed '%s' err: %d.", tmp.c_str(), err);
				}
				break;
				}
			}
		}
	}

	struct Func
	{
		ConsoleFn m_fn;
		void* m_userData;
	};

	typedef stl::unordered_map<uint32_t, Func> CmdLookup;
	CmdLookup m_lookup;
};

static CmdContext* cmdContext;

void CmdInit()
{
	cmdContext = BX_NEW(Auto3D::getAllocator(), CmdContext);
}

void CmdShutdown()
{
	BX_DELETE(Auto3D::getAllocator(), cmdContext);
}

void CmdAdd(const char* name, ConsoleFn fn, void* userData)
{
	cmdContext->Add(name, fn, userData);
}

void CmdExec(const char* format, ...)
{
	char tmp[2048];

	va_list argList;
	va_start(argList, format);
	bx::vsnprintf(tmp, BX_COUNTOF(tmp), format, argList);
	va_end(argList);

	cmdContext->exec(tmp);
}
}