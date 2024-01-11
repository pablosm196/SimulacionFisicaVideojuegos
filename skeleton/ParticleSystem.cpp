#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::generateSpringDemo()
{
	/**/Particle* p1 = new Particle({0, 0, 0}, {10, 0, 0}, 0.998f, 1, 1000);
	Particle* p2 = new Particle({ 0, 0, 0 }, { -10, 0, 0 }, 0.998f, 1, 1000, {0, 0, 255, 1});

	//SpringForceGenerator* f1 = new SpringForceGenerator(1, 10, p2);
	ElasticRubber* f1 = new ElasticRubber(5, 10, p2);
	ForceRegistry->addRegistry(f1, p1);
	for (ForceGenerator* fg : _forces)
		ForceRegistry->addRegistry(fg, p1);

	//SpringForceGenerator* f2 = new SpringForceGenerator(1, 10, p1);
	ElasticRubber* f2 = new ElasticRubber(5, 10, p1);
	ForceRegistry->addRegistry(f2, p2);
	for (ForceGenerator* fg : _forces)
		ForceRegistry->addRegistry(fg, p1);

	_particles.push_back(p1);
	_particles.push_back(p2);

	/**/Particle* p3 = new Particle({0, 0, 0}, {20, -5, 0}, 0.998f, 1, 1000);
	AnchoredSpring* muelle = new AnchoredSpring(10, 10, { 20, 10, 0 });
	ForceRegistry->addRegistry(muelle, p3);

	for (ForceGenerator* fg : _forces)
		ForceRegistry->addRegistry(fg, p1);

	_particles.push_back(p3);

	water = new Particle(Vector3(0, 0, 0), Vector3(0, 0, -20), 0.998f, 1, 10000, Vector4(0, 0, 255, 1), BOX);
	water->setBoxSize(40, 1, 5);
	water->setMass(water->getVolumen() * 1000);

	BuoyancyForceGenerator* buoyancy = new BuoyancyForceGenerator(water, 9.8);
	
	Particle* p4 = new Particle({ 0, 0, 0 }, { -20, 2.99994695, -20 }, 0.998f, 1, 1000, { 255, 0, 0, 1 }, SPHERE); //Partícula que se hunde (más denso que el agua)
	p4->setSphereRadius(3);
	p4->setMass(p4->getVolumen() * 2000);
	for (ForceGenerator* fg : _forces)
		ForceRegistry->addRegistry(fg, p1);
	ForceRegistry->addRegistry(buoyancy, p4);

	Particle* p5 = new Particle({ 0, 0, 0 }, { -10, 10, -20 }, 0.998f, 1, 1000, {255, 0, 0, 1}, BOX); //Partícula que incialmente no está tocando el agua 
	p5->setBoxSize(1, 3, 1);
	for (ForceGenerator* fg : _forces)
		ForceRegistry->addRegistry(fg, p1);
	ForceRegistry->addRegistry(buoyancy, p5);

	Particle* p6 = new Particle({ 0, 0, 0 }, { 0, 5, -20 }, 0.998f, 1, 1000, { 255, 0, 0, 1 }, BOX); //Partícula que está tocando el agua
	p6->setBoxSize(1, 3, 1);
	for (ForceGenerator* fg : _forces)
		ForceRegistry->addRegistry(fg, p1);
	ForceRegistry->addRegistry(buoyancy, p6);

	Particle* p7 = new Particle({ 0, 0, 0 }, { 10, 1.499, -20 }, 0.998f, 1, 1000, { 255, 0, 0, 1 }, BOX); //Fg = E (Partícula quieta)
	p7->setBoxSize(1, 3, 1);
	for (ForceGenerator* fg : _forces)
		ForceRegistry->addRegistry(fg, p1);
	ForceRegistry->addRegistry(buoyancy, p7);

	Particle* p8 = new Particle({ 0, 0, 0 }, { 10, -1.5, -20 }, 0.998f, 1, 1000, { 255, 0, 0, 1 }, BOX); //Partícula inicialmente hundida (desaparece porque hace demasiada fuerza)
	p8->setBoxSize(1, 3, 1);
	p8->setMass(3000);
	for (ForceGenerator* fg : _forces)
		ForceRegistry->addRegistry(fg, p1);
	ForceRegistry->addRegistry(buoyancy, p8);

	_particles.push_back(p4);
	_particles.push_back(p5);
	_particles.push_back(p6);
	_particles.push_back(p7);
	_particles.push_back(p8);

	_forcesSpring.push_back(f1);
	_forcesSpring.push_back(f2);
	_forcesSpring.push_back(muelle);
	_forcesSpring.push_back(buoyancy);

	_forces.front()->setActive(true);
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
	for (auto i = _forces.begin(); i != _forces.end();) {
		delete(*i);
		i = _forces.erase(i);
	}
	for (auto i = _forcesSpring.begin(); i != _forcesSpring.end();) {
		delete(*i);
		i = _forcesSpring.erase(i);
	}
	
	
	delete _firework_generator;
	delete _firework_generator_2;
	//delete water;
}

void ParticleSystem::update(double t)
{
	auto it = _forces.begin();
	while (it != _forces.end() && (*it)->_name != "explosion") ++it;
	if (it != _forces.end() && (*it)->getActive()) (*it)->updateTime(t);

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
				for (ForceGenerator* fg : _forces)
					ForceRegistry->addRegistry(fg, p);
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
					for (ForceGenerator* fg : _forces)
						ForceRegistry->addRegistry(fg, np);
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
			for (ForceGenerator* fg : _forces)
				ForceRegistry->addRegistry(fg, nf);
			_fireworks.push_back((Firework*)nf);
		}
	}

	if (_firework_generator_2->getActive()) {
		std::list<Particle*> l = _firework_generator_2->generateParticles();
		for (Particle* nf : l) {
			for (ForceGenerator* fg : _forces)
				ForceRegistry->addRegistry(fg, nf);
			_fireworks.push_back((Firework*)nf);
		}
	}
}

void ParticleSystem::generateExplosion()
{
	auto it = _forces.begin();
	while (it != _forces.end() && (*it)->_name != "explosion") ++it;
	if (it != _forces.end()) (*it)->setActive(true);
}

void ParticleSystem::setGravity(bool g)
{
	auto it = _forces.begin();
	while (it != _forces.end() && (*it)->_name != "gravity") ++it;
	if (it != _forces.end()) (*it)->setActive(true);
}

void ParticleSystem::setTorbellino(bool t)
{
	auto it = _forces.begin();
	while (it != _forces.end() && (*it)->_name != "torbellino") ++it;
	if (it != _forces.end()) (*it)->setActive(true);
}

void ParticleSystem::setWind(bool w)
{
	auto it = _forces.begin();
	while (it != _forces.end() && (*it)->_name != "wind") ++it;
	if (it != _forces.end()) (*it)->setActive(true);
}
