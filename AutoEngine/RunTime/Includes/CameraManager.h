#ifndef CAMERA_MANAGER_H_
#define CAMERA_MANAGER_H_
#include "GameManager.h"
#include "FPSCamera.h"
#include "Singleton.h"
AUTO_BEGIN
class CameraManager : public LevelGameManager , public Singleton<CameraManager>
{
	REGISTER_DERIVED_CLASS(CameraManager, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(CameraManager);
public:
	CameraManager();
	AUTO_HASH_MAP(int,FPSCamera*) CameraArray;
private:
};
AUTO_END
#endif //!CAMERA_MANAGER_H_