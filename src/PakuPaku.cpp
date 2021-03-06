// PakuPaku.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.

#include "pch.h"
#include "Global.h"
#include "Shape.h"
#include "RigidBody.h"
#include "Edge.h"
#include "Container.h"
#include "ObstacleRect.h"
#include "ObstacleLine.h"
#include "ObstacleL.h"
#include "Cliff.h"
#include "PowerUp.h"
#include "Character.h"
#include "Pacman.h"
#include "Monster.h"
#include <ctime>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
using namespace std;

enum Messages { START = 0, OVER = 1 };
enum State { STARTING, RUNNING, ENDING };

struct E {
	static const int NUM_OBSTACLES = 20, NUM_POWERUPS = 6, NUM_CHARACTERS = 2;
	RigidBody *obstacles[NUM_OBSTACLES];
	PowerUp *powerUps[NUM_POWERUPS];
	Character *characters[NUM_CHARACTERS];
} elements;

struct S {
	const Point TEXT_POSITIONS[2] = { { -86.0, -H(2.7) }, { -49.0, -H(2.7) } };
	const string TEXTS[2] = { "Press Space to Start", "Game Over" };
	Messages m = START;
	State s = STARTING;
	int match = 0;
	int stunTimer = 0;
	bool modifiedPacman = false;
} state;

void bitmap_output(double x, double y, string s, void *font)
{
	glRasterPos3f((GLfloat)x, (GLfloat)y, 0);
	for (int i = 0; i < s.length(); i++)
		glutBitmapCharacter(font, s[i]);
}

void stunReturn(int i)
{
	if(i == state.match)
		elements.powerUps[rand() % 4]->show();
}

void arrowReturn(int i)
{
	if (i == state.match)
		elements.powerUps[rand() % 2 + 4]->show();
}

void stunCountdown(int i)
{
	if (i == state.match) {
		state.stunTimer--;
		if (state.stunTimer <= 0) {
			((Monster *)elements.characters[MONSTER])->unstun();
			glutTimerFunc(Times::STUN_RETURN_MSECS, stunReturn, i);
		}
		else {
			glutTimerFunc(1000, stunCountdown, i);
		}
	}
}

void recallPacman(int i)
{
	if (i == state.match) {
		((Pacman *)elements.characters[PACMAN])->restoreSpeed();
		state.modifiedPacman = false;
	}
}

void throwPower(Characters character)
{
	if (elements.characters[character]->hasPower()) {
		elements.characters[character]->setPower(false);
		
		if(character == PACMAN)
			((Pacman *)elements.characters[PACMAN])->accelerate();
		else
			((Pacman *)elements.characters[PACMAN])->decelerate();

		if (!state.modifiedPacman) {
			glutTimerFunc(Times::POWER_MSECS, recallPacman, state.match);
			state.modifiedPacman = true;
		}
	}
}

void init(void) {
	elements.obstacles[0] = new Edge();
	elements.obstacles[1] = new Container();
	elements.obstacles[2] = new ObstacleRect({ W(-2.5), 0.0 }, W(1.0), H(2.0));
	elements.obstacles[3] = new ObstacleRect({ W(2.5), 0.0 }, W(1.0), H(2.0));
	elements.obstacles[4] = new ObstacleRect({ W(-5.75), H(-1.25) }, W(1.5), H(1.5));
	elements.obstacles[5] = new ObstacleRect({ W(5.75), H(-1.25) }, W(1.5), H(1.5));
	elements.obstacles[6] = new ObstacleRect({ W(-5.75), H(1.75) }, W(1.5), H(2.5));
	elements.obstacles[7] = new ObstacleRect({ W(5.75), H(1.75) }, W(1.5), H(2.5));
	elements.obstacles[8] = new ObstacleLine({ 0.0, H(-2.0) + World::OBSTACLES_PADDING / 2.0 }, Orientation::HORIZONTAL, W(3.0));
	elements.obstacles[9] = new ObstacleLine({ W(-1.5), H(-3.0) - World::OBSTACLES_PADDING / 2.0 }, Orientation::HORIZONTAL, W(2.0));
	elements.obstacles[10] = new ObstacleLine({ W(1.5), H(-3.0) - World::OBSTACLES_PADDING / 2.0 }, Orientation::HORIZONTAL, W(2.0));
	elements.obstacles[11] = new ObstacleLine({ 0.0, H(2.5) }, Orientation::VERTICAL, H(1) + 2 * World::OBSTACLES_PADDING);
	elements.obstacles[12] = new ObstacleL({ W(-4.0), H(2.0) }, W(3.0), H(-1.5));
	elements.obstacles[13] = new ObstacleL({ W(4.0), H(2.0) }, W(-3.0), H(-1.5));
	elements.obstacles[14] = new ObstacleL({ W(-4.0), H(-2.0) }, W(1.5), H(1.5));
	elements.obstacles[15] = new ObstacleL({ W(4.0), H(-2.0) }, W(-1.5), H(1.5));
	elements.obstacles[16] = new Cliff({ W(-2.5), H(3.5) - World::OBSTACLES_PADDING / 2.0 }, W(3), H(1) - World::OBSTACLES_PADDING, Direction::UP);
	elements.obstacles[17] = new Cliff({ W(2.5), H(3.5) - World::OBSTACLES_PADDING / 2.0 }, W(3), H(1) - World::OBSTACLES_PADDING, Direction::UP);
	elements.obstacles[18] = new Cliff({ W(-4.5), H(-3.5) + World::OBSTACLES_PADDING / 2.0 }, W(2), H(1) - World::OBSTACLES_PADDING, Direction::DOWN);
	elements.obstacles[19] = new Cliff({ W(4.5), H(-3.5) + World::OBSTACLES_PADDING / 2.0 }, W(2), H(1) - World::OBSTACLES_PADDING, Direction::DOWN);
	elements.powerUps[0] = new PowerUp(PowerUps::STUN, AnchorPoints::UP_LEFT);
	elements.powerUps[1] = new PowerUp(PowerUps::STUN, AnchorPoints::UP_RIGHT);
	elements.powerUps[2] = new PowerUp(PowerUps::STUN, AnchorPoints::DOWN_LEFT);
	elements.powerUps[3] = new PowerUp(PowerUps::STUN, AnchorPoints::DOWN_RIGHT);
	elements.powerUps[4] = new PowerUp(PowerUps::FAST, AnchorPoints::STARTING);
	elements.powerUps[5] = new PowerUp(PowerUps::SLOW, AnchorPoints::STARTING);
	elements.characters[Characters::MONSTER] = new Monster();
	elements.characters[Characters::PACMAN] = new Pacman();
	glLineWidth((GLfloat)Dimension::LINE_WIDTH);
}

void free(void) {
	for (int i = 0; i < elements.NUM_OBSTACLES; i++)
		delete elements.obstacles[i];
	for (int i = 0; i < elements.NUM_POWERUPS; i++)
		delete elements.powerUps[i];
	for (int i = 0; i < elements.NUM_CHARACTERS; i++)
		delete elements.characters[i];
}

void restart(int i)
{
	state.s = State::STARTING;
	state.m = Messages::START;
	state.stunTimer = 0;
	state.match++;
	state.modifiedPacman = false;
	free();
	init();
}

void end(void)
{
	state.s = State::ENDING;
	state.m = Messages::OVER;
	glutTimerFunc(Times::GAMEOVER_MSECS, restart, state.match);
}

void drawScene(void) {
	glClearColor((GLclampf)World::BACKGROUND_COLOR.r, (GLclampf)World::BACKGROUND_COLOR.g,
				 (GLclampf)World::BACKGROUND_COLOR.b, (GLclampf)World::BACKGROUND_COLOR.a);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glPushMatrix();

	double resizingScale = (World::SEMI_WIDTH * Window::semiHeight > World::SEMI_HEIGHT * Window::semiWidth) ?
		(double)Window::semiWidth / World::SEMI_WIDTH : (double)Window::semiHeight / World::SEMI_HEIGHT;
	glTranslated(World::SEMI_WIDTH, World::SEMI_HEIGHT, 0);
	glScaled(resizingScale * World::SEMI_WIDTH / Window::semiWidth, resizingScale * World::SEMI_HEIGHT / Window::semiHeight, 1.0);

	for (int i = 0; i < elements.NUM_OBSTACLES; i++)
		elements.obstacles[i]->draw();
	for (int i = 0; i < elements.NUM_POWERUPS; i++)
		elements.powerUps[i]->draw();
	for (int i = 0; i < elements.NUM_CHARACTERS; i++)
		elements.characters[i]->draw();

	if(state.s != State::RUNNING)
		bitmap_output(state.TEXT_POSITIONS[state.m].x, state.TEXT_POSITIONS[state.m].y, state.TEXTS[state.m], GLUT_BITMAP_HELVETICA_18);
	else if(state.stunTimer > 0)
		bitmap_output(-5.0, -10.0, to_string(state.stunTimer), GLUT_BITMAP_HELVETICA_18);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glutSwapBuffers();
}

void update(int i) {
	for (int i = 0; i < elements.NUM_CHARACTERS; i++)
		elements.characters[i]->update();

	switch(state.s) {
	case State::RUNNING:
		for (int i = 0; i < elements.NUM_CHARACTERS; i++) {
			for(int j = 0; j < elements.NUM_OBSTACLES; j++)
				if (elements.obstacles[j]->isColliding(elements.characters[i]))
					elements.characters[i]->onCollision(elements.obstacles[j]);
			
			for (int j = 0; j < elements.NUM_POWERUPS; j++)
				if (elements.powerUps[j]->isColliding(elements.characters[i])) {
					elements.powerUps[j]->hide();
					switch (elements.powerUps[j]->getType()) {
					case PowerUps::FAST:
						glutTimerFunc(Times::POWER_RETURN_MSECS, arrowReturn, state.match);
						if (i == PACMAN)
							elements.characters[PACMAN]->setPower(true);
						break;
					case PowerUps::SLOW:
						glutTimerFunc(Times::POWER_RETURN_MSECS, arrowReturn, state.match);
						if (i == MONSTER)
							elements.characters[MONSTER]->setPower(true);
						break;
					case PowerUps::STUN:
						if (i == PACMAN) {
							((Monster *)elements.characters[MONSTER])->stun();
							state.stunTimer = Times::STUN_MSECS / 1000;
							glutTimerFunc(Times::STUN_MSECS % 1000, stunCountdown, state.match);
						}
						else {
							glutTimerFunc(Times::STUN_RETURN_MSECS, stunReturn, state.match);
						}
						break;
					default:
						break;
					}
				}
		}

		if (elements.characters[PACMAN]->isColliding(elements.characters[MONSTER])) {
			if(((Monster *)elements.characters[MONSTER])->isStunned()) {
				elements.characters[MONSTER]->die();
				elements.characters[PACMAN]->setDirection(Direction::NONE);
			}
			else {
				elements.characters[PACMAN]->die();
				elements.characters[MONSTER]->setDirection(Direction::NONE);
			}
			end();
		}
		break;
	case State::ENDING:
		break;
	}

	glutPostRedisplay();
	if(state.s != State::STARTING)
		glutTimerFunc(1000 / Animation::FPS, update, state.match);
}

void resize(int w, int h) {

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 2.0 * World::SEMI_WIDTH, 0.0, 2.0 * World::SEMI_HEIGHT, -1.0, 1.0);
	Window::semiWidth = w / 2;
	Window::semiHeight = h / 2;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyPressed(unsigned char key, int x, int y) {
	if(key == 27) {
		free();
		exit(0);
	}

	switch (state.s) {
	case State::STARTING:
		if (key == ' ') {
			state.s = State::RUNNING;
			stunReturn(state.match);
			glutTimerFunc(15000, arrowReturn, state.match);
			glutTimerFunc(1000 / Animation::FPS, update, state.match);
		}
		break;
	case State::RUNNING:
		switch (key)
		{
		case 'W':
		case 'w':
			elements.characters[MONSTER]->setDirection(Direction::UP);
			break;
		case 'A':
		case 'a':
			elements.characters[MONSTER]->setDirection(Direction::LEFT);
			break;
		case 'S':
		case 's':
			elements.characters[MONSTER]->setDirection(Direction::DOWN);
			break;
		case 'D':
		case 'd':
			elements.characters[MONSTER]->setDirection(Direction::RIGHT);
			break;
		case 'Q':
		case 'q':
			throwPower(MONSTER);
			break;
		case 'I':
		case 'i':
			elements.characters[PACMAN]->setDirection(Direction::UP);
			break;
		case 'J':
		case 'j':
			elements.characters[PACMAN]->setDirection(Direction::LEFT);
			break;
		case 'K':
		case 'k':
			elements.characters[PACMAN]->setDirection(Direction::DOWN);
			break;
		case 'L':
		case 'l':
			elements.characters[PACMAN]->setDirection(Direction::RIGHT);
			break;
		case 'O':
		case 'o':
			throwPower(PACMAN);
			break;
		default:
			break;
		}
	}
}

int main(int argc, char **argv)
{
	srand((unsigned int)time(0));

	GLboolean GlewInitResult;
	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(2 * Window::semiWidth, 2 * Window::semiHeight);
	glutInitWindowPosition(Window::POSITION_X, Window::POSITION_Y);
	glutCreateWindow(Window::TITLE);

	GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult) {
		fprintf(stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyPressed);

	init();
	glutMainLoop();

	return 0;
}