#pragma once
#include "RenderUtils.hpp"
using namespace physx;

class GaussianRigidGenerator;

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
	virtual ~Particle();

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
	inline virtual const Vector3 getPos() { return pose.p; }
	inline virtual const Vector3 getVel() { return vel; }
	inline const Vector4 getColor() { return color; }
	float getHeight();
	float getVolumen();
	virtual void setBoxSize(float w, float h, float l);
	void setSphereRadius(float r);
};

class RigidParticle : public Particle {
protected:
	PxRigidDynamic* rigid = nullptr;
	PxScene* scene = nullptr;
	PxShape* Pshape;
	GaussianRigidGenerator* rg = nullptr;
public:
	RigidParticle(Vector3 linear_v, Vector3 p, float m, PxPhysics* physics, PxScene* px_scene, float t = 5.0f, Vector4 col = { 1, 0, 0, 1 }, Shape s = SPHERE, Vector3 angular_v = {0, 0, 0});
	~RigidParticle() override;
	void addForce(const Vector3& f) override;
	void integrate(double t) override;
	void setBoxSize(float w, float h, float l) override;
	inline void setGenerator(GaussianRigidGenerator* creator) { rg = creator; }
	PxRigidDynamic* getRigid() { return rigid; }
	inline const Vector3 getPos() override { return rigid->getGlobalPose().p; }
	inline const Vector3 getVel() override { return rigid->getLinearVelocity(); }
};

class Bolo : public RigidParticle {
private:
	bool tirado;
public:
	Bolo(Vector3 linear_v, Vector3 p, float m, PxPhysics* physics, PxScene* px_scene, float t = 5.0f, Vector4 col = { 1, 0, 0, 1 }, Shape s = SPHERE, Vector3 angular_v = { 0, 0, 0 }) : RigidParticle(linear_v, p, m, physics, px_scene, t, col, s, angular_v) { tirado = false; };
	~Bolo() override = default;
	void setTirado() { tirado = true; }
	bool getTirado() { return tirado; }
};

