#pragma once
#include "Poly.h"
#include <math.h>

/*
 * Rectangle
 */
class Rect :
	public Poly
{
public:
	Rect(double s);
	Rect(double s, Color c);
	Rect(double s, Color edge, Color center);
	Rect(double w, double h);
	Rect(double w, double h, Color c);
	Rect(double w, double h, Color edge, Color center);
	virtual ~Rect();
	
	double getHeight();
	double getWidth();
	virtual void draw();

private:
	static const int N;
	static const double R;
	static const int ROTATION_ANGLE;

	double height, width;
};

const int Rect::N = 4;
const double Rect::R = 1 / sqrt(2);
const int Rect::ROTATION_ANGLE = -45;

Rect::Rect(double s) : Rect(s, s) { }

Rect::Rect(double s, Color c) : Rect(s, s, c) { }

Rect::Rect(double s, Color edge, Color center) : Rect(s, s, edge, center) { }

Rect::Rect(double w, double h) : Rect(w, h, World::BACKGROUND_COLOR) { }

Rect::Rect(double w, double h, Color c) : Rect(w, h, c, c) { }

Rect::Rect(double w, double h, Color edge, Color center) : Poly(N, R, edge, center) {
	this->width = w;
	this->height = h;
}

Rect::~Rect() { }

inline void Rect::draw()
{
	glPushMatrix();
	glScaled(width, height, 1);
	glRotated(ROTATION_ANGLE, 0, 0, 1);
	Poly::draw();
	glPopMatrix();
}

inline double Rect::getHeight()
{
	return this->height;
}

inline double Rect::getWidth()
{
	return this->width;
}
