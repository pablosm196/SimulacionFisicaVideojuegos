#include "Particle.h"

Particle::Particle()
{
	vel = Vector3(0, 0, 0);
	acceleration = Vector3(0, 0, 0);
	pose = PxTransform(Vector3(0, 0, 0));
	damping = 0.998f;
	lifespan = 1000.0f;
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(10)), Vector4(255, 0, 0, 1));
}

Particle::Particle(Vector3 v, Vector3 p, Vector3 a, float d, float t, Vector4 col)
{
	vel = v;
	pose = PxTransform(p);
	acceleration = a;
	damping = d;
	lifespan = t;
	color = col;
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(10)), &pose, color);
}

Particle::~Particle()
{
	renderItem->release();
}

void Particle::integrate(double t)
{
	pose.p = pose.p + vel * t;
	vel += acceleration * t;
	vel *= powf(damping, t);
	actualTime += t;
}

bool Particle::checkTime()
{
	return actualTime >= lifespan;
}
