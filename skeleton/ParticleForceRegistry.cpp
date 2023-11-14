#include "ParticleForceRegistry.h"

void ParticleForceRegistry::updateForces(double t)
{
	for (auto it = begin(); it != end(); ++it) {
		it->first->updateForce(it->second);
	}
}

void ParticleForceRegistry::addRegistry(ForceGenerator* fg, Particle* p)
{
	insert({ fg, p });
}

void ParticleForceRegistry::deleteParticleRegistry(Particle* p)
{
	for (auto it = begin(); it != end();)
	{
		if (it->second == p)
			it = erase(it);
		else
			it++;
	}
}
