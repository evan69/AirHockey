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
GLdouble table_x = 1.0,table_y,table_z;
const GLdouble PI = 3.1415926535897;
const GLdouble half_l = (GLdouble)(sqrt(25.0 + 49.0) * tan(40 * PI / 360));
GLuint flag = 0;//1-win -1-lose

puck* game;
mallet* self;
AImallet* oppo;

char win[] = "You Win";
char los[] = "You Lose";

void drawFlag()
{
    glRasterPos3f(-0.5,2.03,0);
	glColor3d(1,0,0);
    if (flag==-1)
        for (int i=0;i<=7;++i)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, los[i]);
    if (flag==1)
        for (int i=0;i<=6;++i)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, win[i]);
}

void display();
void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7,0.7,0.9,0);
	glColor3d(0,0,0.5);
	glViewport(0,0,WIDTH,HEIGHT);
	GLfloat mat_ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {100.0};
    GLfloat mat_position[] = {0, 0, 50, 1};
    
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    
    glLightfv(GL_LIGHT0, GL_POSITION, mat_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
    
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
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
	glColor3d(0,1,1);
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
	self->show();
	oppo->show();
	game->show();
	drawFlag();
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
	if(flag != 0 && a == 13)
	{
		flag = 0;
		//self = new mallet('b',table_x,table_y,0.15,0.1);
		oppo = new AImallet('p',0,-1.5,0.15,0.1,game);
		game = new puck(0.1,0.1,self,oppo,&flag);
		oppo->info = game;
	}
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
	self->update(table_x,table_z);
	glutPostRedisplay();
}

void idle()
{
	if(flag != 0) return;
	oppo->control();
	game->update(0.002);
	glutPostRedisplay();
}

int main(int argc,char** argv)
{
	srand(time(0));
	self = new mallet('b',table_x,table_y,0.15,0.1);
	oppo = new AImallet('p',0,-1.5,0.15,0.1,game);
	game = new puck(0.1,0.1,self,oppo,&flag);
	oppo->info = game;
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