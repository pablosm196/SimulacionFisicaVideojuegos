#include "Particle.h"

Particle::Particle()
{
	vel = Vector3(0, 0, 0);
	acceleration = Vector3(0, 0, 0);
	pose = PxTransform(Vector3(0, 0, 0));
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(10)), Vector4(255, 0, 0, 1));
}

Particle::Particle(Vector3 v, Vector3 p, Vector3 a)
{
	vel = v;
	pose = PxTransform(p);
	acceleration = a;
	PxShape* shape = CreateShape(PxSphereGeometry(10));
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(10)), &pose, Vector4(255, 0, 0, 1));
}

Particle::~Particle()
{
	renderItem->release();
}

void Particle::integrate(double t)
{
	pose.p = pose.p + vel * t + 0.5 * acceleration * t * t;
}
