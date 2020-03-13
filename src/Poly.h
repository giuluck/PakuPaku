#pragma once
#include "Shape.h"
#include <math.h>

/*
 * Generic 2D Regular Polygon Inscribed in a Circle with Radius "r"
 */
class Poly :
	public Shape
{
public:
	Poly(int n, double r);
	Poly(int n, double r, Color color);
	Poly(int n, double r, Color edge, Color center);
	virtual ~Poly();

	Poly* setFillStatus(bool fill);
	bool getFillStatus();
	int getNumVertices();
	Color getEdgeColor();
	Color getCenterColor();
};

inline Poly::Poly(int n, double r) : Poly(n, r, World::BACKGROUND_COLOR) { }

inline Poly::Poly(int n, double r, Color color) : Poly(n, r, color, color) { }

/*
 * The first vertex is {0, 0} and the last vertex is duplicated, so there are n + 2 vertices.
 * This is used for radial gradient from center to edges.
 */
inline Poly::Poly(int n, double r, Color edge, Color center) : Shape(n + 2)
{
	double step = (2 * M_PI) / n;
	Point *vs = new Point[n + 2];
	Color *cs = new Color[n + 2];

	vs[0] = { 0, 0 };
	cs[0] = center;
	for (int i = 0; i < n; i++) {
		vs[i + 1] = { sin(i * step) * r, cos(i * step) * r };
		cs[i + 1] = edge;
	}
	vs[n + 1] = { 0, r };
	cs[n + 1] = edge;
	
	this->setVertices(vs);
	this->setColors(cs);
	this->setFillStatus(true);

	delete vs;
	delete cs;
}

Poly::~Poly() { }

inline Poly* Poly::setFillStatus(bool fill)
{
	if (fill) {
		// draws filled triangles using every vertex
		this->setRenderingMode(GL_TRIANGLE_FAN);
		this->setBounds(0, this->getNumVertices() + 2);
	}
	else {
		// draws lines only excluding the center (first vertex) and the duplicate (last vertex)
		this->setRenderingMode(GL_LINE_LOOP);
		this->setBounds(1, this->getNumVertices() + 1);
	}
	return this;
}

inline bool Poly::getFillStatus()
{
	return this->getRenderingMode() == GL_TRIANGLE_FAN;
}

inline int Poly::getNumVertices()
{
	return Shape::getNumVertices() - 2;
}

inline Color Poly::getEdgeColor()
{
	return this->getColors()[1];
}

inline Color Poly::getCenterColor()
{
	return this->getColors()[0];
}
