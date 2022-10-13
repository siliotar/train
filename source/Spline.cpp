#include "Spline.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/spline.hpp"

void Spline::_addPointInfo(size_t i)
{
	size_t size = _splinePoints.size();

	glm::vec3 v = _splinePoints[(i + size) % size] - _splinePoints[(i + size - 1) % size];
	float length = glm::length(v);
	glm::vec3 direction = v / length;
	_directions.push_back(direction);
	_normals.push_back(glm::cross(direction, _up));
	_lengths.push_back(length);
	_length += length;
}

Spline::Spline(const float* path, size_t pointsCount, size_t pointsPerSegment)
	: _length(0.0f), _up(0.0f, 1.0f, 0.0f)
{
	const float smoothness = 1.0f / pointsPerSegment;
	const glm::vec3 up(0.0f, 1.0f, 0.0f);

	for (size_t i = 0; i < pointsCount; ++i)
	{
		size_t p1 = (i + pointsCount - 1) % pointsCount;
		size_t p2 = i;
		size_t p3 = (i + pointsCount + 1) % pointsCount;
		size_t p4 = (i + pointsCount + 2) % pointsCount;

		glm::vec3 v1(path[p1 * 3], path[p1 * 3 + 1], path[p1 * 3 + 2]);
		glm::vec3 v2(path[p2 * 3], path[p2 * 3 + 1], path[p2 * 3 + 2]);
		glm::vec3 v3(path[p3 * 3], path[p3 * 3 + 1], path[p3 * 3 + 2]);
		glm::vec3 v4(path[p4 * 3], path[p4 * 3 + 1], path[p4 * 3 + 2]);

		for (size_t j = 0; j < pointsPerSegment; ++j)
		{
			_splinePoints.push_back(glm::catmullRom(v1, v2, v3, v4, smoothness * j));
			size_t size = _splinePoints.size();
			if (size > 1)
				_addPointInfo(size - 1);
		}
	}

	if (_splinePoints.size() > 0)
		_addPointInfo(0);
}

Spline::~Spline()
{}

glm::vec3& Spline::operator[](size_t i)
{
	return _splinePoints[i];
}

const glm::vec3& Spline::operator[](size_t i) const
{
	return _splinePoints[i];
}

SplinePoint Spline::getSplinePointByDistance(float distance) const
{
	while (distance > _length)
		distance -= _length;

	float currentDistance = 0.0f;
	size_t size = _splinePoints.size();
	SplinePoint result;
	for (size_t i = 0; i < size; ++i)
	{
		if (distance < currentDistance + _lengths[i])
		{
			const glm::vec3& direction = _directions[i];

			float delta = distance - currentDistance;
			result.position = _splinePoints[i] + direction * delta;
			result.direction = direction;
			result.normal = _normals[i];
			result.length = _lengths[i];
			break;
		}
		currentDistance += _lengths[i];
	}
	return result;
}
