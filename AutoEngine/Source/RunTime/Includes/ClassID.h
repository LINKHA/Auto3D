#pragma once
#define ClassID(x)		  CLASS_##x
#define DO_CLASS_ID(x,ID) ClassID(x) = ID,
namespace Auto3D {

enum ClassIDType
{
	DO_CLASS_ID(Default, -1)
	DO_CLASS_ID(Object, 0)
	DO_CLASS_ID(ObjectFactory, 1)

	DO_CLASS_ID(GameManager, 2)
	DO_CLASS_ID(LevelGameManager, 3)
	DO_CLASS_ID(GlobalGameManager, 4)
	DO_CLASS_ID(Time, 5)

	DO_CLASS_ID(Script, 10)
	DO_CLASS_ID(PreLoadManager, 11)

	DO_CLASS_ID(Node, 13)
	DO_CLASS_ID(GameObject, 14)

	DO_CLASS_ID(Component, 15)
	DO_CLASS_ID(Transform, 16)
	DO_CLASS_ID(MeshRenderer, 17)
	DO_CLASS_ID(Texture, 18)
	DO_CLASS_ID(Texture2D, 19)
	DO_CLASS_ID(Light, 20)
	DO_CLASS_ID(Camera, 21)
	DO_CLASS_ID(ScriptComponent, 22)
	DO_CLASS_ID(Texture3D, 23)
	DO_CLASS_ID(SkyBox, 24)


	DO_CLASS_ID(MotionSpace, 30)
	DO_CLASS_ID(Prefab, 31)

	DO_CLASS_ID(TextureMap, 32)
	DO_CLASS_ID(TextureNormal, 33)
	DO_CLASS_ID(TextureParallax, 34)
	DO_CLASS_ID(RenderComponent, 35)
	DO_CLASS_ID(SpriteRenderer, 36)

	DO_CLASS_ID(Engine, 40)
	DO_CLASS_ID(Graphics, 41)
	DO_CLASS_ID(Renderer, 42)
	DO_CLASS_ID(BaseSpace, 43)
	DO_CLASS_ID(Input, 45)
	DO_CLASS_ID(Geometry, 48)
	DO_CLASS_ID(ShadowRenderAssist, 52)
	DO_CLASS_ID(ShadowRenderer, 53)
	DO_CLASS_ID(LightContainer, 54)
	DO_CLASS_ID(OffScreen, 56)
	DO_CLASS_ID(MeshPBR, 57)
	DO_CLASS_ID(MeshPBRTexture, 58)
	DO_CLASS_ID(ResourceSystem, 60)
	DO_CLASS_ID(Material, 61)
	DO_CLASS_ID(Scene, 62)
	DO_CLASS_ID(IO, 63)
	DO_CLASS_ID(Model, 66)
	DO_CLASS_ID(LevelScene, 67)
	DO_CLASS_ID(BehaviorObject, 68)
	DO_CLASS_ID(Audio, 69)
	DO_CLASS_ID(UI, 70)

	DO_CLASS_ID(AudioListener, 72)
	DO_CLASS_ID(AudioSource, 73)
	DO_CLASS_ID(Timer, 74)

	DO_CLASS_ID(Physics, 75)
	DO_CLASS_ID(Collider2D, 76)
	DO_CLASS_ID(RigidBody2D, 77)
	DO_CLASS_ID(ColliderBox2D, 78)
	DO_CLASS_ID(PhysicsWorld2D, 79)
	DO_CLASS_ID(SceneNode, 80)
	DO_CLASS_ID(Behavior, 81)
	DO_CLASS_ID(Constraint2D, 82)

	DO_CLASS_ID(Collider, 83)
	DO_CLASS_ID(RigidBody, 84)
	DO_CLASS_ID(ColliderBox, 85)
	DO_CLASS_ID(PhysicsWorld, 86)
	DO_CLASS_ID(Constraint, 87)
	DO_CLASS_ID(ColliderSphere, 88)
	DO_CLASS_ID(FileSystem, 89)
	DO_CLASS_ID(Resource, 90)

	DO_CLASS_ID(RenderComponent2D, 92)
	DO_CLASS_ID(StaticSprite2D, 93)
	DO_CLASS_ID(Sprite, 94)
	DO_CLASS_ID(File, 95)
	DO_CLASS_ID(Image, 96)
	DO_CLASS_ID(Mesh, 97)
	DO_CLASS_ID(Shader, 98)
	DO_CLASS_ID(ShaderVariation, 100)
	DO_CLASS_ID(AudioBuffer, 101)
	DO_CLASS_ID(Text, 102)
	DO_CLASS_ID(UIComponent, 103)
	DO_CLASS_ID(Font, 104)
	DO_CLASS_ID(LevelBehaviorObject, 105)
	DO_CLASS_ID(Application, 106)
	

	CLASS_ID_Count
};

}