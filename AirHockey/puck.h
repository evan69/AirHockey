#ifndef PUCK_H
#define PUCK_H
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include "mallet.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class puck
{
public:
	GLdouble x;
	GLdouble y;
	GLdouble dir_x;
	GLdouble dir_y;
	GLdouble r;
	GLdouble h;
	mallet* self;
	mallet* oppo;
	char color;
	puck(GLdouble _r,GLdouble _h);
	void update();
	void show();
};

#endif