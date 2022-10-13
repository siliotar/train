#include "Train.h"
#include "framework/engine.h"

Train::Train(float width, size_t size, const Spline& spline)
	: _spline(spline), _splineLength(spline.getLength()), _wagonMesh(createCube()), _up(0.0f, 1.0f, 0.0f), _yOffset(0.0f, width / 2, 0.0f)
{
	if (size > _splineLength / (width * 1.1f))
		size = _splineLength / (width * 1.1f);

	Engine* engine = Engine::get();

	for (size_t i = 0; i < size; ++i)
	{
		Wagon wagon;

		wagon.object = engine->createObject(&_wagonMesh);
		wagon.object->setColor(0.4f, 0.4f, 0.4f);
		moveWagon(wagon, (size - i - 1) * 1.1f);

		_wagons.push_back(wagon);
	}
}

Train::~Train()
{}

void Train::move(float distance)
{
	size_t size = _wagons.size();
	for (size_t i = 0; i < size; ++i)
		moveWagon(_wagons[i], distance);
}

void Train::moveWagon(Wagon& wagon, float distance)
{
	float endDistance = wagon.curPosition + distance;

	SplinePoint point = _spline.getSplinePointByDistance(endDistance);
	wagon.object->setPosition(point.position + _yOffset);

	glm::mat3 Result;

	Result[2] = -point.direction;
	Result[0] = point.normal;
	Result[1] = _up;

	glm::quat rotation = quat_cast(Result);

	wagon.object->setRotation(rotation);
	wagon.curPosition = endDistance;
}
