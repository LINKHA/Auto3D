#include "TextureSpace.h"
#include "Sprite.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "TimeManager.h"
#include "Mesh.h"
#include "LightPoint.h"
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

	Sprite* tex1 = new Sprite("Resource/texture/square.jpg");
	te_obj = new GameObject();
	tex1->SetColor(Color(0.5f, 0.5f, 0.5f));
	te_obj->AddComponent(tex1);

	Sprite * tex2 = new Sprite("Resource/texture/window.png");
	tex2->EnableBlend(true);
	tex2->EnableDepth(false);
	GameObject * obj2 = new GameObject();
	obj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 0.0f);
	obj2->AddComponent(tex2);

	Sprite * tex3 = new Sprite("Resource/texture/window.png");
	tex3->EnableBlend(true);
	tex3->EnableDepth(false);
	GameObject * obj3 = new GameObject();
	obj3->GetComponent(Transform).SetPosition(0.2f, 0.0f, -1.0f);
	obj3->AddComponent(tex3);

	Sprite * tex4 = new Sprite("Resource/texture/window.png");
	tex4->EnableBlend(true);
	tex4->EnableDepth(false);
	GameObject * obj4 = new GameObject();
	obj4->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -2.0f);
	obj4->AddComponent(tex4);

	Sprite * tex5 = new Sprite("Resource/texture/grass.png");
	tex5->EnableBlend(true);
	GameObject * obj5 = new GameObject();
	obj5->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -3.0f);
	obj5->AddComponent(tex5);

	Sprite * tex6 = new Sprite("Resource/texture/grass.png");
	tex6->EnableBlend(true);
	GameObject * obj6 = new GameObject();
	obj6->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -4.0f);
	obj6->AddComponent(tex6);

	GameObject * lightObj = new GameObject();
	Light * light = new LightPoint();
	lightObj->AddComponent(light);
	//////////////////////////////////////////////////////////////////////////
	Mesh * mesh = new Mesh("Resource/object/base/Cube.FBX");
	mesh->GetMaterial().color.Set(0.5f, 0.8f, 0.3f);
	//mesh->GetMaterial().SetImage("Resource/texture/window.png");
	GameObject * meshObj = new GameObject();
	meshObj->GetComponent(Transform).SetPosition(1.0f, 0.0f, 0.0f);
	meshObj->AddComponent(mesh);
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
