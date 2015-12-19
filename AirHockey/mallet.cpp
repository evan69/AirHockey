#include "mallet.h"

mallet::mallet(char _c,GLdouble _x,GLdouble _y,GLdouble _r,GLdouble _h)
{
	color = _c;
	x = _x;
	y = _y;
	r = _r;
	h = _h;
}

void mallet::update(GLdouble _x,GLdouble _y)
{
	x = _x;
	y = _y;
}

void mallet::show()
{
	if(color == 'b') glColor3d(0,0,1);
	if(color == 'p') glColor3d(1,0.1,0.5);
	GLUquadricObj *objCylinder = gluNewQuadric();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslated(x,2.05,y);
		glRotated(90,1,0,0);
		//glutSolidCube(0.1);
		gluCylinder(objCylinder, 0.15, 0.15, 0.1, 30, 5);
		gluDisk(objCylinder,0,0.15,30,5);
	glPopMatrix();
}