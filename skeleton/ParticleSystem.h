#pragma once
#include<list>
#include "Particle.h"
class ParticleSystem
{
private:
	std::list<Particle*> _particles;
	//std::list<ParticleGenerator*> _particle_generators;
public:
	void update(double t);
	//ParticleGenerator* getParticleGenerator(string name);
	void generateFireworkSystem();
};

