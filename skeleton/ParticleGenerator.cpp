#include "ParticleGenerator.h"

void ParticleGenerator::setParticle(Particle* model)
{
	_model = model;
}

GaussianParticleGenerator::GaussianParticleGenerator(Vector3 pos, Vector3 vel, double t)
{

}

std::list<Particle*> GaussianParticleGenerator::generateParticles()
{
	return std::list<Particle*>();
}

UniformParticleGenerator::UniformParticleGenerator(Vector3 pos, Vector3 vel)
{

}

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
	return std::list<Particle*>();
}
