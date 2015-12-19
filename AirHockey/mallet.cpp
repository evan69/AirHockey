#include "mallet.h"

mallet::mallet(char _c,GLdouble _x,GLdouble _z,GLdouble _r,GLdouble _h)
{
	color = _c;
	x = _x;
	z = _z;
	r = _r;
	h = _h;
}

void mallet::update(GLdouble _x,GLdouble _z)
{
	double l2 = (_x - px) * (_x - px) + (_z - pz) * (_z - pz);
	if(l2 < (pr + r) * (pr + r)) return;
	x = _x;
	z = _z;
}

void mallet::show()
{
	if(color == 'b') glColor3d(0,0,1);
	if(color == 'p') glColor3d(1,0.1,0.5);
	objCylinder = gluNewQuadric();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslated(x,2.05,z);
		glRotated(90,1,0,0);
		gluCylinder(objCylinder, 0.15, 0.15, 0.1, 30, 5);
		gluDisk(objCylinder,0,0.15,30,5);
	glPopMatrix();
}