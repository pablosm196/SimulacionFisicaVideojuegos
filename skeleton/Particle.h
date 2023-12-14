#pragma once
#include "RenderUtils.hpp"
using namespace physx;

enum Shape { SPHERE, BOX };
class Particle
{
protected:
	Vector3 vel;
	Vector3 force;
	float damping, lifespan, actualTime = 0, Imass, mass;
	PxTransform pose;
	RenderItem* renderItem;
	Vector4 color;
	Shape shape;
public:
	Particle();
	Particle(Vector3 v, Vector3 p, float d, float m, float t = 5.0f, Vector4 col = {255, 0, 0, 1}, Shape s = SPHERE);
	~Particle();

	virtual void integrate(double t);
	virtual bool checkTime();
	inline void clearForce() { force *= 0.0f; }
	virtual inline void addForce(const Vector3& f) { force += f; }
	inline const float getMass() { return mass; }
	inline void setMass(float m) { 
		mass = m;
		if (mass > 0) Imass = 1 / m;
		else Imass = 0;
	}
	inline const Vector3 getPos() { return pose.p; }
	inline const Vector3 getVel() { return vel; }
	inline const Vector4 getColor() { return color; }
	float getHeight();
	float getVolumen();
	void setBoxSize(float w, float h, float l);
	void setSphereRadius(float r);
};

class RigidParticle : public Particle {
private:
	PxRigidDynamic* rigid;
	PxScene* scene = nullptr;
public:
	RigidParticle(Vector3 linear_v, Vector3 p, float m, PxPhysics* physics, float t = 5.0f, Vector4 col = { 1, 0, 0, 1 }, Shape s = SPHERE, Vector3 angular_v = {0, 0, 0});
	~RigidParticle();
	void addForce(const Vector3& f) override;
	inline PxRigidDynamic* getRigidDynamic() { return rigid; }
	inline void setScene(PxScene* s) { scene = s; }
	void integrate(double t) override;
};

