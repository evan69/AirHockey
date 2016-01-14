#include "puck.h"
#include <stdio.h>

puck::puck(GLdouble _r,GLdouble _h,mallet* _self,mallet* _oppo,GLuint* _flag)
{
	x = 0;
	z = 0;
	r = _r;
	h = _h;
	flag = _flag;
	rand();
	dir_x = (GLdouble)(rand() % 100 - 50.0);
	dir_z = (GLdouble)(rand() % 100 - 50.0);
	GLdouble pr = (GLdouble)sqrt(double(dir_x * dir_x + dir_z * dir_z));
	dir_x = dir_x / pr;
	dir_z = dir_z / pr;
	//初始时，随机生成单位的运动方向向量
	self = _self;
	self->pr = r;
	oppo = _oppo;
	oppo->pr = r;
}


void puck::update(GLdouble step)//处理运动和各种碰撞事件
{
	x += step * dir_x;
	z += step * dir_z;
	if(z > 2.0 - r && x < 0.4 && x > -0.4)
	{
		*flag = -1;
		return;
	}
	if(z < - (2.0 - r) && x < 0.4 && x > -0.4)
	{
		*flag = 1;
		return;
	}
	if(x > 1.0 - r || x < - (1.0 - r)) 
	{
		dir_x = - abs(dir_x) * (x/abs(x));
	}
	if(z > 2.0 - r || z < - (2.0 - r))
	{
		dir_z = - abs(dir_z) * (z/abs(z));
	}
	double l2 = (x - self->x) * (x - self->x) + (z - self->z) * (z - self->z);
	if(l2 == 0.0) l2 = 0.000001;
	if((GLdouble)l2 < (r + self->r) * (r + self->r))
	{
		double theta = acos(((self->x - x) * dir_x + (self->z - z) * dir_z) / sqrt(l2));
		if(theta > 1.57) return;
		double tot_x = 2.00 * (self->z - z) * sin(theta) / sqrt(l2);
		double tot_z = - 2.00 * (self->x - x) * sin(theta) / sqrt(l2);
		dir_x = tot_x - dir_x;
		dir_z = tot_z - dir_z;
		GLdouble pr = (GLdouble)sqrt(double(dir_x * dir_x + dir_z * dir_z));
		dir_x = dir_x / pr;
		dir_z = dir_z / pr;
	}
	self->px = x;
	self->pz = z;

	l2 = (x - oppo->x) * (x - oppo->x) + (z - oppo->z) * (z - oppo->z);
	if(l2 == 0.0) l2 = 0.000001;
	if((GLdouble)l2 < (r + oppo->r) * (r + oppo->r))
	{
		double theta = acos(((oppo->x - x) * dir_x + (oppo->z - z) * dir_z) / sqrt(l2));
		if(theta > 1.57) return;
		double tot_x = 2.00 * (oppo->z - z) * sin(theta) / sqrt(l2);
		double tot_z = - 2.00 * (oppo->x - x) * sin(theta) / sqrt(l2);
		dir_x = tot_x - dir_x;
		dir_z = tot_z - dir_z;
		GLdouble pr = (GLdouble)sqrt(double(dir_x * dir_x + dir_z * dir_z));
		dir_x = dir_x / pr;
		dir_z = dir_z / pr;
	}
	oppo->px = x;
	oppo->pz = z;
}

void puck::show()//绘制这个puck
{
	objCylinder = gluNewQuadric();
	glMatrixMode(GL_MODELVIEW);
	glColor3d(1,0,0);
	glPushMatrix();
		glNormal3d(0, 1, 0);
		glTranslated(x,2.05,z);
		glRotated(90,1,0,0);
		gluCylinder(objCylinder, r, r, 0.1, 30, 5);
		gluDisk(objCylinder,0,r,30,5);
	glPopMatrix();
}