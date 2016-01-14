#ifndef MALLET_H
#define MALLET_H
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include "puck.h"

#include <stdio.h>
#include <stdlib.h>

class puck;

class mallet
{
public:
	GLdouble x;//x����
	GLdouble z;//z����
	GLdouble r;//�뾶
	GLdouble h;//��
	GLdouble px;//puck x����
	GLdouble pz;//puck z����
	GLdouble pr;//puck �뾶
	GLUquadricObj *objCylinder;
	char color;//��ɫ
	mallet(char _c,GLdouble _x,GLdouble _z,GLdouble _r,GLdouble _h);
	void update(GLdouble _x,GLdouble _z);
	void show();
};

class AImallet : public mallet
{
public:
	AImallet(char _c,GLdouble _x,GLdouble _z,GLdouble _r,GLdouble _h,
		puck* _info,GLdouble _maxSpeed = 0.0003);
	puck* info;
	GLdouble maxSpeed;//����ƶ��ٶ�
	void control(GLdouble delta);
	void show();
};

#endif