#include "TextureSpace.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "TimeManager.h"
Texture2D* te_tex;
GameObject* te_obj;

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

	te_tex = new Texture2D("Resource/texture/square.jpg");
	te_obj = new GameObject();
	te_tex->SetColor(Color(0.5f, 0.5f, 0.5f));

	te_obj->AddComponent(te_tex);
}
void TextureSpace::Update()
{
	float scaleAmount = (float)sin(GrGetTime());
	Transform& t = te_obj->GetComponent(Transform);
	t.SetPosition(1.0f, 1.0f, 0.0f);
	t.SetRotation(Vector3(0.0f, 0.0f, 90.0f));
	//obj->GetComponent(Transform).setRotation(-55.0f, Vector3::xAxis);
	t.SetRotation(90.0f, Vector3::zAxis);
	t.SetScale(Vector3(scaleAmount));
}
int TextureSpace::Launch()
{
	return INSTANCE(Application).Run();
}
