#include "Particle.h"

Particle::Particle()
{
	vel = Vector3(0, 0, 0);
	pose = PxTransform(Vector3(0, 0, 0));
	Imass = 1.0f;
	damping = 0.998f;
	lifespan = 1000.0f;
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(10)), Vector4(255, 0, 0, 1));
	force = Vector3(0, 0, 0);
}

Particle::Particle(Vector3 v, Vector3 p, float mass, float d, float t, Vector4 col)
{
	vel = v;
	pose = PxTransform(p);
	damping = d;
	Imass = mass;
	lifespan = t;
	color = col;
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &pose, color);
	force = Vector3(0, 0, 0);
}

Particle::~Particle()
{
	renderItem->release();
}

void Particle::integrate(double t)
{
	pose.p = pose.p + vel * t;
	Vector3 acceleration = force * Imass;
	vel += acceleration * t;
	vel *= powf(damping, t);
	actualTime += t;

	clearForce();
}

bool Particle::checkTime()
{
	return actualTime >= lifespan;
}
