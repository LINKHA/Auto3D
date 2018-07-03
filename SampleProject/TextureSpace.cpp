#include "TextureSpace.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "TimeManager.h"
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

	Texture2D* tex1 = new Texture2D("Resource/texture/square.jpg");
	te_obj = new GameObject();
	tex1->SetColor(Color(0.5f, 0.5f, 0.5f));
	te_obj->AddComponent(tex1);

	Texture2D * tex2 = new Texture2D("Resource/texture/grass.png");
	GameObject * obj = new GameObject();
	tex2->SetColor(Color(0.5f, 0.5f, 0.5f));
	obj->AddComponent(tex2);

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
