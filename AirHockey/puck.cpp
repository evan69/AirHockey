#include "puck.h"

puck::puck(GLdouble _r,GLdouble _h,mallet* _self)
{
	x = 0;
	z = 1;
	r = _r;
	h = _h;
	rand();
	dir_x = (GLdouble)rand() / 100.0;
	dir_z = (GLdouble)rand() / 100.0;
	GLdouble pr = (GLdouble)sqrt(double(dir_x * dir_x + dir_z * dir_z));
	dir_x = dir_x / pr;
	dir_z = dir_z / pr;
	self = _self;
	self->pr = r;
}

void puck::update(GLdouble step)
{
	x += step * dir_x;
	z += step * dir_z;
	if(x > 1.0 - r || x < - (1.0 - r)) dir_x = - dir_x;
	if(z > 2.0 - r || z < - (2.0 - r)) dir_z = - dir_z; 
	double l2 = (x - self->x) * (x - self->x) + (z - self->z) * (z - self->z);
	if(l2 == 0.0) l2 = 0.000001;
	if((GLdouble)l2 < (r + self->r) * (r + self->r))
	{
		double theta = acos(((self->x - x) * dir_x + (self->z - z) * dir_z) / sqrt(l2));
		double tot_x = (1.99 + 0.001 * (rand() % 20))* (self->z - z) * sin(theta) / sqrt(l2);
		double tot_z = - (1.99 + 0.001 * (rand() % 20))* (self->x - x) * sin(theta) / sqrt(l2);
		dir_x = tot_x - dir_x;
		dir_z = tot_z - dir_z;
		GLdouble pr = (GLdouble)sqrt(double(dir_x * dir_x + dir_z * dir_z));
		dir_x = dir_x / pr;
		dir_z = dir_z / pr;
	}
	self->px = x;
	self->pz = z;
}

void puck::show()
{
	objCylinder = gluNewQuadric();
	glMatrixMode(GL_MODELVIEW);
	glColor3d(1,0,0);
	glPushMatrix();
		glTranslated(x,2.05,z);
		glRotated(90,1,0,0);
		gluCylinder(objCylinder, r, r, 0.1, 30, 5);
		gluDisk(objCylinder,0,r,30,5);
	glPopMatrix();
}