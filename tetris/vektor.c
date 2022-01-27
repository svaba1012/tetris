#include <stdlib.h>
#include <stdio.h>
#include "vektor.h"

struct vektor saberiVektore(struct vektor a, struct vektor b){
	a.x += b.x;
	a.y += b.y;
	a.z += a.z;
	return a;
}

struct vektor pomnoziSkalarom(struct vektor a, double b){
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

double skalarniProizvod(struct vektor a, struct vektor b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct vektor vektorskiProizvod(struct vektor a, struct vektor b);
