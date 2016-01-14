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
	GLdouble x;//x坐标
	GLdouble z;//z坐标
	GLdouble r;//半径
	GLdouble h;//高
	GLdouble px;//puck x坐标
	GLdouble pz;//puck z坐标
	GLdouble pr;//puck 半径
	GLUquadricObj *objCylinder;
	char color;//颜色
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
	GLdouble maxSpeed;//最大移动速度
	void control(GLdouble delta);
	void show();
};

#endif