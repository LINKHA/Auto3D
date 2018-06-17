#include "MotionSpace.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "BaseLight.h"
AUTO_BEGIN

SINGLETON_INSTANCE(MotionSpace);




Mesh* mesh;
GameObject* meshObj;

Light* light;
GameObject* lightObj;

Texture2D* tex;
GameObject* obj;

Camera* cam;
GameObject* camObj;


bool firstMouse = true;

RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();
float lastX = rect.width;
float lastY = rect.height;

void mouseCallBack(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;


	cam->ProcessMouseMovement(xoffset, yoffset);
}
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	cam->ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window)
{
	if (GrGetKey(window, KEY_ESCAPE) == BUTTON_PRESS)
		GrCloseWindow(window);
	if (GrGetKey(window, KEY_W) == BUTTON_PRESS)
		cam->ProcessKeyboard(FORWARD, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_S) == BUTTON_PRESS)
		cam->ProcessKeyboard(BACKWARD, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_A) == BUTTON_PRESS)
		cam->ProcessKeyboard(LEFT, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_D) == BUTTON_PRESS)
		cam->ProcessKeyboard(RIGHT, TimeManager::Instance().GetDeltaTime() * 2);
}


MotionSpace::MotionSpace()
{
}


MotionSpace::~MotionSpace()
{
}

void MotionSpace::Awake()
{
	
}


void MotionSpace::Start()
{
	

	cam = new Camera(Vector3(0.0f, 0.0f, 3.0f));
	camObj = new GameObject();
	camObj->GetComponent(Transform).SetPosition(Vector3(0.0f, 0.0f, 3.0f));

	camObj->AddComponent(cam);


	glfwSetCursorPosCallback(INSTANCE(GLWindow).GetGLWindow(), mouseCallBack);
	glfwSetScrollCallback(INSTANCE(GLWindow).GetGLWindow(), scrollCallBack);
	glfwSetInputMode(INSTANCE(GLWindow).GetGLWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//////////////////////////////////////////////////////////////////////////

	
	
	lightObj = new GameObject();
	light = new Light(Directional);
	light->ambient = Vector3(0.3f, 0.3f, 0.3f);
	lightObj->AddComponent(light);
	
	//////////////////////////////////////////////////////////////////////////
	mesh = new Mesh("Resource/object/base/Cube.FBX");
	//mesh = new Mesh();
	mesh->GetMaterial().color.Set(0.5f, 0.8f, 0.3f);

	meshObj = new GameObject();
	meshObj->AddComponent(mesh);
	
	//////////////////////////////////////////////////////////////////////////
	tex = new Texture2D("Resource/texture/wood.jpg");
	obj = new GameObject();
	tex->SetColor(Color(0.5f, 0.5f, 0.5f));

	obj->AddComponent(tex);
	//////////////////////////////////////////////////////////////////////////

	INSTANCE(GameObjectManager).ModeRunGameObject(StartMode);
}
void MotionSpace::Update(Camera* camera)
{
	processInput(INSTANCE(GLWindow).GetGLWindow());

	float scaleAmount = (float)sin(GrGetTime());
	//////////////////////////////////////////////////////////////////////////
	obj->GetComponent(Transform).SetPosition(Vector3(1.5f, 1.5f, 0.0f));
	//obj->GetComponent(Transform).SetRotation(Vector3(0.0f, 0.0f, 90.0f));
	////obj->GetComponent(Transform).setRotation(-55.0f, Vector3::xAxis);
	//obj->GetComponent(Transform).SetRotation(90.0f, Vector3::zAxis);
	//obj->GetComponent(Transform).SetScale(Vector3(scaleAmount));
	////Update Transform
	obj->GetComponent(Transform).UpdateTransform();

	meshObj->GetComponent(Transform).SetPosition(Vector3(0.0f, 0.0f, -1.0f));
	meshObj->GetComponent(Transform).UpdateTransform();
	//////////////////////////////////////////////////////////////////////////
	INSTANCE(GameObjectManager).ModeRunGameObject(UpdateMode, camera);
}
void MotionSpace::FixUpdate()
{

}
void MotionSpace::Finish()
{
	INSTANCE(GameObjectManager).ModeRunGameObject(FinishMode);
}

AUTO_END