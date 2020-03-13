#pragma once
#include "Shape.h"

/*
 * A shape with collisions and transformations
 */
class RigidBody
{
public:
	RigidBody(Shape *shape);
	RigidBody(double width, double height);
	RigidBody(Shape *shape, double width, double height);

	virtual ~RigidBody();

	virtual Point getPosition();
	virtual Point getScale();
	virtual double getRotationAngle();
	virtual double getWidth();
	virtual double getHeight();
	virtual void applyMovement(double dx, double dy);
	virtual void setPosition(double px, double py);
	virtual void setScale(double sx, double sy);
	virtual void setRotationAngle(double r);
	virtual bool isColliding(RigidBody *r);
	virtual void onCollision(RigidBody *r);
	virtual void draw();
	virtual void drawExtra();

protected:
	virtual Shape* getShape();

private:
	Shape *shape;
	Point position;
	Point scale;
	double rotationAngle;
	double width, height;
};

RigidBody::RigidBody(Shape *shape) : RigidBody(shape, 0, 0) { }

inline RigidBody::RigidBody(double width, double height) : RigidBody(NULL, width, height) { }

RigidBody::RigidBody(Shape *shape, double width, double height)
{
	this->shape = shape;
	this->position = { 0, 0 };
	this->scale = { 1, 1 };
	this->rotationAngle = 0;
	this->width = width;
	this->height = height;
}

RigidBody::~RigidBody() { }

inline Point RigidBody::getPosition()
{
	return this->position;
}

inline Point RigidBody::getScale()
{
	return this->scale;
}

inline double RigidBody::getRotationAngle()
{
	return this->rotationAngle;
}

inline double RigidBody::getWidth()
{
	return this->width;
}

inline double RigidBody::getHeight()
{
	return this->height;
}

inline void RigidBody::applyMovement(double dx, double dy)
{
	this->position.x += dx;
	this->position.y += dy;
}

inline void RigidBody::setPosition(double px, double py)
{
	this->position = { px, py };
}

inline void RigidBody::setScale(double sx, double sy)
{
	this->scale = { sx, sy };
}

inline void RigidBody::setRotationAngle(double r)
{
	this->rotationAngle = r;
}

inline bool RigidBody::isColliding(RigidBody *r)
{
	return (absv(r->getPosition().x - this->position.x) - Dimension::COLLISION_PADDING <= (r->getWidth() + this->width) / 2.0) &&
			(absv(r->getPosition().y - this->position.y) - Dimension::COLLISION_PADDING <= (r->getHeight() + this->height) / 2.0);
}

inline void RigidBody::onCollision(RigidBody * r) { }

inline void RigidBody::draw()
{
	if (shape == NULL)
		return;

	glPushMatrix();
	glTranslated(position.x, position.y, 0);
	glScaled(scale.x, scale.y, 1);
	glRotated(rotationAngle, 0, 0, 1);
	shape->draw();
	drawExtra();
	glPopMatrix();
}

inline void RigidBody::drawExtra() { }

inline Shape *RigidBody::getShape()
{
	return this->shape;
}
