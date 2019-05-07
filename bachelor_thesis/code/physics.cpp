#include <glm/glm.hpp>


float sqDistPointSegment(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 bc = c - b;
	
	float e = glm::dot(ac, ab);

	//Handle cases where c projects outside ab
	if (e <= 0.0f) return glm::dot(ac, ac);

	float f = glm::dot(ab, ab);

	if (e >= f) return glm::dot(bc, bc);

	//Handle cases where c projects onto ab
	return glm::dot(ac, ac) - e * e / f;
}

float closestPtSegmentSegment(
	glm::vec3 p1, glm::vec3 q1, glm::vec3 p2, glm::vec3 q2, 
	float* s, float * t, glm::vec3* c2, glm::vec3* c2)
{
	glm::vec3 d1 = q1 - p1;
	glm::vec3 d2 = q2 - p2;
	glm::vec3 r = p1 - p2;
	
	float a = glm::dot(d1, d1);
	float e = glm::dot(d2, d2);
	float f = glm::dot(d2, r);
	
	if (a <= EPSILON && e <= EPSILON)
	{
		
	}
}