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
	for (auto i = _fireworks.begin(); i != _fireworks.end();) {
		delete(*i);
		i = _fireworks.erase(i);
	}
	
	delete _firework_generator;
}

void ParticleSystem::update(double t)
{
	ForceRegistry->updateForces(t);
	for (auto i = _particles.begin(); i != _particles.end();) {
		(*i)->integrate(t);

		if ((*i)->checkTime()) {
			delete (*i);
			i = _particles.erase(i);
		}
		else ++i;
	}

	for (auto i = _fireworks.begin(); i != _fireworks.end();) {
		(*i)->integrate(t);

		if ((*i)->checkTime()) {
			std::list<Particle*> l = _firework_generator->generateParticlesFromFireworks((*i));
			for (Particle* p : l)
				_fireworks.push_back((Firework*)p);

			delete(*i);
			i = _fireworks.erase(i);
		}
		else ++i;
	}

	time += t;
	timeFirework += t;

	if (time >= newParticle) {
		newParticle += NEW_PARTICLE_TIME;
		for (ParticleGenerator* p : _particle_generators) {
			std::list<Particle*> l = p->generateParticles();
			for (Particle* np : l) {
				ForceRegistry->addRegistry(Gravity, np);
				_particles.push_back(np);
			}
		}
	}

	if (timeFirework >= newFirework) {
		newFirework += NEW_FIREWORK_TIME;
		generateFireworkSystem();
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{
	return nullptr;
}

void ParticleSystem::generateFireworkSystem()
{
		std::list<Particle*> l = _firework_generator->generateParticles();
		for (Particle* nf : l) {
			ForceRegistry->addRegistry(Gravity, nf);
			_fireworks.push_back((Firework*)nf);
		}
}
