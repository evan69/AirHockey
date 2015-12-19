#ifndef MALLET_H
#define MALLET_H
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include <stdio.h>
#include <stdlib.h>

class mallet
{
public:
	GLdouble x;
	GLdouble y;
	GLdouble r;
	GLdouble h;
	char color;
	mallet(char _c,GLdouble _x,GLdouble _y,GLdouble _r,GLdouble _h);
	void update(GLdouble _x,GLdouble _y);
	void show();
};

#endif