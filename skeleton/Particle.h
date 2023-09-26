#pragma once
#include "RenderUtils.hpp"
using namespace physx;

class Particle
{
private:
	Vector3 vel;
	Vector3 acceleration;
	PxTransform pose;
	RenderItem* renderItem;
public:
	Particle();
	Particle(Vector3 v, Vector3 p, Vector3 a);
	~Particle();

	void integrate(double t);
};

