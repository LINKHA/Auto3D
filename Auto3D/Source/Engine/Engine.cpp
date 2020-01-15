#include "Engine.h"

namespace Auto3D
{
/// Construct
FEngine::FEngine()
{

}
/// Cestructor
FEngine::~FEngine()
{

}
/// Init engine, Register some SubSystem
bool FEngine::Init()
{

	return true;
}
/// AEngine normal exit
void FEngine::Exit()
{

}
/// Render geometry
void FEngine::Render()
{

}
/// Sub system update data,  If pause when _minimized -mode return false
bool FEngine::Update()
{

	return true;
}
/// Frame finish
void FEngine::FrameFinish()
{

}
}