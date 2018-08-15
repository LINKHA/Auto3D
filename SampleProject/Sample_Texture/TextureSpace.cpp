#include "TextureSpace.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Time.h"
#include "SpriteTranslucent.h"
#include "Application.h"
#include "Transform.h"
#include "../FreeCamera.h"
#include "Graphics.h"
GameObject* te_obj;
TextureSpace::TextureSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}
TextureSpace::~TextureSpace()
{
}
void TextureSpace::Start()
{
	

	GameObject* camObj = new GameObject(_ambient);
	FreeCamera* freeCamera = new FreeCamera(_ambient);
	camObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(freeCamera);


	SpriteTranslucent * tex2 = new SpriteTranslucent(_ambient,"../Resource/texture/window.png");
	tex2->EnableBlend(true);
	//tex2->EnableDepth(false);
	GameObject * obj2 = new GameObject();
	obj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 0.0f);
	obj2->AddComponent(tex2);

	SpriteTranslucent * tex3 = new SpriteTranslucent(_ambient,"../Resource/texture/window.png");
	tex3->EnableBlend(true);
	//tex3->EnableDepth(false);
	GameObject * obj3 = new GameObject(_ambient);
	obj3->GetComponent(Transform).SetPosition(0.2f, 0.0f, -1.0f);
	obj3->AddComponent(tex3);

	SpriteTranslucent * tex4 = new SpriteTranslucent(_ambient,"../Resource/texture/window.png");
	tex4->EnableBlend(true);
	//tex4->EnableDepth(false);
	GameObject * obj4 = new GameObject(_ambient);
	obj4->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -2.0f);
	obj4->AddComponent(tex4);


	Sprite* tex1 = new Sprite(_ambient,"../Resource/texture/logo.png");
	te_obj = new GameObject(_ambient);
	//tex1->SetColor(Color(0.5f, 0.5f, 0.5f));
	te_obj->AddComponent(tex1);



	Sprite * tex5 = new Sprite(_ambient, "../Resource/texture/grass.png");
	//tex5->EnableBlend(true);
	GameObject * obj5 = new GameObject(_ambient);
	obj5->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -3.0f);
	obj5->AddComponent(tex5);

	Sprite * tex6 = new Sprite(_ambient, "../Resource/texture/grass.png");
	//tex6->EnableBlend(true);
	GameObject * obj6 = new GameObject(_ambient);
	obj6->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -4.0f);
	obj6->AddComponent(tex6);
	
}
void TextureSpace::Update()
{
	float scaleAmount = (float)sin(GetSubSystem<Time>()->GetCurTime());
	Transform& t = te_obj->GetComponent(Transform);

	t.SetPosition(1.0f, 1.0f, 0.0f);
	t.SetRotation(Vector3(0.0f, 0.0f, 90.0f));
	//obj->GetComponent(Transform).setRotation(-55.0f, Vector3::xAxis);
	t.SetRotation(90.0f, Vector3::zAxis);
	t.SetScale(Vector3(scaleAmount));

}

int TextureSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(TextureSpace)
