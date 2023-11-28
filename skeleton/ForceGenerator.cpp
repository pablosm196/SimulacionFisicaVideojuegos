#include "ForceGenerator.h"

void GravityForceGenerator::updateForce(Particle* p, double t)
{
	if (active) {
		if (p->getMass() < 1e-10)
			return;
		p->addForce(_gravity * p->getMass());
	}
}

void WindGenerator::updateForce(Particle* p, double t)
{
	if (active) {
		if (p->getMass() < 1e-10)
			return;

		if(p->getPos().x < _position.x + radius && p->getPos().y < _position.y + radius && p->getPos().z < _position.z + radius)
			p->addForce(k1 * (_windVelocity - p->getVel() + k2 * (_windVelocity - p->getVel()).magnitude() * (_windVelocity - p->getVel())));
	}
}

void TorbellinoGenerator::updateForce(Particle* p, double t)
{
	if (active) {
		if (p->getMass() < 1e-10)
			return;
		Vector3 vel = K * Vector3(-(p->getPos().z - _position.z), 50 - (p->getPos().y - _position.y), p->getPos().x - _position.x);
		if (p->getPos().x < _position.x + radius && p->getPos().y < _position.y + radius && p->getPos().z < _position.z + radius)
			p->addForce(k1 * (vel - p->getVel()) + k2 * (vel - p->getVel()).magnitude() * (vel - p->getVel()));
	}
}

void ExplosionGenerator::updateForce(Particle* p, double t)
{
	if (active) {
		if (p->getMass() < 1e-10)
			return;

		if (time < 4 * timeConst) {
			radius = _velocity.magnitude() * time;

			float r = sqrt(powf(p->getPos().x - _position.x, 2) + powf(p->getPos().y - _position.y, 2) + powf(p->getPos().x - _position.x, 2));

			Vector3 force = K / (r * r) * (p->getPos() - _position) * exp(- time / timeConst);

			if (r < radius)
				p->addForce(force);
		}
	}
}

void SpringForceGenerator::updateForce(Particle* p, double t)
{
	Vector3 distance = other->getPos() - p->getPos();
	
	const float length = distance.normalize();
	const float delta = length - resting_length;

	p->addForce(distance * delta * k);
}

void ElasticRubber::updateForce(Particle* p, double t)
{
	Vector3 distance = other->getPos() - p->getPos();

	if (distance.magnitude() < resting_length)
		SpringForceGenerator::updateForce(p, t);
}
