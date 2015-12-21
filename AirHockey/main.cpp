#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <iostream>

using namespace std;

#include "mallet.h"
#include "puck.h"

const GLuint WIDTH = 480;
const GLuint HEIGHT = 480;
GLdouble alpha = 30.0;
const GLdouble a_step = 6.0;
const GLdouble a_max = 42.1;
GLdouble table_x,table_y,table_z;
const GLdouble PI = 3.1415926535897;
const GLdouble half_l = (GLdouble)(sqrt(25.0 + 49.0) * tan(40 * PI / 360));

mallet self = mallet('b',table_x,table_y,0.15,0.1);
mallet oppo = mallet('p',0,-1.7,0.15,0.1);
puck game = puck(0.1,0.1,&self,&oppo);

void display();
void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7,0.7,0.9,0);
	glColor3d(0,0,0.5);
	glViewport(0,0,WIDTH,HEIGHT);
	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,1,0.01,100);
	glMatrixMode(GL_MODELVIEW);
	*/
	display();
}

void drawCube(GLdouble x,GLdouble y,GLdouble z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScaled(x,y,z);
	glutSolidCube(2);
	glPopMatrix();
}

void drawSourround()
{
	glColor3d(0.8,0.8,0.8);

	glPushMatrix();
		glTranslated(1.05,0,0);
		drawCube(0.05,2,2);
	glPopMatrix();

	glPushMatrix();
		glTranslated(-1.05,0,0);
		drawCube(0.05,2,2);
	glPopMatrix();
	
	glPushMatrix();
		glTranslated(0,0,2.05);
		drawCube(1.1,2,0.05);
	glPopMatrix();
	
	glPushMatrix();
		glTranslated(0,0,-2.05);
		drawCube(1.1,2,0.05);
	glPopMatrix();
}

void drawWall()
{
	glColor3d(1,1,1);
	glPushMatrix();
		glTranslated(1.05,2.05,0);
		drawCube(0.05,0.05,2);
	glPopMatrix();

	glPushMatrix();
		glTranslated(-1.05,2.05,0);
		drawCube(0.05,0.05,2);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0.75,2.05,2.05);
		drawCube(0.35,0.05,0.05);
	glPopMatrix();
	
	glPushMatrix();
		glTranslated(-0.75,2.05,2.05);
		drawCube(0.35,0.05,0.05);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0.75,2.05,-2.05);
		drawCube(0.35,0.05,0.05);
	glPopMatrix();
	
	glPushMatrix();
		glTranslated(-0.75,2.05,-2.05);
		drawCube(0.35,0.05,0.05);
	glPopMatrix();
}

void drawPlayArea()
{
	glMatrixMode(GL_MODELVIEW);
	glColor3d(0,1,0);
	drawCube(1,2,2);
	drawSourround();
	drawWall();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40,1,1,100);
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,7,5,0,0,0,0,5,-7);
	//gluLookAt(0,0,0,0,0,-1,0,1,0);
	glPushMatrix();
	glRotated(alpha,0,1,0);
	
	drawPlayArea();
	self.show();
	oppo.show();
	game.show();
	/*
	glPushMatrix();
	glTranslated(table_x,2,table_z);
	drawCube(0.1,0.1,0.1);
	glPopMatrix();
	*/
	
	glPopMatrix();
	
	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char a,int x,int y)
{
	if(a == 27) exit(0);
	if(a == 'a' || a == 'A')
		alpha += a_step;
	if(a == 'd' || a == 'D')
		alpha -= a_step;
	if(alpha > a_max) alpha = a_max;
	if(alpha < -a_max) alpha = -a_max;
	glutPostRedisplay();
}

void mouse(int x,int y)
{
	//printf("x,y:%d %d\n",x,y);
	GLdouble t = half_l / 240.0;
	GLdouble point_x = x * t - half_l;
	GLdouble point_y = (half_l - y * t) * sin(atan(5.0 / 7.0));
	GLdouble point_z = (y * t - half_l) * cos(atan(5.0 / 7.0));
	
	GLdouble tmp_table_x = point_x * (- 5 / (point_y - 7.0));
	//table_y = 7.0 + (point_y - 7.0) * (- 5 / (point_y - 7.0));
	GLdouble tmp_table_z = 5.0 + (point_z - 5.0) * (- 5 / (point_y - 7.0));
	GLdouble rad_alpha = 2.0 * PI * alpha / 360.0;
	table_x = (tmp_table_x / cos(rad_alpha)) - (tmp_table_z + tmp_table_x * tan(rad_alpha)) * sin(rad_alpha);
	table_z = (tmp_table_z + tmp_table_x * tan(rad_alpha)) * cos(rad_alpha);
	if(table_x < -0.85) table_x = -0.85;
	if(table_x > 0.85) table_x = 0.85;
	if(table_z < 0.15) table_z = 0.15;
	if(table_z > 1.85) table_z = 1.85;
	self.update(table_x,table_z);
	glutPostRedisplay();
}

void idle()
{
	game.update(0.002);
	glutPostRedisplay();
}

int main(int argc,char** argv)
{
	srand(time(0));
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutCreateWindow("Air Hockey Game");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}