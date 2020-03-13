#pragma once
#include "Shape.h"

class Arrow :
	public Shape
{
public:
	Arrow(Direction direction, double width, double height, Color borderColor, Color fillColor);
	virtual ~Arrow();
	virtual void draw();

private:
	static const int NUM_VERTICES;

	Shape *border;
	Direction direction;
	double width, height;
};

const int Arrow::NUM_VERTICES = 7;

Arrow::Arrow(Direction direction, double width, double height, Color borderColor, Color fillColor) : Shape(NUM_VERTICES)
{
	this->direction = direction;
	this->width = width;
	this->height = height;

	Point vs[NUM_VERTICES] = { { 0.1, 0.2 }, { 0.1, 0.5 }, { 0.5, 0.0 }, { 0.1, -0.5 }, { 0.1, -0.2 }, { -0.5, -0.2 }, { -0.5, 0.2 } };
	Color fcs[NUM_VERTICES] = { fillColor, fillColor, fillColor, fillColor, fillColor, fillColor, fillColor };
	Color bcs[NUM_VERTICES] = { borderColor, borderColor, borderColor, borderColor, borderColor, borderColor, borderColor };
	setVertices(vs)->setColors(fcs)->setRenderingMode(GL_POLYGON);
	border = new Shape(NUM_VERTICES, vs, bcs, GL_LINE_LOOP);
}

Arrow::~Arrow() 
{
	delete border;
}

inline void Arrow::draw()
{
	glLineWidth(1.0);
	glPushMatrix();
	glRotated(direction, 0.0, 0.0, 1.0);
	glScaled(width, height, 1.0);
	Shape::draw();
	border->draw();
	glPopMatrix();
	glLineWidth((GLfloat)Dimension::LINE_WIDTH);
}
