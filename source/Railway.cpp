#include "Railway.h"
#include "framework/engine.h"

void createRailwayVertecies(const Spline& spline, float railwayWidth, \
	std::vector<glm::vec3>& innerRails, std::vector<glm::vec3>& outerRails, \
	std::vector<glm::vec3>& sleepers, float distanceBtwSleepers)
{
	float railwayHalfWidth = railwayWidth / 2;
	size_t size = spline.size();
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	float distFromCenterToOuter = railwayWidth / 3.0f;
	float railWidth = railwayWidth / 8.0f;
	float distFromCenterToInner = distFromCenterToOuter - railWidth;

	float sleepersWidth = railWidth;
	float sleepersHalfWidth = sleepersWidth / 2.0f;
	int sleepersCount = static_cast<int>(spline.getLength() / distanceBtwSleepers);
	distanceBtwSleepers = spline.getLength() / sleepersCount;
	float currentDistance = 0.0f;
	float nextSleeperPosition = 0.0f;

	for (size_t i = 0; i < size; ++i)
	{
		const glm::vec3& direction = spline.getDirection(i);
		const glm::vec3& normal = spline.getNormal(i);
		float segmentLength = spline.getLength(i);

		float nextDistance = currentDistance + segmentLength;
		while (nextSleeperPosition < nextDistance)
		{
			float distanceToNextSleeper = nextSleeperPosition - currentDistance;
			glm::vec3 sleeperCenter = spline[i] + direction * distanceToNextSleeper;
			sleepers.push_back(sleeperCenter - normal * railwayHalfWidth - direction * sleepersHalfWidth);
			sleepers.push_back(sleeperCenter + normal * railwayHalfWidth - direction * sleepersHalfWidth);
			sleepers.push_back(sleeperCenter + normal * railwayHalfWidth + direction * sleepersHalfWidth);
			sleepers.push_back(sleeperCenter - normal * railwayHalfWidth + direction * sleepersHalfWidth);

			nextSleeperPosition += distanceBtwSleepers;
		}
		currentDistance = nextDistance;

		innerRails.push_back(spline[i] + normal * distFromCenterToInner);
		innerRails.push_back(spline[i] + normal * distFromCenterToOuter);
		outerRails.push_back(spline[i] - normal * distFromCenterToOuter);
		outerRails.push_back(spline[i] - normal * distFromCenterToInner);
	}
}

Mesh createRailMesh(const std::vector<glm::vec3>& rail)
{
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;

	size_t size = rail.size();
	glm::vec3 offset(0.0f, 0.01f, 0.0f);
	for (size_t i = 0; i < size; i += 2)
	{
		vertices.push_back({ rail[i] + offset, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
		vertices.push_back({ rail[i + 1] + offset, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });

		indices.push_back(i);
		indices.push_back((i + 1) % size);
		indices.push_back((i + 2) % size);
		indices.push_back((i + 2) % size);
		indices.push_back((i + 1) % size);
		indices.push_back((i + 3) % size);
	}

	return Mesh(vertices, indices);
}

Mesh createSleepersMesh(const std::vector<glm::vec3>& sleepers)
{
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;

	size_t size = sleepers.size();
	for (size_t i = 0; i < size; i += 4)
	{
		vertices.push_back({ sleepers[i], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
		vertices.push_back({ sleepers[i + 1], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
		vertices.push_back({ sleepers[i + 2], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
		vertices.push_back({ sleepers[i + 3], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i + 2);
		indices.push_back(i + 3);
		indices.push_back(i);
	}

	return Mesh(vertices, indices);
}
