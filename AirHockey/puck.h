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
	GLdouble x;//x坐标
	GLdouble z;//z坐标
	GLdouble dir_x;//x方向速度
	GLdouble dir_z;//z方向速度
	GLdouble r;//半径
	GLdouble h;//高度
	mallet* self;
	mallet* oppo;//两个mallet
	GLUquadricObj *objCylinder;
	char color;//颜色
	GLuint* flag;//允许修改胜利标示
	puck(GLdouble _r,GLdouble _h,mallet*,mallet*,GLuint*);
	void update(GLdouble step);
	void show();
};

#endif