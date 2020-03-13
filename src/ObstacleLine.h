#pragma once
#include "ObstacleRect.h"

enum Orientation { HORIZONTAL = 0, VERTICAL = 1 };

class ObstacleLine :
	public ObstacleRect
{
public:
	static const double PADDING;

	ObstacleLine(Point p, Orientation o, double l);
	virtual ~ObstacleLine();
};

const double ObstacleLine::PADDING = World::OBSTACLES_PADDING;

ObstacleLine::ObstacleLine(Point p, Orientation o, double l) : 
	ObstacleRect(p, o == Orientation::HORIZONTAL ? l : PADDING, o == Orientation::HORIZONTAL ? PADDING : l) { }

ObstacleLine::~ObstacleLine() { }
