#include "puck.h"

puck::puck(GLdouble _r,GLdouble _h)
{
	x = 0;
	y = 0;
	r = _r;
	h = _h;
	dir_x = (GLdouble)rand();
	dir_y = (GLdouble)rand();
	GLdouble pr = sqrt(dir_x * dir_x + dir_y * dir_y);
	dir_x = dir_x / pr;
	dir_y = dir_y / pr;
}

void puck::update()
{
	x = dir_x;
	y = dir_y;
}