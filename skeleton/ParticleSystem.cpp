#include "ParticleSystem.h"
#include <iostream>

void ParticleSystem::generateSpringDemo()
{
	/**/Particle* p1 = new Particle({0, 0, 0}, {10, 0, 0}, 0.998f, 1, 1000);
	Particle* p2 = new Particle({ 0, 0, 0 }, { -10, 0, 0 }, 0.998f, 1, 1000, {0, 0, 255, 1});

	//SpringForceGenerator* f1 = new SpringForceGenerator(1, 10, p2);
	ElasticRubber* f1 = new ElasticRubber(5, 10, p2);
	ForceRegistry->addRegistry(f1, p1);
	ForceRegistry->addRegistry(Gravity, p1);
	ForceRegistry->addRegistry(Wind, p1);

	//SpringForceGenerator* f2 = new SpringForceGenerator(1, 10, p1);
	ElasticRubber* f2 = new ElasticRubber(5, 10, p1);
	ForceRegistry->addRegistry(f2, p2);
	ForceRegistry->addRegistry(Gravity, p2);
	//ForceRegistry->addRegistry(Wind, p2);

	_particles.push_back(p1);
	_particles.push_back(p2);

	/**/Particle* p3 = new Particle({0, 0, 0}, {20, -5, 0}, 0.998f, 1, 1000);
	muelle = new AnchoredSpring(10, 10, { 20, 10, 0 });
	ForceRegistry->addRegistry(muelle, p3);

	ForceRegistry->addRegistry(Gravity, p3);
	ForceRegistry->addRegistry(Wind, p3);
	ForceRegistry->addRegistry(Explosion, p3);

	_particles.push_back(p3);

	water = new Particle(Vector3(0, 0, 0), Vector3(0, 0, -20), 0.998f, 1, 10000, Vector4(0, 0, 255, 1), BOX);
	water->setBoxSize(40, 1, 5);
	water->setMass(water->getVolumen() * 1000);

	buoyancy = new BuoyancyForceGenerator(water, 9.8);
	
	Particle* p4 = new Particle({ 0, 0, 0 }, { -20, 2.99994695, -20 }, 0.998f, 1, 1000, { 255, 0, 0, 1 }, SPHERE); //Partícula que se hunde (más denso que el agua)
	p4->setSphereRadius(3);
	p4->setMass(p4->getVolumen() * 2000);
	ForceRegistry->addRegistry(Gravity, p4);
	ForceRegistry->addRegistry(buoyancy, p4);

	Particle* p5 = new Particle({ 0, 0, 0 }, { -10, 10, -20 }, 0.998f, 1, 1000, {255, 0, 0, 1}, BOX); //Partícula que incialmente no está tocando el agua 
	p5->setBoxSize(1, 3, 1);
	ForceRegistry->addRegistry(Gravity, p5);
	ForceRegistry->addRegistry(buoyancy, p5);

	Particle* p6 = new Particle({ 0, 0, 0 }, { 0, 5, -20 }, 0.998f, 1, 1000, { 255, 0, 0, 1 }, BOX); //Partícula que está tocando el agua
	p6->setBoxSize(1, 3, 1);
	ForceRegistry->addRegistry(Gravity, p6);
	ForceRegistry->addRegistry(buoyancy, p6);

	Particle* p7 = new Particle({ 0, 0, 0 }, { 10, 1.499, -20 }, 0.998f, 1, 1000, { 255, 0, 0, 1 }, BOX); //Fg = E (Partícula quieta)
	p7->setBoxSize(1, 3, 1);
	ForceRegistry->addRegistry(Gravity, p7);
	ForceRegistry->addRegistry(buoyancy, p7);

	Particle* p8 = new Particle({ 0, 0, 0 }, { 10, -1.5, -20 }, 0.998f, 1, 1000, { 255, 0, 0, 1 }, BOX); //Partícula inicialmente hundida (desaparece porque hace demasiada fuerza)
	p8->setBoxSize(1, 3, 1);
	p8->setMass(3000);
	ForceRegistry->addRegistry(Gravity, p8);
	ForceRegistry->addRegistry(buoyancy, p8);

	_particles.push_back(p4);
	_particles.push_back(p5);
	_particles.push_back(p6);
	_particles.push_back(p7);
	_particles.push_back(p8);

	Gravity->setActive(true);
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
	delete muelle;
	delete water;
	delete buoyancy;
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

	/*auto it = _particles.begin();
	for (int i = 0; i < _particles.size() - 1; ++i) it++;

	std::cout << (*it)->getPos().y << std::endl;*/
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
