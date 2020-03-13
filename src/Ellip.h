#pragma once
#include "Poly.h"

/*
 * Ellipse
 */
class Ellip :
	public Poly
{
public:
	Ellip(double r);
	Ellip(double r, Color c);
	Ellip(double r, Color edge, Color center);
	Ellip(double rx, double ry);
	Ellip(double rx, double ry, Color c);
	Ellip(double rx, double ry, Color edge, Color center);
	virtual ~Ellip();
	
	virtual double getRadiusX();
	virtual double getRadiusY();
	virtual void draw();

private:
	static const int N;
	static const double R;

	double radiusX, radiusY;
};

const int Ellip::N = 30;
const double Ellip::R = 1.0;

Ellip::Ellip(double r) : Ellip(r, r) { }

Ellip::Ellip(double r, Color c) : Ellip(r, r, c) { }

Ellip::Ellip(double r, Color edge, Color center) : Ellip(r, r, edge, center) { }

Ellip::Ellip(double rx, double ry) : Ellip(rx, ry, World::BACKGROUND_COLOR) { }

Ellip::Ellip(double rx, double ry, Color c) : Ellip(rx, ry, c, c) { }

Ellip::Ellip(double rx, double ry, Color edge, Color center) : Poly(N, R, edge, center) {
	this->radiusX = rx;
	this->radiusY = ry;
}

Ellip::~Ellip() { }

inline double Ellip::getRadiusX()
{
	return this->radiusX;
}

inline double Ellip::getRadiusY()
{
	return this->radiusY;
}

inline void Ellip::draw()
{
	glPushMatrix();
	glScaled(radiusX, radiusY, 1);
	Poly::draw();
	glPopMatrix();
}
