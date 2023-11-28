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

Particle::Particle(Vector3 v, Vector3 p, float m, float d, float t, Vector4 col, shape s)
{
	vel = v;
	pose = PxTransform(p);
	damping = d;
	mass = m;
	if (mass > 0)
		Imass = 1 / mass;
	else
		Imass = 0;
	lifespan = t;
	color = col;
	if(s == SPHERE)
		renderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &pose, color);
	else if(s == BOX)
		renderItem = new RenderItem(CreateShape(PxBoxGeometry(1, 1, 1)), &pose, color);
	force = Vector3(0, 0, 0);
}

Particle::~Particle()
{
	renderItem->release();
}

void Particle::integrate(double t)
{
	Vector3 acceleration = force * Imass;
	vel += acceleration * t;
	vel *= powf(damping, t);

	pose.p = pose.p + vel * t;

	actualTime += t;

	clearForce();
}

bool Particle::checkTime()
{
	return actualTime >= lifespan;
}
