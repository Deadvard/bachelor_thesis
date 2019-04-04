#include "engine.h"

#include <glad.c>
#include <GLFW/glfw3.h>

#include "graphics.h"

GLFWwindow* createWindow(int width, int height);

void run()
{
	GLFWwindow* window = createWindow(1280, 720);

	RenderData renderData;

	double timestep = 1.0 / 120.0;
	double lastTime = glfwGetTime();
	double deltaTime = 0.0;
	
	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		deltaTime += now - lastTime;
		lastTime = now;

		while (deltaTime > timestep)
		{
			deltaTime -= timestep;
		}
		
		glClear(GL_COLOR_BUFFER_BIT);
		render(&renderData);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}

GLFWwindow* createWindow(int width, int height)
{
	if (glfwInit())
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window =
			glfwCreateWindow(width, height, "bachelor_thesis", 0, 0);

		if (window)
		{
			glfwMakeContextCurrent(window);

			if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				return window;
			}
		}
	}

	return 0;
}