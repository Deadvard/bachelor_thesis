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
			
			float speed = 10.0f * (float)timestep;
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
			update(&renderData, &voxelData);
		}
		
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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
				//glEnable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

	camera->yaw += (float)camera->cursor.x * sensitivity;
	camera->pitch += (float)camera->cursor.y * sensitivity;
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

glm::vec3 interpolation(const glm::vec3& p1, const glm::vec3& p2, char v1, char v2)
{	
	if (v1 == 0) return p1;	
	if (v2 == 0) return p2;	
	if (v1 - v2 == 0) return p1;

	float f1 = (float)v1 / 1000.0f;
	float f2 = (float)v2 / 1000.0f;
		
	float mu = -f1 / (f2 - f1);
	glm::vec3 p;
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);
	p.z = p1.z + mu * (p2.z - p1.z);
	return p;
}

void createPoints(const VoxelData* voxelData, RenderData* renderData)
{
	struct PosCol
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	glm::ivec3 size = voxelData->distancesSize;
	const char* distances = voxelData->isosurface.distances;

	std::vector<PosCol> points;
	for (int i = 0; i < size.x * size.y * (size.z - 1); ++i)
	{
		int x = i % size.x;
		int y = (i / size.x) % size.y;
		int z = i / (size.x * size.y);

		float fx = (float)x * voxelData->offset;
		float fy = (float)y * voxelData->offset;
		float fz = (float)z * voxelData->offset;

		PosCol temp[8];
		temp[0].position = glm::vec3(fx, fy, fz);
		temp[1].position = glm::vec3(fx + voxelData->offset, fy, fz);
		temp[2].position = glm::vec3(fx + voxelData->offset, fy, fz + voxelData->offset);
		temp[3].position = glm::vec3(fx, fy, fz + voxelData->offset);

		temp[4].position = glm::vec3(fx, fy + voxelData->offset, fz);
		temp[5].position = glm::vec3(fx + voxelData->offset, fy + voxelData->offset, fz);
		temp[6].position = glm::vec3(fx + voxelData->offset, fy + voxelData->offset, fz + voxelData->offset);
		temp[7].position = glm::vec3(fx, fy + voxelData->offset, fz + voxelData->offset);

		int indices[4];
		indices[0] = 0 + i;
		indices[1] = 1 + i;
		indices[2] = 1 + i + size.x * size.y;
		indices[3] = 0 + i + size.x * size.y;

		temp[0].color = (float)distances[indices[0]] >= 0.f ? glm::vec3(1,1,1) : glm::vec3(0,0,0);
		temp[1].color = (float)distances[indices[1]] >= 0.f ? glm::vec3(1,1,1) : glm::vec3(0,0,0);
		temp[2].color = (float)distances[indices[2]] >= 0.f ? glm::vec3(1,1,1) : glm::vec3(0,0,0);
		temp[3].color = (float)distances[indices[3]] >= 0.f ? glm::vec3(1,1,1) : glm::vec3(0,0,0);

		for(int j = 0; j < 8; ++j)
			points.emplace_back(temp[j]);
	}

	renderData->marchingCubes.numPoints = points.size();

	glGenVertexArrays(1, &renderData->marchingCubes.ptVao);
	glBindVertexArray(renderData->marchingCubes.ptVao);
	glGenBuffers(1, &renderData->marchingCubes.ptVbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderData->marchingCubes.ptVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PosCol) * points.size(), &points[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

}

void voxelsToMeshes(const VoxelData* voxelData, RenderData* renderData)
{
	struct Triangle
	{
		glm::vec3 p1,p2,p3;
	};

	std::vector<Triangle> triangles;
	glm::ivec3 size = voxelData->distancesSize;
	const char* distances = voxelData->isosurface.distances;
	
	for (int i = 0; i < size.x * size.y * size.z; ++i)
	{
		int x = i % size.x;
		int y = (i / size.x) % size.y;
		int z = i / (size.x * size.y);

		if (x < size.x - 1 && y < size.y - 1 && z < size.z - 1)
		{
			float fx = (float)x * voxelData->offset;
			float fy = (float)y * voxelData->offset;
			float fz = (float)z * voxelData->offset;

			int indices[8];
			indices[0] = 0 + i;
			indices[1] = 1 + i;
			indices[2] = 1 + i + size.x * size.y;
			indices[3] = 0 + i + size.x * size.y;
			indices[4] = 0 + i + size.x;
			indices[5] = 1 + i + size.x;
			indices[6] = 1 + i + size.x + size.x * size.y;
			indices[7] = 0 + i + size.x + size.x * size.y;
						
			unsigned int marchingCubesCase =
				(distances[indices[7]] < 0) << 7 |
				(distances[indices[6]] < 0) << 6 |
				(distances[indices[5]] < 0) << 5 |
				(distances[indices[4]] < 0) << 4 |
				(distances[indices[3]] < 0) << 3 |
				(distances[indices[2]] < 0) << 2 |
				(distances[indices[1]] < 0) << 1 |
				(distances[indices[0]] < 0) << 0;

			if (marchingCubesCase != 0 && marchingCubesCase != 255)
			{
				glm::vec3 positions[8];

				positions[0] = glm::vec3(fx, fy, fz);
				positions[1] = glm::vec3(fx + voxelData->offset, fy, fz);
				positions[2] = glm::vec3(fx + voxelData->offset, fy, fz + voxelData->offset);
				positions[3] = glm::vec3(fx, fy, fz + voxelData->offset);
										 
				positions[4] = glm::vec3(fx, fy + voxelData->offset, fz);
				positions[5] = glm::vec3(fx + voxelData->offset, fy + voxelData->offset, fz);
				positions[6] = glm::vec3(fx + voxelData->offset, fy + voxelData->offset, fz + voxelData->offset);
				positions[7] = glm::vec3(fx, fy + voxelData->offset, fz + voxelData->offset);

				glm::vec3 vertices[12];

				if (edgeTable[marchingCubesCase] & 1)
					vertices[0] = interpolation(positions[0], positions[1], distances[indices[0]], distances[indices[1]]);
				if (edgeTable[marchingCubesCase] & 2)
					vertices[1] = interpolation(positions[1], positions[2], distances[indices[1]], distances[indices[2]]);
				if (edgeTable[marchingCubesCase] & 4)
					vertices[2] = interpolation(positions[2], positions[3], distances[indices[2]], distances[indices[3]]);
				if (edgeTable[marchingCubesCase] & 8)
					vertices[3] = interpolation(positions[3], positions[0], distances[indices[3]], distances[indices[0]]);
				if (edgeTable[marchingCubesCase] & 16)
					vertices[4] = interpolation(positions[4], positions[5], distances[indices[4]], distances[indices[5]]);
				if (edgeTable[marchingCubesCase] & 32)
					vertices[5] = interpolation(positions[5], positions[6], distances[indices[5]], distances[indices[6]]);
				if (edgeTable[marchingCubesCase] & 64)
					vertices[6] = interpolation(positions[6], positions[7], distances[indices[6]], distances[indices[7]]);
				if (edgeTable[marchingCubesCase] & 128)
					vertices[7] = interpolation(positions[7], positions[4], distances[indices[7]], distances[indices[4]]);
				if (edgeTable[marchingCubesCase] & 256)
					vertices[8] = interpolation(positions[0], positions[4], distances[indices[0]], distances[indices[4]]);
				if (edgeTable[marchingCubesCase] & 512)
					vertices[9] = interpolation(positions[1], positions[5], distances[indices[1]], distances[indices[5]]);
				if (edgeTable[marchingCubesCase] & 1024)
					vertices[10] = interpolation(positions[2], positions[6], distances[indices[2]], distances[indices[6]]);
				if (edgeTable[marchingCubesCase] & 2048)
					vertices[11] = interpolation(positions[3], positions[7], distances[indices[3]], distances[indices[7]]);

				for (int j = 0; triTable[marchingCubesCase][j] != -1; j += 3)
				{
					Triangle triangle;
					triangle.p1 = vertices[triTable[marchingCubesCase][j]];
					triangle.p2 = vertices[triTable[marchingCubesCase][j + 1]];
					triangle.p3 = vertices[triTable[marchingCubesCase][j + 2]];
					triangles.emplace_back(triangle);
				}
			}
		}
	}
	
	if (triangles.size() > 0)
	{
		glGenVertexArrays(1, &renderData->marchingCubes.vao);
		glBindVertexArray(renderData->marchingCubes.vao);
		glGenBuffers(1, &renderData->marchingCubes.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, renderData->marchingCubes.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * triangles.size(), &triangles[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		glEnableVertexAttribArray(0);
		renderData->marchingCubes.numTriangles = (int)triangles.size();
	}
}
