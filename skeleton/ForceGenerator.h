#pragma once
#include "Particle.h"
class ForceGenerator {
protected:
	bool active = false;
public:
	virtual void updateForce(Particle* particle, double t) = 0;
	std::string _name;
	double _t = 0.0; // If starting negative --> eternal
	double _duration = -1e10;
	inline void setActive(bool ac) { active = ac; }
	inline bool getActive() { return active; }
	inline bool updateTime(double t) {
		_t += t;
		return _t < _duration || _duration < 0.0; // Devuelve true si ya era cero o si es positivo
	}
	virtual ~ForceGenerator() {}
};

class GravityForceGenerator : public ForceGenerator {
protected:
	Vector3 _gravity;
public:
	GravityForceGenerator(const Vector3& g) : _gravity(g) { active = false; _name = "gravity"; };
	void updateForce(Particle* p, double t) override;
	inline void setGravity(Vector3 g) { _gravity = g; }
};

class WindGenerator : public ForceGenerator {
protected:
	Vector3 _windVelocity, _position;
	float k1 = 0, k2 = 0, radius;
public:
	WindGenerator(Vector3 p, Vector3 v, float r, float coef1 = 0, float coef2 = 0) : _position(p), _windVelocity(v), radius(r), k1(coef1), k2(coef2) { active = false;  _name = "wind"; };
	void updateForce(Particle* p, double t) override;
	inline void setWindVelocity(Vector3 v) { _windVelocity = v; }
	inline void setK1(float coef1) { k1 = coef1; }
	inline void setK2(float coef2) { k2 = coef2; }
};

class TorbellinoGenerator : public WindGenerator {
protected:
	float K;
public:
	TorbellinoGenerator(Vector3 p, Vector3 v, float r, float force, float coef1 = 0, float coef2 = 0) : WindGenerator(p, v, r, coef1, coef2), K(force) { active = false; _name = "torbellino"; };
	void updateForce(Particle* p, double t) override;
};

class ExplosionGenerator : public ForceGenerator {
protected:
	Vector3 _position, _velocity;
	float K, radius, timeConst, time;
public:
	ExplosionGenerator(Vector3 p, Vector3 v, float force, float r = 5.0f, float t = 1.0f) : _position(p), _velocity(v), K(force), radius(r), timeConst(t), time(0) { active = false; _name = "explosion"; };
	void updateForce(Particle* p, double t) override;
	inline void updateTime(double t) { time += t; }
	inline void setPosition(Vector3 pos) { _position = pos; }
};

class SpringForceGenerator : public ForceGenerator {
protected:
	float k, resting_length;
	Particle* other;
public:
	SpringForceGenerator(float _k, float resting, Particle* p) : k(_k), resting_length(resting), other(p) { _name = "muelle"; };
	virtual ~SpringForceGenerator() = default;
	void updateForce(Particle* p, double t) override;
	inline void setK(float _k) { k = _k; }
};

class AnchoredSpring : public SpringForceGenerator {
public:
	AnchoredSpring(float _k, float resting, Vector3 position) : SpringForceGenerator(_k, resting, nullptr) {
		other = new Particle({ 0, 0, 0 }, position, 0.998f, 0, 1000, { 0, 0, 0, 1 }, BOX);
		_name = "muelleAnclado";
	}
	virtual ~AnchoredSpring() { delete other; }
};

class ElasticRubber : public SpringForceGenerator {
public:
	ElasticRubber(float _k, float resting, Particle* p) : SpringForceGenerator(_k, resting, p) { _name = "gomaElastica"; };
	void updateForce(Particle* p, double t) override;
};

class BuoyancyForceGenerator : public ForceGenerator {
protected:
	float density, gravity;
	Particle* liquid;
	Vector3 current;
public:
	BuoyancyForceGenerator(Particle* l, float g, Vector3 corriente = { 0, 0, 0 });
	void updateForce(Particle* p, double t) override;
};


