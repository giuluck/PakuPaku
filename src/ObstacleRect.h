#pragma once
#include "RigidBody.h"
#include "Rect.h"

class ObstacleRect :
	public RigidBody
{
public:
	ObstacleRect(Point p, double w, double h);
	virtual ~ObstacleRect();
};

ObstacleRect::ObstacleRect(Point p, double w, double h) : RigidBody(new Rect(w, h, World::OBSTACLES_COLOR), w, h)
{
	((Rect *)this->getShape())->setFillStatus(false);
	this->setPosition(p.x, p.y);
}

ObstacleRect::~ObstacleRect() { }
