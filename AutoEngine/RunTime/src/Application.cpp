#include "Application.h"
AUTO_BEGIN

Application * Application::m_pApplication = nullptr;


Application::~Application()
{
	Assert(this == m_pApplication);
	m_pApplication = nullptr;
}
/*
* @Application::run
* this is Engine important funcation
*/
int Application::run()
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

int Application::init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ErrorString("Failed to initialize GLAD \n");
		return AU_ERROR;
	}
	return AU_NORMAL;
}

void processInput(GLFWwindow *window)
{
	if (GLGetKey(window, KEY_ESCAPE) == S_PRESS)
		GLCloseWindow(window);
}

int Application::runLoop()
{


	Shader ourShader((AtConfig::shader_path + "AUTO_texture.vs").c_str(), (AtConfig::shader_path + "AUTO_texture.fs").c_str());
	Shader ourShader( AtConfig::shader_path + "AUTO_texture.vs", AtConfig::shader_path + "AUTO_texture.fs" );


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// 加载并创建纹理
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);			//需要输入生成纹理的数量,然后把它们存储在第二个参数的unsigned int中
	glBindTexture(GL_TEXTURE_2D, texture);//绑定纹理
										  // 为当前绑定的纹理对象设置环绕方式
										  /*
										  最后一个参数
										  GL_REPEAT			超出部分原样
										  GL_MIRRORED_REPEAT	超出部分镜像
										  GL_CLAMP_TO_EDGE	超出部分拉伸
										  GL_CLAMP_TO_BORDER	超出部分使用指定颜色
										  */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 为当前绑定的纹理对象设置过滤方式
	/*
	GL_NEARSET 选择最近的颜色(边缘锯齿)
	GL_LINEAR  基于纹理坐标附件的纹理像素计算差值 (边缘模糊)
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	/*
	1.接受一个图像文件的位置作为输入
	2.需要三个int作为他的2,3,4 宽度 高度 颜色通道个数
	*/
	unsigned char *data = stbi_load("Resource/texture/square.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{	/*
		生成纹理
		第一个参数 指定了纹理目标 设置GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理
		第二个参数 为纹理指定多级渐远纹理的级别
		第三个参数 告诉OpenGL我们希望吧纹理存储为何种格式
		第四个第五个参数 设置最终的纹理的宽度和高度
		第六个参数 总设为0历史遗留问题
		第七第八个参数定义了源图的格式和数据类型
		最后一个参数是真正的图像数据
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	while (!GLShouldCloseWindow(glfwWindow))
	{
		processInput(glfwWindow);

		window.drawWindow();

		// bind Texture
		//默认分配给第一个uniform sampler2D 如要自己分配glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// render container
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/*glfwSwapBuffers(glfwWindow);
		glfwPollEvents();*/
		window.runLoopOver();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return AU_NORMAL;
}
int Application::finish()
{
	window.destoryWindow();
	glfwWindow = nullptr;
	return AU_NORMAL;
}

///Static
Application* Application::getInstance()
{
	if (m_pApplication == nullptr)
	{
		m_pApplication = new Application();
	}
	return m_pApplication;
}

///Private
Application::Application()
{
	window = Window();
	glfwWindow = window.createWindow();
	Assert(!m_pApplication);
	m_pApplication = this;
}
AUTO_END
