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
	Vector4 color;
public:
	Particle();
	Particle(Vector3 v, Vector3 p, Vector3 a, float d, float t = 5.0f, Vector4 col = {255, 0, 0, 1});
	~Particle();

	virtual void integrate(double t);
	virtual bool checkTime();
	inline Vector3 getPos() { return pose.p; }
	inline Vector3 getVel() { return vel; }
};

