#include "MotionSpace.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "Camera.h"

AUTO_BEGIN

SINGLETON_INSTANCE(MotionSpace);

GLFWwindow* glfwWindow;

Mesh* mesh;
GameObject* meshObj;
Texture2D* tex;
GameObject* obj;
Camera* cam;
GameObject* camObj;

float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;



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
		INSTANCE(RenderManager).CameraArray.find(0)->second->ProcessKeyboard(FORWARD, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_S) == BUTTON_PRESS)
		INSTANCE(RenderManager).CameraArray.find(0)->second->ProcessKeyboard(BACKWARD, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_A) == BUTTON_PRESS)
		INSTANCE(RenderManager).CameraArray.find(0)->second->ProcessKeyboard(LEFT, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_D) == BUTTON_PRESS)
		INSTANCE(RenderManager).CameraArray.find(0)->second->ProcessKeyboard(RIGHT, TimeManager::Instance().GetDeltaTime() * 2);
}


MotionSpace::MotionSpace()
{
}


MotionSpace::~MotionSpace()
{
}


void MotionSpace::SetWindow(GLFWwindow* s_glfwWindow)
{
	glfwWindow = s_glfwWindow;
}

void MotionSpace::Awake()
{
	
}


void MotionSpace::Start()
{
	cam = new Camera(Vector3(0.0f, 0.0f, 3.0f));
	//cam = new Camera(Vector3(0.0f, 0.0f, 3.0f).ToGLM());
	camObj = new GameObject();
	camObj->GetTransformPtr()->SetPosition(Vector3(0.0f, 0.0f, 3.0f));
	camObj->AddComponent(*cam);
	cam->Start();

	glfwSetCursorPosCallback(glfwWindow, mouseCallBack);
	glfwSetScrollCallback(glfwWindow, scrollCallBack);
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	mesh = new Mesh("Resource/object/base/Cube.FBX");
	mesh->SetColor(Color(0.5f, 0.8f, 0.3f));

	meshObj = new GameObject();
	meshObj->AddComponent(*mesh);
	mesh->Start();
	//////////////////////////////////////////////////////////////////////////
	////Mesh mesh;
	//TextMesh meshText;
	//meshText.SetColor(Color(0.8f, 0.8f, 0.3f));

	//GameObject meshtextObj;
	//meshtextObj.AddComponent(meshText);
	//meshText.Draw();
	//////////////////////////////////////////////////////////////////////////
	tex = new Texture2D();
	obj = new GameObject();
	tex->SetColor(Color(0.5f, 0.5f, 0.5f));

	obj->AddComponent(*tex);
	tex->Start();
	//////////////////////////////////////////////////////////////////////////
}
void MotionSpace::Update()
{
	processInput(glfwWindow);


	float scaleAmount = (float)sin(GrGetTime());
	//////////////////////////////////////////////////////////////////////////
	obj->GetTransformPtr()->SetPosition(Vector3(1.5f, 1.5f, 0.0f));
	obj->GetTransformPtr()->SetRotation(Vector3(0.0f, 0.0f, 90.0f));
	//	obj.GetTransformPtr()->setRotation(-55.0f, Vector3::xAxis);
	obj->GetTransformPtr()->SetRotation(90.0f, Vector3::zAxis);
	obj->GetTransformPtr()->SetScale(Vector3(scaleAmount));
	//Update Transform
	obj->GetTransformPtr()->UpdateTransform();

	meshObj->GetTransformPtr()->SetPosition(Vector3(0.0f, 0.0f, -1.0f));
	meshObj->GetTransformPtr()->UpdateTransform();


	//meshtextObj.GetTransformPtr()->SetPosition(Vector3(-1.5f, -1.5f, 0.0f));
	//meshtextObj.GetTransformPtr()->UpdateTransform();
	//////////////////////////////////////////////////////////////////////////
	cam->Update();
	tex->Update();
	//meshText.PushToRunloop();
	mesh->Update();
}
void MotionSpace::FixUpdate()
{

}
void MotionSpace::Finish()
{
	obj->GetTransformPtr()->Identity();
	meshObj->GetTransformPtr()->Identity();
	//meshtextObj.GetTransformPtr()->Identity();
}

AUTO_END