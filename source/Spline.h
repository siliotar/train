#pragma once

#include <vector>
#include "glm/glm.hpp"

struct SplinePoint
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 normal;
	float length;

	SplinePoint() : position(glm::vec3(0)), direction(glm::vec3(0)), normal(glm::vec3(0)), length(0) {}
};

class Spline
{
private:
	std::vector<glm::vec3> _splinePoints;
	std::vector<glm::vec3> _directions;
	std::vector<float> _lengths;
	std::vector<glm::vec3> _normals;
	float _length;

	const glm::vec3 _up;

	void _addPointInfo(size_t i);
public:
	Spline(const float* path, size_t pointsCount, size_t pointsPerSegment);
	~Spline();

	const std::vector<glm::vec3>& getPoints() const { return _splinePoints; }
	const size_t size() const { return _splinePoints.size(); }
	const float getLength() const { return _length; }
	const float getLength(size_t i) const { return _lengths[i]; }
	const glm::vec3& getNormal(size_t i) const { return _normals[i]; }
	const glm::vec3& getDirection(size_t i) const { return _directions[i]; }
	// returns a point on a spline at a given distance from the start
	SplinePoint getSplinePointByDistance(float distance) const;

	glm::vec3& operator[](size_t i);
	const glm::vec3& operator[](size_t i) const;
};
