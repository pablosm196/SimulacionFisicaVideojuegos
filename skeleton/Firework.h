#pragma once
#include "Particle.h"
#include"list"
#include<random>
class Firework : public Particle
{
protected:
	int nHijos;
public:
	Firework(Vector3 v, Vector3 p, Vector3 a, float d, float t = 5000.0f, Vector4 col = { 255, 0, 0, 1 }, int n = 1);
	inline int getNumHijos() { return nHijos; }
};

