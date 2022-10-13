#include "framework/engine.h"
#include "framework/utils.h"
#include "Spline.h"
#include "Railway.h"
#include "Train.h"

using namespace std;
using namespace glm;

/*
* Coordinate system:
* x - right
* y - up
* z - backward
*/

int main()
{
	// initialization
	Engine *engine = Engine::get();
	engine->init(1600, 900, "UNIGINE Test Task");

	// set up camera
	Camera &cam = engine->getCamera();
	cam.Position = vec3(0.0f, 12.0f, 17.0f);
	cam.Yaw = -90.0f;
	cam.Pitch = -45.0f;
	cam.UpdateCameraVectors();

	// create shared meshes
	Mesh plane_mesh = createPlane();

	// create background objects
	Object *plane = engine->createObject(&plane_mesh);
	plane->setColor(0.2f, 0.37f, 0.2f); // green
	plane->setPosition(0, -0.5f, 0);
	plane->setRotation(-90.0f, 0.0f, 0.0f);
	plane->setScale(20.0f);

	// path
	const float path[] = {
		 0.0f, -0.375f,  7.0f, // 1
		-6.0f, -0.375f,  5.0f, // 2
		-8.0f, -0.375f,  1.0f, // 3
		-4.0f, -0.375f, -6.0f, // 4
		 0.0f, -0.375f, -7.0f, // 5
		 1.0f, -0.375f, -4.0f, // 6
		 4.0f, -0.375f, -3.0f, // 7
		 8.0f, -0.375f,  7.0f  // 8
	};

	// create Spline by given points
	Spline spline(path, 8, 32);

	// create vertices for inner rail, outer rail and sleepers
	std::vector<glm::vec3> inner;
	std::vector<glm::vec3> outer;
	std::vector<glm::vec3> sleepers_positions;
	createRailwayVertecies(spline, 0.8f, inner, outer, sleepers_positions, 0.4f);

	// create inner rail object
	Mesh inner_rail_mesh = createRailMesh(inner);
	Object* inner_rail = engine->createObject(&inner_rail_mesh);
	inner_rail->setColor(0.2f, 0.2f, 0.2f);
	
	// create outer rail object
	Mesh outer_rail_mesh = createRailMesh(outer);
	Object* outer_rail = engine->createObject(&outer_rail_mesh);
	outer_rail->setColor(0.2f, 0.2f, 0.2f);

	// create sleepers object
	Mesh sleepers_mesh = createSleepersMesh(sleepers_positions);
	Object* sleepers = engine->createObject(&sleepers_mesh);
	sleepers->setColor(0.54f, 0.27f, 0.07f);

	// create train
	Train train(1.0f, 8, spline);

	float trainSpeed = 3.0f;

	// main loop
	while (!engine->isDone())
	{
		engine->update();

		float deltaTime = engine->getDeltaTime();

		// move train every frame
		train.move(deltaTime * trainSpeed);

		engine->render();
		
		engine->swap();
	}

	engine->shutdown();
	return 0;
}
