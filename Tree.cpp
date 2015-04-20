#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <iostream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;




GLuint makeaTree;
float x,y,z;



void makeCylinder(float height, float base)
{
    GLUquadric *obj = gluNewQuadric();
    //gluQuadricDrawStyle(obj, GLU_LINE);
    glColor3f(0.0f, .4f, .5f);
    glPushMatrix();
    glRotatef(-90, 1.0,0.0,0.0);
    gluCylinder(obj, base,base-(0.2*base), height, 20,20);
    glPopMatrix();
    glutSwapBuffers();
}




void makeTree(float height, float base)
{

    float angle;
    makeCylinder(height, base);
    glTranslatef(0.0, height, 0.0);
    height -= height*.2;
    base-= base*0.3;
    for(int a= 0; a<3; a++)
    {
        angle = rand()%50+20;
        if(angle >48)
            angle = -(rand()%50+20);
        if (height >1)
        {
            glPushMatrix();
            glRotatef(angle,1,0.0,1);
            makeTree(height,base);
            glPopMatrix();

        }
        else
        {
            glColor3f(0.3,.8,0.2);
            glutSolidSphere(.2,5,10);
        }
    }

}
void initTree(void)
{
    glClearColor(1.0,1.0,1.0,1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    makeaTree=glGenLists(1);
    glNewList(makeaTree, GL_COMPILE);
        makeTree(4,0.2);
    glEndList();

}


void reshapeTree(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLfloat) w/(GLfloat) h, 0.001, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0,-8.0,-50.0);
}
