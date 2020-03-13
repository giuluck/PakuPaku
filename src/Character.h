#pragma once
#include "RigidBody.h"

/*
 * Generic Game Character
 */
class Character :
	public RigidBody
{
public:
	static const double DEFAULT_SPEED;

	Character(Shape *s, Point p, double w, double h);
	virtual ~Character();
	virtual void setDirection(Direction d);
	virtual void update();
	virtual bool hasPower();
	virtual void setPower(bool power);
	virtual void onCollision(RigidBody *r);
	virtual void draw();
	virtual void drawExtra();
	virtual void die();

protected:
	virtual unsigned int getFrame();
	virtual double getSpeed();
	virtual void setSpeed(double s);
	virtual void setPowerShape(Shape *shape);
	virtual Shape *getPowerShape();

private:
	unsigned int frame;
	bool dead, power;
	double speed;
	Point movementVector;
	Shape *powerShape;
};

const double Character::DEFAULT_SPEED = 4.0;

Character::Character(Shape *s, Point p, double w, double h) : RigidBody(s, w, h)
{
	this->setPosition(p.x, p.y);
	this->frame = 0;
	this->dead = false;
	this->power = false;
	this->speed = DEFAULT_SPEED;
	this->movementVector = { 0, 0 };
}

Character::~Character() 
{
	delete powerShape;
}

inline void Character::setDirection(Direction d)
{
	switch (d) {
	case Direction::NONE:
		this->movementVector = { 0, 0 };
		break;
	case Direction::LEFT:
		this->movementVector = { -speed, 0 };
		break;
	case Direction::RIGHT:
		this->movementVector = { speed, 0 };
		break;
	case Direction::UP:
		this->movementVector = { 0, speed };
		break;
	case Direction::DOWN:
		this->movementVector = { 0, -speed };
		break;
	}
}

inline void Character::update()
{
	this->frame++;
	this->applyMovement(movementVector.x, movementVector.y);

	// if out of bounds take it back inside the bounds then flip
	if (this->getPosition().x < -World::SEMI_WIDTH || this->getPosition().x > World::SEMI_WIDTH)
		this->applyMovement(-2.0 * World::SEMI_WIDTH * sign(this->getPosition().x), 0.0);
	if (this->getPosition().y < -World::SEMI_HEIGHT || this->getPosition().y > World::SEMI_HEIGHT)
		this->applyMovement(0.0, -2.0 * World::SEMI_HEIGHT * sign(this->getPosition().y));
}

inline bool Character::hasPower()
{
	return this->power;
}

inline void Character::setPower(bool power)
{
	this->power = power;
}

inline void Character::onCollision(RigidBody *r)
{
	Point p = movementVector;
	p.x = (p.x == 0) ? 0 : p.x / absv(p.x);
	p.y = (p.y == 0) ? 0 : p.y / absv(p.y);

	while(r->isColliding(this))
		this->applyMovement(-p.x, -p.y);

	this->setDirection(Direction::NONE);
}

inline void Character::draw()
{
	if (dead)
		return;

	RigidBody::draw();
}

inline void Character::drawExtra()
{
	/*
	 * POWER
	 */
	if (hasPower()) {
		glPushMatrix();
		glScaled(0.75, 0.75, 1);
		glTranslated(-getWidth() / 2.0, -getHeight() / 2.0, 0.0);
		powerShape->draw();
		glPopMatrix();
	}

	RigidBody::drawExtra();
}

inline void Character::die()
{
	this->dead = true;
}

inline unsigned int Character::getFrame()
{
	return this->frame;
}

inline void Character::setSpeed(double s)
{
	this->movementVector.x *= (s / speed);
	this->movementVector.y *= (s / speed);
	this->speed = s;
}

inline double Character::getSpeed()
{
	return this->speed;
}

inline void Character::setPowerShape(Shape *shape)
{
	this->powerShape = shape;
}

inline Shape *Character::getPowerShape()
{
	return this->powerShape;
}
