#include "TextureSpace.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "TimeManager.h"
Texture2D* tex;
GameObject* obj;

TextureSpace::TextureSpace()
{
}
TextureSpace::~TextureSpace()
{
}
void TextureSpace::Start()
{
	GameObject* camObj = new GameObject();
	FreeCamera* freeCamera = new FreeCamera();
	camObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(freeCamera);

	tex = new Texture2D("Resource/texture/square.jpg");
	obj = new GameObject();
	tex->SetColor(Color(0.5f, 0.5f, 0.5f));

	obj->AddComponent(tex);
}
void TextureSpace::Update()
{
	float scaleAmount = (float)sin(GrGetTime());
	obj->GetComponent(Transform).SetPosition(1.0f, 1.0f, 0.0f);
	obj->GetComponent(Transform).SetRotation(Vector3(0.0f, 0.0f, 90.0f));
	//obj->GetComponent(Transform).setRotation(-55.0f, Vector3::xAxis);
	obj->GetComponent(Transform).SetRotation(90.0f, Vector3::zAxis);
	obj->GetComponent(Transform).SetScale(Vector3(scaleAmount));
}
int TextureSpace::Launch()
{
	return INSTANCE(Application).Run();
}
