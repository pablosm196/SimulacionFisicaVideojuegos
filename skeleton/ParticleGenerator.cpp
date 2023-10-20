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

UniformParticleGenerator::UniformParticleGenerator(Vector3 pos, Vector3 vel, int n)
{
	_pos_width = pos;
	_vel_width = vel;

	_num_particles = n;
}

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
	std::list<Particle*> particles;

	for (int i = 0; i < _num_particles; ++i) {
		Vector3 pos, vel;
		pos.x = _pos_width.x + dist(dre);
		pos.y = _pos_width.y + dist(dre);
		pos.z = _pos_width.z + dist(dre);

		vel.x = _vel_width.x + dist(dre);
		vel.y = _vel_width.y + dist(dre);
		vel.z = _vel_width.z + dist(dre);

		Particle* newParticle = new Particle(vel, pos, Vector3(0, -9.8f, 0), 0.998);
		particles.push_back(newParticle);
	}

	return particles;
}
