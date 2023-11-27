#include "ParticleSystem.h"

void ParticleSystem::generateSpringDemo()
{
	/*Particle* p1 = new Particle({0, 0, 0}, {10, 0, 0}, 0.998f, 2, 1000);
	Particle* p2 = new Particle({ 0, 0, 0 }, { -10, 0, 0 }, 0.998f, 2, 1000);

	SpringForceGenerator* f1 = new SpringForceGenerator(1, 10, p2);
	ForceRegistry->addRegistry(f1, p1);
	SpringForceGenerator* f2 = new SpringForceGenerator(1, 10, p1);
	ForceRegistry->addRegistry(f2, p2);
	_particles.push_back(p1);
	_particles.push_back(p2);*/

	Particle* p1 = new Particle({ 0, 0, 0 }, { -10, 0, 0 }, 0.998f, 2, 1000);
	AnchoredSpring* f1 = new AnchoredSpring(1, 10, { 10, 0, 0 });
	ForceRegistry->addRegistry(f1, p1);
	_particles.push_back(p1);
}

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
				ForceRegistry->addRegistry(Gravity, p);
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
					ForceRegistry->addRegistry(Gravity, np);
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
				ForceRegistry->addRegistry(Gravity, nf);
				ForceRegistry->addRegistry(Wind, nf);
				ForceRegistry->addRegistry(Torbellino, nf);
				ForceRegistry->addRegistry(Explosion, nf);
				_fireworks.push_back((Firework*)nf);
			}
		}
}

void ParticleSystem::generateExplosion()
{
	Explosion->setActive(true);
}

void ParticleSystem::setGravity(bool g)
{
	Gravity->setActive(g);
}

void ParticleSystem::setTorbellino(bool t)
{
	Torbellino->setActive(t);
}

void ParticleSystem::setWind(bool w)
{
	Wind->setActive(w);
}
