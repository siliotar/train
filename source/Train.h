#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "framework/object.h"
#include "Spline.h"

struct Wagon
{
	float curPosition = 0.0f;
	Object* object = nullptr;
};

class Train
{
private:
	const Spline& _spline;
	float _splineLength;
	std::vector<Wagon> _wagons;
	Mesh _wagonMesh;
	const glm::vec3 _up;
	const glm::vec3 _yOffset;
public:
	Train(float width, size_t size, const Spline& spline);
	~Train();

	void move(float distance);
	void moveWagon(Wagon& wagon, float distance);
};
