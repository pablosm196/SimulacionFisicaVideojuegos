#include "ForceGenerator.h"

void GravityForceGenerator::updateForce(Particle* p)
{
	if (p->getMass() < 1e-10)
		return;
	p->addForce(_gravity * p->getMass());
}
