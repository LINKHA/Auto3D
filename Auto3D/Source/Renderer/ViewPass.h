#pragma once
#include "AutoConfig.h"
#include "Container/Vector.h"
#include "Container/Singleton.h"
#include "Container/HashSet.h"

#include <bgfx/bgfx.h>
namespace Auto3D
{

#define ConstViewID() GViewPass::Get().ConstViewId()
#define TempViewID()  GViewPass::Get().TempViewId()
#define OrdinaryViewID() GViewPass::Get().OrdinaryViewId()


class AUTO_API GViewPass
{
	REGISTER_SINGLETON(GViewPass)
public:
	GViewPass() {}
	~GViewPass(){}
	void TouchViewPass();

	static bgfx::ViewId ConstViewId();
	static bgfx::ViewId TempViewId();
	static bgfx::ViewId OrdinaryViewId();
private:
	static bgfx::ViewId GetNewId();
	/// This pass buffer is not automatically cleared.
	static THashSet<bgfx::ViewId> s_constViewPass;
	/// The pass buffer, which is used only once, is cleared last in the frame.
	static THashSet<bgfx::ViewId> s_tempViewPass;
	/// Each frame requires an updated pass buffer and this ID is reused.
	static THashSet<bgfx::ViewId> s_ordinaryViewPass;
};

}