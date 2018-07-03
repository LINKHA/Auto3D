#include "TextureSpace.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "TimeManager.h"
#include "Mesh.h"
#include "BaseLight.h"
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

	Texture2D * tex2 = new Texture2D("Resource/texture/window.png");
	tex2->useBlend = true;
	tex2->useDepth = false;
	GameObject * obj2 = new GameObject();
	obj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 0.0f);
	obj2->AddComponent(tex2);

	Texture2D * tex3 = new Texture2D("Resource/texture/window.png");
	tex3->useBlend = true;
	tex3->useDepth = false;
	GameObject * obj3 = new GameObject();
	obj3->GetComponent(Transform).SetPosition(0.2f, 0.0f, -1.0f);
	obj3->AddComponent(tex3);

	Texture2D * tex4 = new Texture2D("Resource/texture/window.png");
	tex4->useBlend = true;
	tex4->useDepth = false;
	GameObject * obj4 = new GameObject();
	obj4->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -2.0f);
	obj4->AddComponent(tex4);

	Texture2D * tex5 = new Texture2D("Resource/texture/grass.png");
	tex5->useBlend = true;
	GameObject * obj5 = new GameObject();
	obj5->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -3.0f);
	obj5->AddComponent(tex5);

	Texture2D * tex6 = new Texture2D("Resource/texture/grass.png");
	tex6->useBlend = false;
	GameObject * obj6 = new GameObject();
	obj6->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -4.0f);
	obj6->AddComponent(tex6);
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
