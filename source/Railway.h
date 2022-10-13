#pragma once
#include <vector>
#include "framework/mesh.h"
#include "Spline.h"

void createRailwayVertecies(const Spline& spline, float railwayWidth, \
	std::vector<glm::vec3>& innerRails, std::vector<glm::vec3>& outerRails, \
	std::vector<glm::vec3>& sleepers, float distanceBtwSleepers);

Mesh createRailMesh(const std::vector<glm::vec3>& rail);

Mesh createSleepersMesh(const std::vector<glm::vec3>& sleepers);
