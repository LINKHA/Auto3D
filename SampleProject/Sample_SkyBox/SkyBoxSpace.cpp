#include "SkyBoxSpace.h"
#include "Config.h"
#include "Level_0.h"
#include "ResourceSystem.h"
#include "FileSystem.h"

//AUTO_APPLICATION_MAIN(SkyBoxSpace)

int runApplication() 
{ 
	RegisterMainThread();
    SharedPtr<Auto3D::Ambient> ambient(new Auto3D::Ambient()); 
	SkyBoxSpace app(ambient);
    return app.Run(); 
} 
SELECT_DEDICATED_GRAPHICS_CARD 
int main(int argc, char** argv) 
{ 
DETECT_MEMORY_LEAKS(); 
int flag = runApplication();
_CrtDumpMemoryLeaks(); 
return flag; 
}

void SkyBoxSpace::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void SkyBoxSpace::Start()
{

}
void SkyBoxSpace::Stop()
{
}