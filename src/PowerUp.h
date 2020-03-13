#pragma once
#include "RigidBody.h"
#include "Ellip.h"
#include "Arrow.h"
#define PARAMS DIMENSIONS[type].first, DIMENSIONS[type].second, COLORS[type].first, COLORS[type].second

#include <iostream>
using namespace std;

/*
 * Power Ups elements in the game
 */
class PowerUp :
	public RigidBody
{
public:
	static const Pair<double, double> DIMENSIONS[3];
	static const Pair<Color, Color> COLORS[3];
	static Shape *GET_SHAPE(PowerUps type);
	
	PowerUp(PowerUps type, Point position);
	virtual ~PowerUp();
	virtual PowerUps getType();
	virtual void show();
	virtual void hide();
	virtual bool isColliding(RigidBody *r);
	virtual void draw();

private:
	PowerUps type;
	bool hidden;
};

const Pair<double, double> PowerUp::DIMENSIONS[3] = { { Dimension::MAX_CHAR_WIDTH / 1.3, Dimension::MAX_CHAR_WIDTH / 2.0 },
													  { Dimension::MAX_CHAR_WIDTH / 1.3, Dimension::MAX_CHAR_WIDTH / 2.0 },
													  { Dimension::MAX_CHAR_WIDTH / 3.0, Dimension::MAX_CHAR_WIDTH / 3.0 } };
const Pair<Color, Color> PowerUp::COLORS[3] = { { { 1.0, 1.0, 1.0, 1.0 }, { 1.0, 0.0, 0.0, 0.6 } },
												{ { 1.0, 1.0, 1.0, 1.0 }, { 0.0, 0.0, 1.0, 0.6 } },
												{ { 0.9, 0.9, 0.2, 1.0 }, { 0.9, 0.9, 0.7, 0.7 } } };
Shape *PowerUp::GET_SHAPE(PowerUps type) {
	switch (type) {
	case PowerUps::FAST:
		return new Arrow(Direction::RIGHT, PARAMS);
	case PowerUps::SLOW:
		return new Arrow(Direction::LEFT, PARAMS);
	case PowerUps::STUN:
		return new Ellip(PARAMS);
	default:
		return NULL;
	}
}

PowerUp::PowerUp(PowerUps type, Point position) : RigidBody(GET_SHAPE(type), DIMENSIONS[type].first, DIMENSIONS[type].second)
{
	this->type = type;
	this->hidden = true;
	this->setPosition(position.x, position.y);
}

PowerUp::~PowerUp() { }

inline PowerUps PowerUp::getType()
{
	return this->type;
}

inline void PowerUp::show()
{
	this->hidden = false;
}

inline void PowerUp::hide()
{
	this->hidden = true;
}

inline bool PowerUp::isColliding(RigidBody * r)
{
	return !hidden && RigidBody::isColliding(r);
}

inline void PowerUp::draw()
{
	if (hidden)
		return;

	RigidBody::draw();
}
