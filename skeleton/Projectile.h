#pragma once
#include "Particle.h"
class Projectile : public Particle
{
private:
	float Imass;
	Vector3 acceleration;
	void escalate(Vector3 rv, Vector3 fv, Vector3 a, float m);
public:
	Projectile(Vector3 rv, Vector3 fv, Vector3 p, Vector3 a, float d, float m) : Particle(fv, p, d, m) {
		escalate(rv, fv, a, m);
	};
};

