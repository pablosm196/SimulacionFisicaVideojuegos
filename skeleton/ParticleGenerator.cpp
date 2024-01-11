#include "ParticleGenerator.h"

void ParticleGenerator::setParticle(Particle* model)
{
	_model = model;
}

GaussianParticleGenerator::GaussianParticleGenerator(Vector3 pos, Vector3 vel, double t, int n)
{
	srand(time(0));
	std_dev_pos = pos;
	std_dev_vel = vel;
	std_dev_t = t;

	_num_particles = n;
}

std::list<Particle*> GaussianParticleGenerator::generateParticles()
{
	srand(time(0));
	std::list<Particle*> particles;

	Vector3 pos, vel;
	float r, g, b, mass;
	for (int i = 0; i < _num_particles; ++i) {
		pos.x = std_dev_pos.x + dist(dre) * 2;
		pos.y = std_dev_pos.y + dist(dre) * 2;
		pos.z = std_dev_pos.z + dist(dre) * 2;

		vel.x = std_dev_vel.x + dist(dre) * 1;
		vel.y = std_dev_vel.y + dist(dre);
		vel.z = std_dev_vel.z + dist(dre) * 1;

		r = 0;
		g = 0;
		b = 1;

		mass = rand() % 10;

		Particle* newParticle = new Particle(vel, pos, 0.998f, mass, 5.0f, Vector4(r, g, b, 1));
		particles.push_back(newParticle);
	}

	return particles;
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

	Vector3 pos, vel;
	float r, g, b, mass;
	for (int i = 0; i < _num_particles; ++i) {
		pos.x = _pos_width.x + dist(dre) * 2;
		pos.y = _pos_width.y + dist(dre) * 2;
		pos.z = _pos_width.z + dist(dre) * 2;

		vel.x = _vel_width.x + dist(dre) * 1;
		vel.y = _vel_width.y + dist(dre);
		vel.z = _vel_width.z + dist(dre) * 1;

		r = 0;
		g = 0;
		b = 1;

		mass = rand() % 10;

		Particle* newParticle = new Particle(vel, pos, 0.998f, mass, 5.0f, Vector4(r, g, b, 1));
		particles.push_back(newParticle);
	}

	return particles;
}

std::list<Particle*> FireworkGenerator::generateRandom(Firework* parent)
{
	std::list<Particle*> particles;
	float r, g, b;
	Vector3 vel;
	for (int i = 0; i < parent->getNumHijos(); ++i) {
		vel.x = parent->getVel().x + dist(dre) * 10;
		vel.y = parent->getVel().y + dist(dre);
		vel.z = parent->getVel().z + dist(dre) * 10;

		r = rand() % 255 / 255.0f;
		g = rand() % 255 / 255.0f;
		b = rand() % 255 / 255.0f;
		particles.push_back(new Firework(vel, parent->getPos(), 0.998f, 1, parent->getType(), 2.0f, Vector4(r, g, b, 1), 2));
	}
	return particles;
}

std::list<Particle*> FireworkGenerator::generateCircle(Firework* parent)
{
	std::list<Particle*> list;
	float r = 10.0f, angle;
	Vector3 pos;
	for (int i = 0; i < parent->getNumHijos(); ++i) {
		angle = i * 360.0f / parent->getNumHijos() * 3.14f / 180;
		pos = parent->getPos() + Vector3(r * cos(angle), r * sin(angle), 0);
		list.push_back(new Firework(Vector3(0, 0, 0), pos, 0.998f, 1, Firework::CIRCLE, 2.0f, parent->getColor(), parent->getNumHijos()));
	}
	return list;
}

FireworkGenerator::FireworkGenerator(Vector3 pos, Vector3 vel, Vector3 ac, int n)
{
	srand(time(0));
	_mean_pos = pos;
	_mean_vel = vel;
	_mean_ac = ac;
	_num_particles = n;
}

std::list<Particle*> FireworkGenerator::generateParticles()
{
	std::list<Particle*> fireworks;
	float r, g, b;
	Vector3 pos, vel;
	for (int i = 0; i < _num_particles; ++i) {

		pos.x = _mean_pos.x + dist(dre);
		pos.y = _mean_pos.y + dist(dre);
		pos.z = _mean_pos.z + dist(dre);

		vel.x = _mean_vel.x + dist(dre);
		vel.y = _mean_vel.y + dist(dre) * 2;
		vel.z = _mean_vel.z + dist(dre);

		r = rand() % 255 / 255.0f;
		g = rand() % 255 / 255.0f;
		b = rand() % 255 / 255.0f;
		fireworks.push_back(new Firework(vel, pos, 0.998f, 1, Firework::RANDOM, 2.0f, Vector4(r, g, b, 1), 5));
	}
	return fireworks;
}

std::list<Particle*> FireworkGenerator::generateParticlesFromFireworks(Firework* parent)
{
	std::list<Particle*> list;
	switch (parent->getType()) {
	case Firework::RANDOM:
		list =  generateRandom(parent); break;
	case Firework::CIRCLE:
		list = generateCircle(parent); break;
	default: break;
	}
	return list;
}

GaussianRigidGenerator::GaussianRigidGenerator(Vector3 pos, Vector3 vel, double t, int n, int max, PxPhysics* px_physics, PxScene* px_scene) : GaussianParticleGenerator(pos, vel, t, n)
{
	maxParticles = max;
	currentParticles = 0;
	physics = px_physics;
	scene = px_scene;
}

std::list<Particle*> GaussianRigidGenerator::generateParticles()
{
	srand(time(0));
	std::list<Particle*> particles;

	Vector3 pos, vel;
	float r, g, b;
	for (int i = 0; i < _num_particles && currentParticles < maxParticles; ++i) {
		pos.x = std_dev_pos.x + dist(dre) * 0.5;
		pos.y = std_dev_pos.y + dist(dre) * 0.5;
		pos.z = std_dev_pos.z + dist(dre) * 0.5;

		vel.x = std_dev_vel.x + dist(dre);
		vel.y = std_dev_vel.y + dist(dre);
		vel.z = std_dev_vel.z + dist(dre);

		r = rand() % 255 / 255.0f;
		g = rand() % 255 / 255.0f;
		b = rand() % 255 / 255.0f;

		RigidParticle* newParticle = new RigidParticle(vel, pos, 1, physics, scene ,5.0f, Vector4(r, g, b, 1));
		newParticle->setGenerator(this);
		particles.push_back(newParticle);
		currentParticles++;
	}

	return particles;
}
