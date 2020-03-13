#pragma once
#include "RigidBody.h"

/*
 * Container-Type Obstacle
 */
class Container :
	public RigidBody
{
public:
	static const int NUM_VERTICES;
	static const double PADDING;
	static const double WIDTH;
	static const double HEIGHT;
	static const double UPPER_WIDTH;
	static Shape* GET_SHAPE();

	Container();
	virtual ~Container();
	virtual bool isColliding(RigidBody *r);
};

const int Container::NUM_VERTICES = 12;
const double Container::PADDING = World::OBSTACLES_PADDING;
const double Container::WIDTH = W(2.0);
const double Container::HEIGHT = H(1.5);
const double Container::UPPER_WIDTH = W(0.5);
Shape* Container::GET_SHAPE()
{
	Point vs[NUM_VERTICES];
	Color cs[NUM_VERTICES];

	vs[0] = { 0.0, 0.0 };
	vs[1] = { 0.0, HEIGHT };
	vs[2] = { UPPER_WIDTH, HEIGHT };
	vs[3] = { UPPER_WIDTH, HEIGHT - PADDING };
	vs[4] = { PADDING, HEIGHT - PADDING };
	vs[5] = { PADDING, PADDING };
	vs[6] = { WIDTH - PADDING, PADDING };
	vs[7] = { WIDTH - PADDING, HEIGHT - PADDING };
	vs[8] = { WIDTH - UPPER_WIDTH, HEIGHT - PADDING };
	vs[9] = { WIDTH - UPPER_WIDTH, HEIGHT };
	vs[10] = { WIDTH, HEIGHT };
	vs[11] = { WIDTH, 0.0 };
	for (int i = 0; i < NUM_VERTICES; i++) {
		vs[i].x -= Container::WIDTH / 2.0;
		vs[i].y -= Container::HEIGHT / 2.0;
		cs[i] = World::OBSTACLES_COLOR;
	};
	
	return new Shape(NUM_VERTICES, vs, cs, GL_LINE_LOOP);
}

Container::Container() : RigidBody(GET_SHAPE(), WIDTH + Dimension::LINE_WIDTH, HEIGHT + Dimension::LINE_WIDTH) { }

Container::~Container() { }

inline bool Container::isColliding(RigidBody * r)
{
	Point rp = r->getPosition();
	Point tp = this->getPosition();
	double rw = r->getWidth() / 2.0, rh = r->getHeight() / 2.0;
	double tw = WIDTH / 2.0 + Dimension::COLLISION_PADDING, th = HEIGHT / 2.0 + Dimension::COLLISION_PADDING;
	double margin = WIDTH / 2.0 - UPPER_WIDTH - Dimension::COLLISION_PADDING;

	bool centered = absv(rp.x) <= absv(margin - rw);
	bool left;
	bool right;
	bool down;
	bool up;
	
	// if the rigid body is inside the container it can exit just if centered and going up
	if (absv(rp.x) <= absv(tw + rw) && absv(rp.y) <= absv(th + rh)) {
		tw -= (PADDING + 2.0 * Dimension::COLLISION_PADDING);
		th -= (PADDING + 2.0 * Dimension::COLLISION_PADDING);
		left = rp.x - rw <= -tw;
		right = rp.x + rw >= tw;
		down = rp.y - rh <= -th;
		up = rp.y + rh >= th;
		
		return left || right || down || (up && !centered);
	}
	// oherwise it can enter only if it's centered and over the container
	else {
		up = rp.y - rh >= th - Dimension::COLLISION_PADDING;

		return !(centered && up) && RigidBody::isColliding(r);
	}
}


