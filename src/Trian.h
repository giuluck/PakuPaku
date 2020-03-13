#pragma once
#include "Poly.h"

/*
 * Triangle
 */
class Trian :
	public Poly
{
public:
	Trian(double s);
	Trian(double s, Color c);
	Trian(double s, Color edge, Color center);
	virtual ~Trian();
	
	double getSide();
	virtual void draw();

private:
	static const int N;
	static const double R;

	double side;
};

const int Trian::N = 3;
const double Trian::R = 1.0 / sqrt(3);

Trian::Trian(double s) : Trian(s, World::BACKGROUND_COLOR) { }

Trian::Trian(double s, Color c) : Trian(s, c, c) { }

Trian::Trian(double s, Color edge, Color center) : Poly(N, R, edge, center)
{
	this->side = s;
}

Trian::~Trian() { }


inline void Trian::draw()
{
	glPushMatrix();
	glScaled(side, side, 1);
	Poly::draw();
	glPopMatrix();
}

inline double Trian::getSide()
{
	return this->side;
}
