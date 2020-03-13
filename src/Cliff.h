#pragma once
#include "RigidBody.h"

class Cliff :
	public RigidBody
{
public:
	static const int NUM_VERTICES;
	static const double PADDING;
	static Shape *GET_SHAPE(double w, double h);

	Cliff(Point p, double w, double h, Direction d);
	virtual ~Cliff();
};

const int Cliff::NUM_VERTICES = 6;
const double Cliff::PADDING = World::OBSTACLES_PADDING;
Shape *Cliff::GET_SHAPE(double w, double h)
{
	w /= 2.0;
	h /= 2.0;
	Point vs[NUM_VERTICES] = { { -w, h }, { -w, h }, { -w, -h }, { w, -h }, { w, h }, { w, h } };
	Color cs[NUM_VERTICES] = { World::BACKGROUND_COLOR, World::OBSTACLES_COLOR, World::OBSTACLES_COLOR,
							   World::OBSTACLES_COLOR, World::OBSTACLES_COLOR, World::BACKGROUND_COLOR };

	return new Shape(NUM_VERTICES, vs, cs, GL_LINE_LOOP);
}

Cliff::Cliff(Point p, double w, double h, Direction d) : RigidBody(GET_SHAPE(w, h), w, h)
{
	this->setPosition(p.x, p.y);
	this->setRotationAngle(d - 90.0);
}

Cliff::~Cliff() { }
