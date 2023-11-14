#pragma once
#include <map>
#include "ForceGenerator.h"

class ParticleForceRegistry : public std::multimap< ForceGenerator*, Particle*>
{
public:
	void updateForces(double t);
	void addRegistry(ForceGenerator* fg, Particle* p);
	void deleteParticleRegistry(Particle* p);
};

