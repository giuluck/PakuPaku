#pragma once
#include "RigidBody.h"

/*
 * Edge-Type Obstacle
 */
class Edge :
	public RigidBody
{
public:
	static const int NUM_VERTICES;
	static const double MARGIN;
	static const double PADDING;
	static const double HOLE_SEMI_WIDTH;
	static const double HOLE_SEMI_HEIGHT;
	static const double WIDTH;
	static const double HEIGHT;
	static Shape* GET_SHAPE();

	Edge();
	virtual ~Edge();
	virtual bool isColliding(RigidBody *r);
	virtual void draw();
};

const int Edge::NUM_VERTICES = 6;
const double Edge::MARGIN = World::MARGIN;
const double Edge::PADDING = World::OBSTACLES_PADDING;
const double Edge::HOLE_SEMI_WIDTH = W(0.5);
const double Edge::HOLE_SEMI_HEIGHT = H(0.5);
const double Edge::WIDTH = World::SEMI_WIDTH - MARGIN - HOLE_SEMI_WIDTH;
const double Edge::HEIGHT = World::SEMI_HEIGHT - MARGIN - HOLE_SEMI_HEIGHT;
Shape *Edge::GET_SHAPE()
{
	Point vs[NUM_VERTICES] = { { 0.0, 0.0 }, { 0.0, HEIGHT }, { PADDING, HEIGHT }, { PADDING, PADDING }, { WIDTH, PADDING }, { WIDTH, 0.0 } };
	Color cs[NUM_VERTICES];
	for (int i = 0; i < NUM_VERTICES; i++) {
		vs[i].x += (MARGIN - World::SEMI_WIDTH);
		vs[i].y += (MARGIN - World::SEMI_HEIGHT);
		cs[i] = World::OBSTACLES_COLOR;
	};
	return new Shape(NUM_VERTICES, vs, cs, GL_LINE_LOOP);
}

Edge::Edge() : RigidBody(GET_SHAPE()) { }

Edge::~Edge() { }

inline void Edge::draw()
{
	glPushMatrix();
	RigidBody::draw();
	glScaled(1.0, -1.0, 1.0);
	RigidBody::draw();
	glScaled(-1.0, 1.0, 1.0);
	RigidBody::draw();
	glScaled(1.0, -1.0, 1.0);
	RigidBody::draw();
	glPopMatrix();
}

inline bool Edge::isColliding(RigidBody *r)
{
	Point p = r->getPosition();
	double w = r->getWidth() / 2.0;
	double h = r->getHeight() / 2.0;
	double cp = Dimension::COLLISION_PADDING;
	double edge = MARGIN + PADDING + cp;

	bool centered = absv(p.x) <= absv(HOLE_SEMI_WIDTH - cp - w) || absv(p.y) <= absv(HOLE_SEMI_HEIGHT - cp - h);
	bool left = p.x - w <= -World::SEMI_WIDTH + edge;
	bool right = p.x + w >= World::SEMI_WIDTH - edge;
	bool down = p.y - h <= -World::SEMI_HEIGHT + edge;
	bool up = p.y + h >= World::SEMI_HEIGHT - edge;

	return !centered && (left || right || down || up);
}
