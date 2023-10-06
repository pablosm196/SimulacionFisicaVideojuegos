#pragma once
#include "RenderUtils.hpp"
using namespace physx;

class Particle
{
protected:
	Vector3 vel;
	Vector3 acceleration;
	float damping, lifespan, actualTime = 0;
	PxTransform pose;
	RenderItem* renderItem;
public:
	Particle();
	Particle(Vector3 v, Vector3 p, Vector3 a, float d, float t);
	~Particle();

	void integrate(double t);
	bool checkTime();
};

