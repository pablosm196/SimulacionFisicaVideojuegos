#include "ParticleSystem.h"

void ParticleSystem::update(double t)
{
	for (auto i = _particles.begin(); i != _particles.end();) {
		(*i)->integrate(t);
		if ((*i)->checkTime()) {
			delete (*i);
			i = _particles.erase(i);
		}
		else i++;
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{
	return nullptr;
}

void ParticleSystem::generateFireworkSystem()
{
}
