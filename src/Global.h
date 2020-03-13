#pragma once

/*
 * Container Class for Global Structs and Variables
 */

#include <cmath>
#define _USE_MATH_DEFINES
#define W(i) (i * Dimension::CELL_WIDTH)
#define H(i) (i * Dimension::CELL_HEIGHT)

enum Direction { RIGHT = 0, LEFT = 180, UP = 90, DOWN = -90, NONE };
enum PowerUps { FAST = 0, SLOW = 1, STUN = 2 };
enum Characters { PACMAN = 0, MONSTER = 1 };

struct Point {
	double x = 0, y = 0;
};

struct Color {
	double r = 0, g = 0, b = 0, a = 1;
};

template <class X, class Y> struct Pair
{
	X first;
	Y second;
};

const struct Animation {
	static const int FPS;
};

const struct Dimension {
	static const double MAX_CHAR_WIDTH;
	static const double MAX_CHAR_HEIGHT;
	static const double CELL_WIDTH;
	static const double CELL_HEIGHT;
	static const double LINE_WIDTH;
	static const double COLLISION_PADDING;
};


const struct World {
	static const int SEMI_WIDTH;
	static const int SEMI_HEIGHT;
	static const double MARGIN;
	static const double OBSTACLES_PADDING;
	static const Color OBSTACLES_COLOR;
	static const Color BACKGROUND_COLOR;
};

const struct Window {
	static const char TITLE[10];
	static const int POSITION_X;
	static const int POSITION_Y;
	static int semiWidth;
	static int semiHeight;
};

const struct AnchorPoints {
	static const double PADDING;
	static const Point UP_LEFT;
	static const Point UP_RIGHT;
	static const Point DOWN_LEFT;
	static const Point DOWN_RIGHT;
	static const Point STARTING;
};

const struct Times {
	static const unsigned int GAMEOVER_MSECS;
	static const unsigned int STUN_MSECS;
	static const unsigned int POWER_MSECS;
	static const unsigned int STUN_RETURN_MSECS;
	static const unsigned int POWER_RETURN_MSECS;
};

const int Animation::FPS = 60;
const double Dimension::MAX_CHAR_WIDTH = 40.0;
const double Dimension::MAX_CHAR_HEIGHT = 45.0;
const double Dimension::LINE_WIDTH = 2.0;
const double Dimension::CELL_WIDTH = 72.0;
const double Dimension::CELL_HEIGHT = 72.0;
const double Dimension::COLLISION_PADDING = Dimension::LINE_WIDTH / 2.0;
const int World::SEMI_WIDTH = (int) (7.5 * Dimension::CELL_WIDTH + World::MARGIN);
const int World::SEMI_HEIGHT = (int) (4.0 * Dimension::CELL_WIDTH + World::MARGIN);
const double World::MARGIN = 5.0;
const double World::OBSTACLES_PADDING = 5.0;
const Color World::OBSTACLES_COLOR = { 0.0, 0.0, 1.0, 1.0 };
const Color World::BACKGROUND_COLOR = { 0.0, 0.0, 0.0, 1.0 };
const char Window::TITLE[10] = "Paku Paku";
const int Window::POSITION_X = 0;
const int Window::POSITION_Y = 0;
int Window::semiWidth = World::SEMI_WIDTH;
int Window::semiHeight = World::SEMI_HEIGHT;
const double AnchorPoints::PADDING = 30.0;
const Point AnchorPoints::UP_LEFT = { -World::SEMI_WIDTH + PADDING, World::SEMI_HEIGHT - PADDING };
const Point AnchorPoints::UP_RIGHT = { World::SEMI_WIDTH - PADDING, World::SEMI_HEIGHT - PADDING };
const Point AnchorPoints::DOWN_LEFT = { -World::SEMI_WIDTH + PADDING, -World::SEMI_HEIGHT + PADDING };
const Point AnchorPoints::DOWN_RIGHT = { World::SEMI_WIDTH - PADDING, -World::SEMI_HEIGHT + PADDING };
const Point AnchorPoints::STARTING = { 0, H(-1.5) };
const unsigned int Times::GAMEOVER_MSECS = 2500;
const unsigned int Times::STUN_MSECS = 10000;
const unsigned int Times::POWER_MSECS = 5000;
const unsigned int Times::STUN_RETURN_MSECS = 7000;
const unsigned int Times::POWER_RETURN_MSECS = 5000;

double absv(double x)
{
	return x > 0 ? x : -x;
}

double sign(double x)
{
	return x == 0 ? 0 : x / absv(x);
}