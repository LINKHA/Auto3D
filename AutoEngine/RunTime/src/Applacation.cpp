#include "Applacation.h"

AUTO_BEGIN
Applacation::Applacation()
{
	window = new Window();
	glfwWindow = window->createWindow();
}
Applacation::~Applacation()
{

}
/*
* @Applacation::run
* this is Engine important funcation
*/
int Applacation::run()
{
	if (init() == AU_ERROR)
	{
		return AU_ERROR;
	}
	if (runLoop() == AU_ERROR)
	{
		return AU_ERROR;
	}
	return finish();
}

int Applacation::init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		//std::cout << "Failed to initialize GLAD" << std::endl;
		return AU_ERROR;
	}
	return AU_NORMAL;
}

int Applacation::runLoop()
{
	while (!glfwWindowShouldClose(glfwWindow))
	{
		window->drawWindow();
	}
	return AU_NORMAL;
}
int Applacation::finish()
{
	window->destoryWindow();
	return AU_NORMAL;
}
AUTO_END
