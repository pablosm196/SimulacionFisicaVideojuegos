#include "Firework.h"

Firework::Firework(Vector3 v, Vector3 p, float d, float mass, Firework_type ft, float t, Vector4 col, int n) : Particle(v, p, d, mass, t, col)
{
	nHijos = n;
	type = ft;
}
