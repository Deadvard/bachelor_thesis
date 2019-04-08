#include "engine.h"

#include <glad.c>
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

#include "graphics.h"
#include "voxel_grid.h"

#define pi32 3.14159265359f

struct Camera
{
	glm::vec3 position;

	float yaw;
	float pitch;

	glm::dvec2 cursor;
};

GLFWwindow* createWindow(int width, int height);
glm::mat4 cameraView(Camera* camera);
void voxelsToMeshes(const VoxelData* voxelData, RenderData* renderData);
void createPoints(const VoxelData* voxelData, RenderData* renderData);

void run()
{
	GLFWwindow* window = createWindow(1280, 720);

	Camera camera = {};
	camera.position = glm::vec3(0.0f, 0.0f, 1.0f);
	
	RenderData renderData = {};
	initalize(&renderData);

	VoxelData voxelData = {};
	initialize(&voxelData);

	voxelsToMeshes(&voxelData, &renderData);
	createPoints(&voxelData, &renderData);

	double timestep = 1.0 / 120.0;
	double lastTime = glfwGetTime();
	double deltaTime = 0.0;

	glm::dvec2 oldMouse = glm::dvec2(0.0, 0.0);
	glm::dvec2 newMouse = glm::dvec2(0.0, 0.0);
	
	bool running = true;
	
	while (running)
	{
		double now = glfwGetTime();
		deltaTime += now - lastTime;
		lastTime = now;

		while (deltaTime > timestep)
		{				
			glm::mat4 mat = renderData.view;
			glm::vec3 forward = glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
			glm::vec3 right = glm::vec3(mat[0][0], mat[1][0], mat[2][0]);	
			
			float speed = 10.0f * timestep;
			if (glfwGetKey(window, GLFW_KEY_W)) camera.position -= speed * forward;
			if (glfwGetKey(window, GLFW_KEY_A)) camera.position -= speed * right;
			if (glfwGetKey(window, GLFW_KEY_S)) camera.position += speed * forward;
			if (glfwGetKey(window, GLFW_KEY_D)) camera.position += speed * right;
			
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwGetCursorPos(window, &newMouse.x, &newMouse.y);
				camera.cursor.x = newMouse.x - oldMouse.x;
				camera.cursor.y = newMouse.y - oldMouse.y;
				oldMouse = newMouse;	
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				glfwGetCursorPos(window, &oldMouse.x, &oldMouse.y);
				glfwGetCursorPos(window, &newMouse.x, &newMouse.y);
				camera.cursor.x = 0.0;
				camera.cursor.y = 0.0;
			}

			renderData.view = cameraView(&camera);				
			deltaTime -= timestep;
			update(&renderData);
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render(&renderData);
		glfwSwapBuffers(window);

		glfwPollEvents();

		running = !glfwWindowShouldClose(window)
			&& !glfwGetKey(window, GLFW_KEY_ESCAPE);
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
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				glEnable(GL_CULL_FACE);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				return window;
			}

		}
	}

	return 0;
}

glm::mat4 cameraView(Camera* camera)
{
	float sensitivity = 0.01f;
	float max = pi32 * 0.4f;
	float min = -max;

	camera->yaw += camera->cursor.x * sensitivity;
	camera->pitch += camera->cursor.y * sensitivity;
	if (camera->pitch < min) camera->pitch = min;
	if (camera->pitch > max) camera->pitch = max;

	glm::quat qPitch = glm::angleAxis(camera->pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(camera->yaw, glm::vec3(0, 1, 0));
	
	glm::quat qRotate = glm::normalize(qPitch * qYaw);
	glm::mat4 rotate = glm::mat4_cast(qRotate);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -camera->position);

	return rotate * translate;
}

float cube[] =
{
	//pos, uv, normal
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
};

void voxelsToMeshes(const VoxelData* voxelData, RenderData* renderData)
{
	renderData->meshes.positions.resize(voxelData->grid.size());

	for(int i = 0; i < renderData->meshes.positions.size(); ++i)
		renderData->meshes.positions[i] = voxelData->grid[i].position;

	glGenVertexArrays(1, &renderData->meshes.vao);
	glBindVertexArray(renderData->meshes.vao);

	glGenBuffers(1, &renderData->meshes.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderData->meshes.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 5));

	//Instancing buffer
	glGenBuffers(1, &renderData->meshes.instanceVbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderData->meshes.instanceVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * renderData->meshes.positions.size(), &renderData->meshes.positions[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
	glVertexAttribDivisor(3, 1);

}

void createPoints(const VoxelData* voxelData, RenderData* renderData)
{
	glGenVertexArrays(1, &renderData->points.vao);
	glBindVertexArray(renderData->points.vao);

	glm::vec3 tempPositions[125];

	for (int i = 0; i < 125; ++i)
	{
		int x = i % 5;
		int y = (i / 5) % 5;
		int z = i / (5 * 5);
		
		tempPositions[i] = glm::vec3(x, y, z);
	}

	glGenBuffers(1, &renderData->points.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderData->points.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tempPositions), &tempPositions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
}