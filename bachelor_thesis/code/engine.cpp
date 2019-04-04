#include "engine.h"

#include <glad.c>
#include <GLFW/glfw3.h>

GLFWwindow* createWindow()
{
	if (glfwInit())
	{
		GLFWwindow* window = 
			glfwCreateWindow(1280, 720, "thesis", 0, 0);
		
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

void run()
{
	GLFWwindow* window = createWindow();
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}