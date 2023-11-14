#pragma once
#include "RenderUtils.hpp"
using namespace physx;

class Particle
{
protected:
	Vector3 vel;
	Vector3 force;
	float damping, lifespan, actualTime = 0, Imass;
	PxTransform pose;
	RenderItem* renderItem;
	Vector4 color;
public:
	Particle();
	Particle(Vector3 v, Vector3 p, float d, float mass, float t = 5.0f, Vector4 col = {255, 0, 0, 1});
	~Particle();

	virtual void integrate(double t);
	virtual bool checkTime();
	inline void clearForce() { force *= 0.0f; }
	inline void addForce(const Vector3& f) { force += f; }
	inline const float getMass() { return Imass; }
	inline const Vector3 getPos() { return pose.p; }
	inline const Vector3 getVel() { return vel; }
	inline const Vector4 getColor() { return color; }
};

