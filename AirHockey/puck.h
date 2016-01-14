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
	GLdouble x;//x����
	GLdouble z;//z����
	GLdouble dir_x;//x�����ٶ�
	GLdouble dir_z;//z�����ٶ�
	GLdouble r;//�뾶
	GLdouble h;//�߶�
	mallet* self;
	mallet* oppo;//����mallet
	GLUquadricObj *objCylinder;
	char color;//��ɫ
	GLuint* flag;//�����޸�ʤ����ʾ
	puck(GLdouble _r,GLdouble _h,mallet*,mallet*,GLuint*);
	void update(GLdouble step);
	void show();
};

#endif