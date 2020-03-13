#pragma once
#include "Character.h"
#include "Trian.h"
#include "Rect.h"
#include "Ellip.h"
#include "PowerUp.h"
#include <math.h>

/*
 * Monster Character
 */
class Monster :
	public Character
{
public:
	static const int NUM_VERTICES;
	static const double POLY_SIDE;
	static const double EYE_RAY;
	static const double WIDTH;
	static const double HEIGHT;
	static const Point INITIAL_POSITION;
	static const Color DEFAULT_COLOR;
	static const Color STUNNED_COLOR;
	static const int NUM_ANIMATIONS;
	static const int ANIMATION_FRAMES;
	static Shape* GET_SHAPE();

	Monster();
	~Monster();
	void update();
	void stun();
	void unstun();
	bool isStunned();

private:
	int animation;
	bool stunned;
	Shape *animationTri, *animationQuad, *eye, *pupil;
	Shape *animationPolys[4];

	virtual void drawExtra();
};

const int Monster::NUM_VERTICES = 17;
const double Monster::POLY_SIDE = 7.0;
const double Monster::EYE_RAY = 5.0;
const double Monster::WIDTH = Dimension::MAX_CHAR_WIDTH;
const double Monster::HEIGHT = Dimension::MAX_CHAR_HEIGHT;
const Point Monster::INITIAL_POSITION = { 0.0, 0.0 };
const Color Monster::DEFAULT_COLOR = { 1, 0, 0, 1 };
const Color Monster::STUNNED_COLOR = { 0, 0, 1, 1 };
const int Monster::NUM_ANIMATIONS = 2;
const int Monster::ANIMATION_FRAMES = 10;
Shape* Monster::GET_SHAPE()
{
	const double s = M_PI / (NUM_VERTICES - 3);
	const double w = WIDTH / 2.0;
	const double h = HEIGHT / 2.0;
	double dh = (HEIGHT - WIDTH) / 2.0;
	Point vs[NUM_VERTICES];
	Color cs[NUM_VERTICES];
	
	vs[0] = { w, -h };
	cs[0] = DEFAULT_COLOR;
	for (int i = 0; i < NUM_VERTICES - 2; i++) {
		vs[i + 1] = { w * cos(i * s), w * sin(i * s) + dh };
		cs[i + 1] = DEFAULT_COLOR;
	}
	vs[NUM_VERTICES - 1] = { -w, -h };
	cs[NUM_VERTICES - 1] = DEFAULT_COLOR;

	return new Shape(NUM_VERTICES, vs, cs, GL_POLYGON);
}

Monster::Monster() : Character(GET_SHAPE(), INITIAL_POSITION, WIDTH, HEIGHT)
{
	this->animation = 1;
	this->stunned = false;

	Point tv[3] = { {-POLY_SIDE, -POLY_SIDE / 2.0}, {0.0, POLY_SIDE / 2.0}, {POLY_SIDE, -POLY_SIDE / 2.0} };
	Color cv[3] = { World::BACKGROUND_COLOR, World::BACKGROUND_COLOR , World::BACKGROUND_COLOR };
	this->animationTri = new Shape(3, tv, cv, GL_POLYGON);
	this->animationQuad = new Rect(POLY_SIDE);
	this->eye = new Ellip(EYE_RAY, { 1.0, 1.0, 1.0, 0.8 });
	this->pupil = new Ellip(EYE_RAY , { 0.3, 0.3, 1.0, 1.0 }, { 0.7, 0.7, 1.0, 1.0 });
	this->animationPolys[0] = this->animationQuad;
	this->animationPolys[1] = this->animationTri;
	this->animationPolys[2] = this->animationQuad;
	this->animationPolys[3] = this->animationTri;
	this->setPowerShape(PowerUp::GET_SHAPE(SLOW));
}

Monster::~Monster()
{
	delete this->animationTri;
	delete this->animationQuad;
	delete this->eye;
	delete this->pupil;
}

inline void Monster::update()
{
	Character::update();
	if (getFrame() % ANIMATION_FRAMES == 0)
		animation = animation % NUM_ANIMATIONS + 1;
}

inline void Monster::stun()
{
	Color cs[NUM_VERTICES];
	for (int i = 0; i < NUM_VERTICES; i++)
		cs[i] = STUNNED_COLOR;

	getShape()->setColors(cs);
	stunned = true;
}

inline void Monster::unstun()
{
	Color cs[NUM_VERTICES];
	for (int i = 0; i < NUM_VERTICES; i++)
		cs[i] = DEFAULT_COLOR;

	getShape()->setColors(cs);
	stunned = false;
}

inline bool Monster::isStunned()
{
	return this->stunned;
}

inline void Monster::drawExtra()
{
	double pupil_factor = 0.5 + 0.3 * sin(M_PI * (getFrame() % 60) / 30);

	/*
	 * ANIMATION
	 */
	for (int i = -1; i <= 1; i++) {
		glPushMatrix();
		glTranslated(i * (POLY_SIDE - WIDTH / 2.0), (POLY_SIDE - HEIGHT) / 2.0, 0.0);
		animationPolys[i + animation]->draw();
		glPopMatrix();
	}

	/*
	 * EYES
	 */
	for (int i = -1; i <= 1; i += 2) {
		glPushMatrix();
		glTranslated(i * WIDTH / 4.0, HEIGHT / 5.0, 0.0);
		eye->draw();
		if (!stunned) {
			glPushMatrix();
			glScaled(pupil_factor, pupil_factor, 1.0);
			pupil->draw();
			glPopMatrix();
		}
		glPopMatrix();
	}

	Character::drawExtra();
}