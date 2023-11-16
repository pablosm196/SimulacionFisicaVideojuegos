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
	delete Gravity;
	delete Wind;
	delete Torbellino;
	delete Explosion;
}

void ParticleSystem::update(double t)
{
	if(Explosion->getActive())
		Explosion->updateTime(t);

	ForceRegistry->updateForces(t);

	//Borrar partículas
	for (auto i = _particles.begin(); i != _particles.end();) {
		(*i)->integrate(t);

		if ((*i)->checkTime()) {
			ForceRegistry->deleteParticleRegistry(*i);
			delete (*i);
			i = _particles.erase(i);
		}
		else ++i;
	}

	//Generar fireworks a partir de fireworks
	for (auto i = _fireworks.begin(); i != _fireworks.end();) {
		(*i)->integrate(t);

		if ((*i)->checkTime()) {
			std::list<Particle*> l = _firework_generator->generateParticlesFromFireworks((*i));
			for (Particle* p : l) {
				_fireworks.push_back((Firework*)p);
				/*ForceRegistry->addRegistry(Gravity, p);*/
				ForceRegistry->addRegistry(Wind, p);
				ForceRegistry->addRegistry(Torbellino, p);
				ForceRegistry->addRegistry(Explosion, p);
			}
			ForceRegistry->deleteParticleRegistry(*i);
			delete(*i);
			i = _fireworks.erase(i);
		}
		else ++i;
	}

	time += t;
	timeFirework += t;

	 //Generar nuevas partículas
	if (time >= newParticle) {
		newParticle += NEW_PARTICLE_TIME;
		for (ParticleGenerator* p : _particle_generators) {
			if (p->getActive()) {
				std::list<Particle*> l = p->generateParticles();
				for (Particle* np : l) {
					/*ForceRegistry->addRegistry(Gravity, np);*/
					ForceRegistry->addRegistry(Wind, np);
					ForceRegistry->addRegistry(Torbellino, np);
					ForceRegistry->addRegistry(Explosion, np);
					_particles.push_back(np);
				}
			}
		}
	}

	//Generar nuevos fireworks
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
	if (_firework_generator->getActive()) {
			std::list<Particle*> l = _firework_generator->generateParticles();
			for (Particle* nf : l) {
				/*ForceRegistry->addRegistry(Gravity, nf);*/
				ForceRegistry->addRegistry(Wind, nf);
				ForceRegistry->addRegistry(Torbellino, nf);
				ForceRegistry->addRegistry(Explosion, nf);
				_fireworks.push_back((Firework*)nf);
			}
		}
}

void ParticleSystem::generateExplosion()
{
	Explosion->setActive();
}
