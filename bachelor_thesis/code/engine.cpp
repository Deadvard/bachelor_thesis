#include "engine.h"

#include <glad.c>
#include <GLFW/glfw3.h>

GLFWwindow* createWindow(int width, int height);

void run()
{
	GLFWwindow* window = createWindow(1280, 720);

	double timestep = 1.0 / 120.0;
	double last_time = glfwGetTime();
	double delta_time = 0.0;
	
	while (!glfwWindowShouldClose(window))
	{
		double now = glfwGetTime();
		delta_time += now - last_time;
		last_time = now;

		while (delta_time > timestep)
		{
			delta_time -= timestep;
		}
		
		glClear(GL_COLOR_BUFFER_BIT);

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