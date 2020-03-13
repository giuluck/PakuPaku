#pragma once
#include "Global.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

/*
 * Generic 2D Shape
 */
class Shape
{
public:
	Shape(int n);
	Shape(int n, Point vs[], Color cs[], GLenum rm);
	virtual ~Shape();
	virtual void draw();

	virtual int getNumVertices();
	virtual Point* getVertices();
	virtual Color* getColors();
	virtual GLenum getRenderingMode();
	virtual Shape* setVertices(Point vs[]);
	virtual Shape* setColors(Color cs[]);
	virtual Shape* setRenderingMode(GLenum rm);
	virtual Pair<int, int> getBounds();
	virtual void setBounds(int from, int to);

private:
	int numVertices;
	int from, to;
	Point *vertices;
	Color *colors;
	GLenum renderingMode;
	GLuint verticesVBO, colorsVBO;
};

Shape::Shape(int n)
{
	this->numVertices = n;
	this->from = 0;
	this->to = n;
	this->vertices = new Point[n];
	this->colors = new Color[n];

	Point vs[1] = { { 0, 0 } };
	glGenBuffers(1, &verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point), vs, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	Color cs[1] = { { 0, 0, 0, 0} };
	glGenBuffers(1, &colorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Color), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->renderingMode = GL_POLYGON;
}

Shape::Shape(int n, Point vs[], Color cs[], GLenum rm) : Shape(n)
{
	this->setVertices(vs)->setColors(cs)->setRenderingMode(rm);
}

Shape::~Shape()
{
	glDeleteBuffers(1, &verticesVBO);
	glDeleteBuffers(1, &colorsVBO);
	delete vertices;
	delete colors;
}

inline void Shape::draw()
{
	// draws points from "from" to "to"
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glVertexPointer(2, GL_DOUBLE, 0, (char *)NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	glColorPointer(4, GL_DOUBLE, 0, (char *)NULL);
	glDrawArrays(renderingMode, from, to - from);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

inline int Shape::getNumVertices()
{
	return this->numVertices;
}

inline Point * Shape::getVertices()
{
	return this->vertices;
}

inline Color * Shape::getColors()
{
	return this->colors;
}

inline GLenum Shape::getRenderingMode()
{
	return this->renderingMode;
}

inline Shape* Shape::setVertices(Point vs[])
{
	glDeleteBuffers(1, &verticesVBO);
	memcpy(vertices, vs, numVertices * sizeof(Point));
	glGenBuffers(1, &verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Point), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return this;
}

inline Shape* Shape::setColors(Color cs[])
{
	glDeleteBuffers(1, &colorsVBO);
	memcpy(colors, cs, numVertices * sizeof(Color));
	glGenBuffers(1, &colorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Color), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return this;
}

inline Shape* Shape::setRenderingMode(GLenum rm)
{
	renderingMode = rm;
	return this;
}

inline Pair<int, int> Shape::getBounds()
{
	return { this->from, this->to };
}

inline void Shape::setBounds(int from, int to)
{
	this->from = from;
	this->to = to;
}