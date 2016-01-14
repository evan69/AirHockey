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

const GLuint WIDTH = 480;//屏幕宽
const GLuint HEIGHT = 480;//屏幕长
GLdouble alpha = 30.0;//xz平面内观察角
const GLdouble a_step = 6.0;//观察角改变步长
const GLdouble a_max = 42.1;//观察角最大值
GLdouble table_x = 0.2,table_y,table_z;//控制的mallet在xyz坐标系里的坐标
const GLdouble PI = 3.1415926535897;
const GLdouble half_l = (GLdouble)(sqrt(25.0 + 49.0) * tan(40 * PI / 360));
const GLdouble SPEED = 0.0035;//移动速度
GLuint flag = 0;//1-win -1-lose
int lastTime = 0;

BITMAPINFO *TexInfo; /* Texture bitmap information */
GLubyte *TexBits; /* Texture bitmap pixel bits */
GLuint panicTexture;

puck* game;//puck
mallet* self;//自己的mallet
AImallet* oppo;//电脑的mallet

const char win[] = "You Win";
const char los[] = "You Lose";

void drawFlag()//绘制胜利-失败标示
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

void errmess( const char s1[], char s2[] ){
   printf("ERR  %s, FILE %s \n",  s1, s2); getchar();
} /* errmess */


//http://unina.stidue.net/Universita'%20di%20Trieste/Ingegneria%20Industriale%20e%20dell'Informazione/Hmeljak/EGD/CORSI-05-07/EGD_07/EGD_07_3D3/Bitmap_AU.cpp
GLubyte *                          /* O - Bitmap data */
LoadDIBitmap( char *filename, /* I - File to load */
             BITMAPINFO **info)    /* O - Bitmap information */
{
    FILE             *fp;          /* Open file pointer stdio */
    GLubyte          *bits;        /* Bitmap pixel bits */
    int              bitsize;      /* Size of bitmap */
    int              infosize;     /* Size of header information */
    BITMAPFILEHEADER header;       /* File header */


    /* Try opening the file; use "rb" mode to read this *binary* file. */
    if ((fp = fopen(filename, "rb")) == NULL)  /* C file stdio */

       { errmess("open file ", filename); return (NULL); }
    
    /* Read the file header and any following bitmap information... */
    if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
	     fclose(fp);   
        errmess("Couldn't read the file header", filename);
        return (NULL);
    }

    if (header.bfType != 'MB') {  /* Check for BM reversed... */
        errmess("Not a bitmap file", filename);
        fclose(fp);   return (NULL);
    }

    infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
    if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
        errmess("Couldn't allocate memory for bitmap info", filename);
        fclose(fp);
        return (NULL);
    }

    if (fread(*info, 1, infosize, fp) < infosize)  {
        errmess("Couldn't read the bitmap header", filename);
        free(*info);   fclose(fp);   return (NULL);
    }

    /* we have the header info read in, allocate mem for the bitmap 
       and read *it* in  ...                               */
    if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
        bitsize = ((*info)->bmiHeader.biWidth *
                   (*info)->bmiHeader.biBitCount + 7) / 8 *
  	           abs((*info)->bmiHeader.biHeight);

    if ((bits = (GLubyte*) malloc(bitsize)) == NULL)   {	//aggiunto cast (GLubyte*)
        free(*info);    fclose(fp);   
        errmess("Couldn't allocate bitsize memory", filename);
        return (NULL);
    }

    if (fread(bits, 1, bitsize, fp) < bitsize)  {
        free(*info);   free(bits);   fclose(fp);   
        errmess("Couldn't read bitmap", filename);
        return (NULL);
    }

    /* OK, everything went fine - return the allocated bitmap... */
    fclose(fp);
    return (bits);
}  /* LoadDIBitmap WIN32 */

void display();
void init()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,0);
	glGenTextures(1, &panicTexture); // Create 1 Texture
	TexBits=LoadDIBitmap("panic.bmp", &TexInfo);
	if (NULL!=TexBits) {
		glBindTexture(GL_TEXTURE_2D, panicTexture);
		//settings
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //线性放大
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //线性缩小
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );//用纹理颜色,不考虑多边形
		//本身的颜色。如果改变函数的第3个参数为GL_MODULATE或GL_BLEND, 则可与多边形的颜色进行调制
		//specify image array
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TexInfo->bmiHeader.biWidth,
			 TexInfo->bmiHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TexBits);
		glShadeModel(GL_FLAT);
	}//http://unina.stidue.net/Universita'%20di%20Trieste/Ingegneria%20Industriale%20e%20dell'Informazione/Hmeljak/EGD/CORSI-05-07/EGD_07/EGD_07_3D3/Bitmap_AU.cpp

	
	glColor3d(1,1,1);
	glViewport(0,0,WIDTH,HEIGHT);
	//添加灯光
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
}

void drawCube(GLdouble x,GLdouble y,GLdouble z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScaled(x,y,z);
	glutSolidCube(2);
	glPopMatrix();
}//绘制一个立方体

void drawSourround()//绘制周围物体
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

void drawFloor() {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, panicTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3d(-16, -1, -10);
		glTexCoord2f(0, 1); glVertex3d(10, -1, -10);
		glTexCoord2f(1, 1); glVertex3d(10, -1, 4);
		glTexCoord2f(1, 0); glVertex3d(-16, -1, 4);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//glFlush();
}

void drawWall()//绘制四周的墙
{
	glColor3d(0.85,0.85,0);
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

void drawPlayArea()//绘制游戏区域
{
	glMatrixMode(GL_MODELVIEW);
	glColor3d(0,1,0);
	drawCube(1,2,2);
	drawSourround();
	drawWall();
	drawFloor();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40,1,1,100);
	glMatrixMode(GL_MODELVIEW);
}

void display()//显示，回调函数
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
	
	glPopMatrix();
	
	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char a,int x,int y)//键盘-回调函数
{
	if(a == 27) exit(0);
	if(flag != 0 && a == 13)
	{
		flag = 0;
		delete oppo;
		delete game;
		oppo = new AImallet('p',0, -1.5, 0.15, 0.1, game);
		game = new puck(0.1,0.1,self,oppo,&flag);
		oppo->info = game;
	}
	if(a == 'a' || a == 'A' || a == 37)
		alpha += a_step;
	if(a == 'd' || a == 'D' || a == 39)
		alpha -= a_step;
	if(alpha > a_max) alpha = a_max;
	if(alpha < -a_max) alpha = -a_max;
	glutPostRedisplay();
}

void mouse(int x,int y)//鼠标-回调函数
{
	GLdouble t = half_l / 240.0;
	GLdouble point_x = x * t - half_l;
	GLdouble point_y = (half_l - y * t) * sin(atan(5.0 / 7.0));
	GLdouble point_z = (y * t - half_l) * cos(atan(5.0 / 7.0));
	
	GLdouble tmp_table_x = point_x * (- 5 / (point_y - 7.0));
	GLdouble tmp_table_z = 5.0 + (point_z - 5.0) * (- 5 / (point_y - 7.0));
	GLdouble rad_alpha = 2.0 * PI * alpha / 360.0;
	table_x = (tmp_table_x / cos(rad_alpha)) - (tmp_table_z + tmp_table_x * tan(rad_alpha)) * sin(rad_alpha);
	table_z = (tmp_table_z + tmp_table_x * tan(rad_alpha)) * cos(rad_alpha);
	if(table_x < -0.85) table_x = -0.85;
	if(table_x > 0.85) table_x = 0.85;
	if(table_z < 0.15) table_z = 0.15;
	if(table_z > 1.85) table_z = 1.85;
	self->update(table_x,table_z);
	//以上为将鼠标在窗体中的二维坐标转换为mallet在xyz坐标系里的三维坐标
	glutPostRedisplay();
}

void idle()//空闲-回调函数
{
	int presentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = presentTime - lastTime;
	lastTime = presentTime;
	if(flag != 0) return;
	oppo->control(deltaTime);
	game->update(SPEED * deltaTime);
	glutPostRedisplay();
}

int main(int argc,char** argv)
{
	srand(time(0));
	self = new mallet('b',table_x,table_z,0.15,0.1);
	oppo = new AImallet('p', 0, -1.5, 0.15, 0.1, game);
	game = new puck(0.1,0.1,self,oppo,&flag);
	//创建游戏的puch和mallet
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