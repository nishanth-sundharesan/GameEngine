#include "Pch.h"
#include "SList.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	const glm::vec4 cornFlowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);

	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(
		800,								//Width of the window			
		600,								//Height of the window
		"Game Engine",						//Title of the window
		nullptr,
		nullptr);

	if (window == nullptr)
	{
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (gl3wInit() != 0)
	{
		return -1;
	}

	glViewport(0, 0, 800, 600);

	while (!glfwWindowShouldClose(window))
	{
		glClearBufferfv(GL_COLOR, 0, &cornFlowerBlue[0]);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}