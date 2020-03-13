#pragma once
#include "Character.h"
#include <math.h>

/*
 * Pacman Character
 */
class Pacman :
	public Character
{
public:
	static const int NUM_VERTICES;
	static const double RADIUS;
	static const Point INITIAL_POSITION;
	static const Color EDGE_COLORS[3];
	static const Color CENTER_COLOR;
	static const int ANIMATION_BOUNDS;
	static const int ANIMATION_FRAMES;
	static const double SPEED_MODIFIER;
	static Shape *GET_SHAPE();

	Pacman();
	virtual ~Pacman();
	virtual void setDirection(Direction d);
	virtual void accelerate();
	virtual void decelerate();
	virtual void restoreSpeed();
	virtual void update();
	virtual void die();

private:
	int edgeColor;
	int direction;
	bool dying;
	double rotationAngle;

	virtual void setSpeed(double speed);
};

const int Pacman::NUM_VERTICES = 30;
const double Pacman::RADIUS = Dimension::MAX_CHAR_WIDTH / 2.0;
const Point Pacman::INITIAL_POSITION = AnchorPoints::STARTING;
const Color Pacman::EDGE_COLORS[3] = { { 0, 0, 1, 1 }, { 1, 1, 0.25, 1 }, { 1, 0, 0, 1 } };
const Color Pacman::CENTER_COLOR = { 1, 1, 0.35, 1 };
const int Pacman::ANIMATION_BOUNDS = 4;
const int Pacman::ANIMATION_FRAMES = 3 * Character::DEFAULT_SPEED;
const double Pacman::SPEED_MODIFIER = 1.5;
Shape *Pacman::GET_SHAPE()
{
	const double RADIUS = Dimension::MAX_CHAR_WIDTH / 2.0;
	const double s = (2 * M_PI) / NUM_VERTICES;
	Point vs[3 * NUM_VERTICES];
	Color cs[3 * NUM_VERTICES];

	for (int i = 0; i < NUM_VERTICES; i++) {
		vs[3 * i] = { cos(i * s) * RADIUS, sin(i * s) * RADIUS };
		cs[3 * i] = EDGE_COLORS[1];
		vs[3 * i + 1] = { 0, 0 };
		cs[3 * i + 1] = CENTER_COLOR;
		vs[3 * i + 2] = { cos((i + 1) * s) * RADIUS, sin((i + 1) * s) * RADIUS };
		cs[3 * i + 2] = EDGE_COLORS[1];
	}

	return new Shape(3 * NUM_VERTICES, vs, cs, GL_TRIANGLES);
}

Pacman::Pacman() : Character(GET_SHAPE(), INITIAL_POSITION, 2 * RADIUS, 2 * RADIUS) 
{
	this->edgeColor = 1;
	this->direction = 3;
	this->dying = false;
	this->setPowerShape(PowerUp::GET_SHAPE(FAST));
}

Pacman::~Pacman() { }

inline void Pacman::setDirection(Direction d)
{
	Character::setDirection(d);

	if (dying)
		return;

	if (d != Direction::NONE)
		if(d == Direction::LEFT) {
			this->setRotationAngle(0);
			this->setScale(-1, 1);
		}
		else {
			this->setRotationAngle(d);
			this->setScale(1, 1);
		}
}

inline void Pacman::accelerate()
{
	setSpeed(getSpeed() * SPEED_MODIFIER);
}

inline void Pacman::decelerate()
{
	setSpeed(getSpeed() / SPEED_MODIFIER);
}

inline void Pacman::restoreSpeed()
{
	setSpeed(Character::DEFAULT_SPEED);
}

inline void Pacman::setSpeed(double speed)
{
	int newEdgeColor = (speed > Character::DEFAULT_SPEED) ? 2 : ((speed < Character::DEFAULT_SPEED) ? 0 : 1);

	if (newEdgeColor != edgeColor) {
		Color cs[3 * NUM_VERTICES];
		for (int i = 0; i < NUM_VERTICES; i++) {
			cs[3 * i] = EDGE_COLORS[newEdgeColor];
			cs[3 * i + 1] = CENTER_COLOR;
			cs[3 * i + 2] = EDGE_COLORS[newEdgeColor];
		}
		getShape()->setColors(cs);
		edgeColor = newEdgeColor;
	}

	Character::setSpeed(speed);
}

inline void Pacman::update()
{
	Character::update();

	int from = getShape()->getBounds().first;
	int to = getShape()->getBounds().second;

	if (getFrame() % ((int)(ANIMATION_FRAMES / getSpeed())) == 0) {
		if (!dying && (from == direction * ANIMATION_BOUNDS || (from == 0 && direction < 0)))
			direction = -direction;
		getShape()->setBounds(from + direction, to - direction);
	}

	if (from >= to)
		Character::die();
}

inline void Pacman::die()
{
	this->dying = true;
	this->direction = 3;
	this->setDirection(Direction::NONE);
}
