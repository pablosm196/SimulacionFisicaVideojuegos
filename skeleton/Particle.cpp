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

Particle::Particle(Vector3 v, Vector3 p, float m, float d, float t, Vector4 col, Shape s)
{
	vel = v;
	pose = PxTransform(p);
	damping = d;
	mass = m;
	if (mass > 0)
		Imass = (float) (1.0f / mass);
	else
		Imass = 0;
	lifespan = t;
	color = col;
	shape = s;
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

float Particle::getHeight()
{
	if (shape == BOX) {
		return renderItem->shape->getGeometry().box().halfExtents.y;
	}
	else if (shape == SPHERE) {
		return 2 * renderItem->shape->getGeometry().sphere().radius;
	}
	return 0.0f;
}

float Particle::getVolumen()
{
	if (shape == BOX) {
		Vector3 box = renderItem->shape->getGeometry().box().halfExtents;
		return box.x * box.y * box.z;
	}
	else if (shape == SPHERE) {
		float r = renderItem->shape->getGeometry().sphere().radius;
		return (4.0f / 3.0f) * PxPi * r * r * r;
	}
	return 0.0f;
}

void Particle::setBoxSize(float w, float h, float l)
{
	shape = BOX;
	renderItem->shape->release();
	renderItem->shape->setGeometry(PxBoxGeometry(w, h, l));
}

void Particle::setSphereRadius(float r)
{
	shape = SPHERE;
	renderItem->shape->release();
	renderItem->shape->setGeometry(PxSphereGeometry(r));
}
