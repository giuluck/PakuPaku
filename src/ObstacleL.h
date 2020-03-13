#pragma once
#include "ObstacleLine.h"

class ObstacleL :
	public RigidBody
{
public:
	static const int NUM_VERTICES;
	static const double PADDING;
	static Shape* GET_SHAPE(double w, double h);

	ObstacleL(Point p, double w, double h);
	virtual ~ObstacleL();
	virtual bool isColliding(RigidBody *r);

private:
	RigidBody *sides[2];
};

const int ObstacleL::NUM_VERTICES = 6;
const double ObstacleL::PADDING = World::OBSTACLES_PADDING;
Shape* ObstacleL::GET_SHAPE(double w, double h) {
	Point p = { sign(w) * PADDING, sign(h) * PADDING };

	Point vs[NUM_VERTICES] = { { 0, 0 }, { w, 0 }, { w, p.y }, { p.x, p.y }, { p.x, h }, { 0, h } };
	Color cs[NUM_VERTICES] = { World::OBSTACLES_COLOR, World::OBSTACLES_COLOR, World::OBSTACLES_COLOR,
							   World::OBSTACLES_COLOR, World::OBSTACLES_COLOR, World::OBSTACLES_COLOR };

	return new Shape(NUM_VERTICES, vs, cs, GL_LINE_LOOP);
}

ObstacleL::ObstacleL(Point p, double w, double h) : RigidBody(GET_SHAPE(w, h), absv(w), absv(h))
{
	sides[HORIZONTAL] = new RigidBody(getWidth(), PADDING);
	sides[VERTICAL] = new RigidBody(PADDING, getHeight());

	this->setPosition(p.x, p.y);
	sides[HORIZONTAL]->setPosition(p.x + w / 2.0, p.y + sign(h) * PADDING / 2.0);
	sides[VERTICAL]->setPosition(p.x + sign(w) * PADDING / 2.0, p.y + h / 2.0);
}

ObstacleL::~ObstacleL() 
{
	delete sides[HORIZONTAL];
	delete sides[VERTICAL];
}

inline bool ObstacleL::isColliding(RigidBody *r)
{
	return sides[HORIZONTAL]->isColliding(r) || sides[VERTICAL]->isColliding(r);
}