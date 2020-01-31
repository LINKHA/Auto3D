#include "DefaultController.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"
#include "Platform/PlatformDef.h"
#include "Platform/cmd.h"
#include "Platform/input.h"
#include "Gameplay/WorldContext.h"
#include "Gameplay/World.h"

namespace Auto3D
{

int CameraCmdMove(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
{
	AWorld* world = FWorldContext::Get().GetActiveWorld();
	TVector<ACameraComponent*>& cameras = world->GetCameras();
	for (auto it = cameras.Begin(); it != cameras.End(); ++it)
	{
		ACameraComponent* camera = dynamic_cast<ACameraComponent*>(*it);

		if (_argc > 1)
		{
			if (0 == bx::strCmp(_argv[1], "forward"))
			{
				camera->SetKeyState(CAMERA_KEY_FORWARD, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "left"))
			{
				camera->SetKeyState(CAMERA_KEY_LEFT, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "right"))
			{
				camera->SetKeyState(CAMERA_KEY_RIGHT, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "backward"))
			{
				camera->SetKeyState(CAMERA_KEY_BACKWARD, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "up"))
			{
				camera->SetKeyState(CAMERA_KEY_UP, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "down"))
			{
				camera->SetKeyState(CAMERA_KEY_DOWN, true);
				return 0;
			}
		}
	}
	return 1;
}

static void CameraCmd(const void* _userData)
{
	CmdExec((const char*)_userData);
}

static const InputBinding camBindings[] =
{
	{ Auto3D::Key::KeyW,             Auto3D::Modifier::None, 0, CameraCmd, "move forward"  },
	{ Auto3D::Key::GamepadUp,        Auto3D::Modifier::None, 0, CameraCmd, "move forward"  },
	{ Auto3D::Key::KeyA,             Auto3D::Modifier::None, 0, CameraCmd, "move left"     },
	{ Auto3D::Key::GamepadLeft,      Auto3D::Modifier::None, 0, CameraCmd, "move left"     },
	{ Auto3D::Key::KeyS,             Auto3D::Modifier::None, 0, CameraCmd, "move backward" },
	{ Auto3D::Key::GamepadDown,      Auto3D::Modifier::None, 0, CameraCmd, "move backward" },
	{ Auto3D::Key::KeyD,             Auto3D::Modifier::None, 0, CameraCmd, "move right"    },
	{ Auto3D::Key::GamepadRight,     Auto3D::Modifier::None, 0, CameraCmd, "move right"    },
	{ Auto3D::Key::KeyQ,             Auto3D::Modifier::None, 0, CameraCmd, "move down"     },
	{ Auto3D::Key::GamepadShoulderL, Auto3D::Modifier::None, 0, CameraCmd, "move down"     },
	{ Auto3D::Key::KeyE,             Auto3D::Modifier::None, 0, CameraCmd, "move up"       },
	{ Auto3D::Key::GamepadShoulderR, Auto3D::Modifier::None, 0, CameraCmd, "move up"       },

	INPUT_BINDING_END
};

ADefaultController::ADefaultController()
{
	CmdAdd("move", CameraCmdMove);
	InputAddBindings("camBindings", camBindings);
}
ADefaultController::~ADefaultController()
{
	InputRemoveBindings("camBindings");
}

void ADefaultController::BeginPlay()
{
	Super::BeginPlay();
}

void ADefaultController::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
	AActor* owner = GetOwner();
	if (owner)
	{
		ACameraComponent* camera = owner->FindComponent<ACameraComponent>();
		//camera->Update(deltaTime);
	}
}
}