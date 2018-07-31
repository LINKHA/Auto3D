#include "SpriteSort.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteTranslucent.h"
namespace Auto3D {

SpriteSort::SpriteSort(Ambient* ambient)
	:Super(ambient)
{
}


SpriteSort::~SpriteSort()
{
}
void SpriteSort::AddSprite(SpriteTranslucent * sprite)
{
	_sprites.push_back(sprite);
}
void SpriteSort::ComputeMap()
{
	Camera& camera = GetSubSystem<Renderer>()->GetCurrentCamera();
	for (unsigned int i = 0; i < _sprites.size(); i++)
	{
		float distance = glm::length(camera.GetPosition() - _sorted[i]->GetGameObject().GetComponent(Transform).GetPosition().ToGLM());
		_sorted[distance] = _sprites[i];
	}
}

//reverse_iterator to render target
void SpriteSort::RenderSprite()
{
	Camera& camera = GetSubSystem<Renderer>()->GetCurrentCamera();
	for (unsigned int i = 0; i < _sprites.size(); i++)
	{
		//Print(t_camera.GetPosition().z);
		float distance = glm::length(camera.GetPosition() - _sprites[i]->GetGameObject().GetComponent(Transform).GetPosition().ToGLM());

		_sorted[distance] = _sprites[i];
	}
	for (AUTO_MAP(float, SpriteTranslucent*)::reverse_iterator it = _sorted.rbegin(); it != _sorted.rend(); ++it)
	{
		it->second->GetGameObject().GetComponent(Transform).UpdateTransform();
		it->second->DrawTranslucentSprite();
		it->second->GetGameObject().GetComponent(Transform).Identity();
	}
	_sorted.clear();
}
}