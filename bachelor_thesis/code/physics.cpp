#include <glm/glm.hpp>

#define FLT_EPSILON 1.192092896e-07F

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

float clamp(float n, float min, float max)
{
	if (n < min) return min;
	if (n > max) return max;
	return n;
}

float closestPtSegmentSegment(
	glm::vec3 p1, glm::vec3 q1, glm::vec3 p2, glm::vec3 q2, 
	float* s, float * t, glm::vec3* c1, glm::vec3* c2)
{
	glm::vec3 d1 = q1 - p1;
	glm::vec3 d2 = q2 - p2;
	glm::vec3 r = p1 - p2;
	
	float a = glm::dot(d1, d1);
	float e = glm::dot(d2, d2);
	float f = glm::dot(d2, r);
	
	if (a <= FLT_EPSILON && e <= FLT_EPSILON)
	{
		*s = 0.0f;
		*t = 0.0f;

		*c1 = p1;
		*c2 = p2;

		return glm::dot(*c1 - *c2, *c1 - *c2);
	}

	if (a <= FLT_EPSILON)
	{
		*s = 0.0f;
		*t = f / e;
		*t = clamp(*t, 0.0f, 1.0f);
	}
	else
	{
		float c = glm::dot(d1, r);
		if (e <= FLT_EPSILON)
		{
			*t = 0.0f;
			*s = clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{
			float b = glm::dot(d1, d2);
			float denom = a * e - b * b;

			if (denom != 0.0f)
			{
				*s = clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else
			{
				*s = 0.0f;
			}

			*t = (b * (*s) + f) / e;

			if (*t < 0.0f)
			{
				*t = 0.0f;
				*s = clamp(-c / a, 0.0f, 1.0f);
			}
			else if (*t > 1.0f)
			{
				*t = 1.0f;
				*s = clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	*c1 = p1 + d1 * (*s);
	*c2 = p2 + d2 * (*t);
	return glm::dot(*c1 - *c1, *c1 - *c1);
}

int testSphereCapsule()
{
	float dist2 = sqDistPointSegment(glm::vec3(), glm::vec3(), glm::vec3());
	float radius = 0;
	return dist2 <= radius * radius;
}

int testCapsuleCapsule()
{
	float s;
	float t;
	glm::vec3 c1;
	glm::vec3 c2;
	
	float dist2 = closestPtSegmentSegment(c1, c1, c1, c1, &s, &t, &c1, &c2);
	float radius = 0;
	return dist2 <= radius * radius;
}