#pragma once
#include "Particle.h"
#include"list"
#include<random>
class Firework : public Particle
{
public:
	static enum Firework_type{RANDOM, CIRCLE};
	Firework(Vector3 v, Vector3 p, float d, float mass, Firework_type ft, float t = 5000.0f, Vector4 col = { 255, 0, 0, 1 }, int n = 1);
	inline int getNumHijos() { return nHijos; }
	inline Firework_type getType() { return type; }
protected:
	int nHijos;
	Firework_type type;
};

