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
	GLdouble z;
	GLdouble r;
	GLdouble h;
	GLdouble px;
	GLdouble pz;
	GLdouble pr;
	GLUquadricObj *objCylinder;
	char color;
	mallet(char _c,GLdouble _x,GLdouble _z,GLdouble _r,GLdouble _h);
	void update(GLdouble _x,GLdouble _z);
	void show();
};

class AImallet : public mallet
{
public:
	AImallet(char _c,GLdouble _x,GLdouble _z,GLdouble _r,GLdouble _h,GLdouble maxSpeed = 0.1);
	GLdouble maxSpeed;
	void controller();
	void show();
};

#endif