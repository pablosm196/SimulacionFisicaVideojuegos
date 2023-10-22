#include "ParticleSystem.h"

ParticleSystem::~ParticleSystem()
{
	for (auto i = _particles.begin(); i != _particles.end();) {
		delete(*i);
		i = _particles.erase(i);
	}
	for (auto i = _particle_generators.begin(); i != _particle_generators.end();) {
		delete(*i);
		i = _particle_generators.erase(i);
	}
}

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
	time += t;
	if (time >= newParticle) {
		newParticle += NEW_PARTICLE_TIME;
		for (ParticleGenerator* p : _particle_generators) {
			std::list<Particle*> l = p->generateParticles();
			for (Particle* np : l)
				_particles.push_back(np);
		}
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{
	return nullptr;
}

void ParticleSystem::generateFireworkSystem()
{
}
