#include "Launch.h"
#include "MultLightsSpace.h"
#include "TextureSpace.h"
USING_AUTO


#ifdef __OPENGL__
int main()
{


	TextureSpace work;
	//MultLightsSpace work;
	return work.Launch();
}

#endif //__OPENGL__