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

class mallet;

class puck
{
public:
	GLdouble x;
	GLdouble z;
	GLdouble dir_x;
	GLdouble dir_z;
	GLdouble r;
	GLdouble h;
	mallet* self;
	mallet* oppo;
	GLUquadricObj *objCylinder;
	char color;
	GLuint* flag;
	puck(GLdouble _r,GLdouble _h,mallet*,mallet*,GLuint*);
	void update(GLdouble step);
	void show();
};

#endif