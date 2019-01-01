#include "DebugSapce.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "level_0.h"

int runApplication() 
{ 
	SharedPtr<Auto3D::Ambient> ambient(new Auto3D::Ambient()); 
	DebugSapce work(ambient);
	return work.Run();
} 

SELECT_DEDICATED_GRAPHICS_CARD 
int main(int argc, char** argv) 
{ 
	DETECT_MEMORY_LEAKS(); 
	int flag = runApplication();
	_CrtDumpMemoryLeaks(); 
	return flag; 
}


void DebugSapce::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void DebugSapce::Destruct()
{
	RemoveScene(0);
}