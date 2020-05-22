#include "Renderer/ViewPass.h"

namespace Auto3D
{

THashSet<bgfx::ViewId> GViewPass::s_constViewPass;
THashSet<bgfx::ViewId> GViewPass::s_tempViewPass;
THashSet<bgfx::ViewId> GViewPass::s_ordinaryViewPass;
IMPLEMENT_SINGLETON(GViewPass)

void GViewPass::TouchViewPass()
{
	for (auto it = s_tempViewPass.Begin(); it != s_tempViewPass.End(); ++it)
	{
		bgfx::ViewId tempId = *it;
		bgfx::touch(tempId);
	}
	s_tempViewPass.Clear();
	s_ordinaryViewPass.Clear();
}

bgfx::ViewId GViewPass::ConstViewId()
{
	bgfx::ViewId id = GetNewId();
	s_constViewPass.Insert(id);

	return id;
}

bgfx::ViewId GViewPass::TempViewId()
{
	bgfx::ViewId id = GetNewId();
	s_tempViewPass.Insert(id);

	return id;
}

bgfx::ViewId GViewPass::OrdinaryViewId()
{
	bgfx::ViewId id = GetNewId();
	s_ordinaryViewPass.Insert(id);

	return id;
}

bgfx::ViewId GViewPass::GetNewId()
{
	bgfx::ViewId id = 0;
	for (;;)
	{
		//
		if (s_constViewPass.Find(id) == s_constViewPass.End() &&
			s_tempViewPass.Find(id) == s_tempViewPass.End() &&
			s_ordinaryViewPass.Find(id) == s_ordinaryViewPass.End())
		{
			break;
		}
		// When the ID is too large, it will pop up automatically
		if (id >= 10000)
			break;
		++id;
	}
	return id;
}

}