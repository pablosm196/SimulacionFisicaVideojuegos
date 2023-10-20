#pragma once
#include<list>
#include "Particle.h"
#include"ParticleGenerator.h"
class ParticleSystem
{
private:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _particle_generators;
public:
	ParticleSystem() : _particles(0), _particle_generators(0) {};
	void update(double t);
	ParticleGenerator* getParticleGenerator(std::string name);
	void generateFireworkSystem();
};

