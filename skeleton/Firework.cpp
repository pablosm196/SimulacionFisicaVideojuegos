#include "Firework.h"

Firework::Firework(Vector3 v, Vector3 p, Vector3 a, float d, float t, Vector4 col, int n) : Particle(v, p, a, d, t, col)
{
	nHijos = n;
}
