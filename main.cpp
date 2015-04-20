/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>

// C library headers
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cassert>
#include <windows.h>


// C++ headers
#include <iostream>
#include <string>
#include <random>

#include "rgbpixmap.cpp"
#include "Cycle.cpp"


using namespace std;


bool fullScreenMode = true; // Full-screen or windowed mode?

// OpenGL utility Functions

double windowPosX = 10;
double windowPosY = 30;
double windowWidth = glutGet(GLenum GLUT_SCREEN_WIDTH)-30;
double windowHeight = glutGet(GLenum GLUT_SCREEN_HEIGHT)-20;

void keyBoard(unsigned char key, int x, int y)
{
    switch(key)
    {

    case 27:
        exit(0);
    }
}

void specialKeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
        fullScreenMode = !fullScreenMode;         // Toggle state
        if (fullScreenMode)                       // Full-screen mode
        {
            windowPosX   = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
            windowPosY   = glutGet(GLUT_WINDOW_Y);
            windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
        }
        else                                             // Windowed mode
        {
            glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
            glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
        }
        break;
    }
}


std::random_device rd;
std::mt19937 gen(rd());

void randomColor()
{
    double colorR = generate_canonical<double, 2>(gen);
    double colorG = generate_canonical<double, 2>(gen);
    double colorB = generate_canonical<double, 2>(gen);
    glColor3f(colorR,colorG,colorB);
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//===========================================================================================



#define PI 3.1415926535897932384626433832795
#define MaximumTheta 1.0
#define MinimumTheta 0.0009
#define MaximumRadious 10.5
#define MinimumRadious .9

int width =  glutGet(GLenum GLUT_SCREEN_WIDTH);
int height =  glutGet(GLenum GLUT_SCREEN_HEIGHT);


GLfloat IncrementTheta = 0.05;
GLint WindowSizX=width,WindowSizY=height;

GLfloat EyePosition_X=2.0,EyePosition_Y=1.3,EyePosition_Z= 2.0;
GLfloat Radious=3.3,ProRadious=3.0,InitialTheta1=0.716,InitialTheta2=0.403;

GLfloat Center_X=0.0,Center_Y=0.0,Center_Z=0.0;
GLfloat Up_X=0.0,Up_Y=1.0,Up_Z=0.0;

enum { ClockWise,AntiClockWise };
enum {Theta1,Theta2};

float Near=.05, Far=10.0,fova = 50.0;

GLint direction = AntiClockWise;
GLint PressMovenent=Theta1;

RGBpixmap pix[15];

//FUNCTION DECLEARATION

void CalculationX_Y_Z();
void CalculationTH1();
void CalculationTH2();
void Movenent();



GLUquadricObj *cone, *base, *qsphere, *cylinder;









void tableLeg(double thick, double len)

{

    glPushMatrix();
    glTranslated(0, len/2, 0);
    glScaled(thick, len, thick);
    glutSolidCube(1.0);
    glPopMatrix();
}


void buildingBlock()
{

    double topWidth = 0.6;
    double topThick = 0.02;
    double legThick = 0.02;
    double legLen = 0.3;

    glPushMatrix();
    glTranslated(0, legLen, 0);
    glScaled(topWidth, topThick, topWidth);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0, 0);
    glScaled(topWidth, topThick, topWidth);
    glutSolidCube(1.0);
    glPopMatrix();


    glPushMatrix();
    glPopMatrix();

    /// Solid Cube

    glPushMatrix();
    glScaled(1,0.5,1);
    glTranslated(0,0.3,0);
    glutSolidCube(0.55);
    glPopMatrix();


    double dist = 0.95 * topWidth/2.0 - legThick/2.0;
    glPushMatrix();
    glTranslated(dist, 0, dist);
    tableLeg(legThick, legLen);
    glTranslated(0, 0, -2*dist);
    tableLeg(legThick, legLen);
    glTranslated(-2*dist, 0, 2*dist);
    tableLeg(legThick, legLen);
    glTranslated(0, 0, -2*dist);
    tableLeg(legThick, legLen);
    glPopMatrix();
}


void fench()
{

    double topWidth = 0.6;
    double topThick = 0.02;
    double legThick = 0.02;
    double legLen = 0.3;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,5);
    glEnable(GL_TEXTURE_2D);
    glTranslated(0, legLen, 0);
    glScaled(topWidth, topThick, topWidth);
    glutSolidCube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


void drawfench()
{
    glPushMatrix();
    glRotated(90,0,0,1);
    fench();
    glPopMatrix();
}

void draw2fench()
{
    glPushMatrix();
    glScalef(1,0.3,1);
    glTranslated(0.6,1.28,0);
    glRotated(90,0,1,0);
    drawfench();
    glPopMatrix();
}

void draw3fench()
{
    glPushMatrix();
    glTranslated(-0.6,0,0);
    draw2fench();
    glPopMatrix();
}


void draw4fench()
{

    glPushMatrix();
    glScaled(0.5,1,1);
    glTranslated(0.3,0,0);
    draw3fench();
    glPopMatrix();


}


void grass()
{

    double topWidth = 0.6;
    double topThick = 0.02;
    double legThick = 0.02;
    double legLen = 0.3;

    glPushMatrix();
    glTranslated(0, legLen, 0);
    glScaled(topWidth, topThick, topWidth);
    glutSolidCube(1.0);
    glPopMatrix();


    glPushMatrix();
    glScalef(1,0.3,1);
    glTranslated(0,1.28,0);
    drawfench();
    glPopMatrix();

    glPushMatrix();
    glScalef(1,0.3,1);
    glTranslated(0.6,1.28,0);
    drawfench();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0,0,-0.6);
    draw3fench();
    glPopMatrix();

    ///void draw4fench()

    glPushMatrix();
    draw4fench();
    glPopMatrix();


    glPushMatrix();

    glScaled(0.6,1,1);
    glTranslated(-0.5,0,0);
    draw4fench();

    glPopMatrix();


    glPushMatrix();

    glPopMatrix();

    /// Gate code goes here

}


void building()
{
    glPushMatrix();
    glTranslated(0.4, 0, 0.4);
    glBindTexture(GL_TEXTURE_2D,2);
    glEnable(GL_TEXTURE_2D);

    buildingBlock();
    glTranslated(0,0.30,0);
    buildingBlock();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

}



void gateRoof()
{

    double topWidth = 0.6;
    double topThick = 0.02;
    double legThick = 0.02;
    double legLen = 0.3;

    glPushMatrix();
    glTranslated(0, legLen, 0);
    glScaled(topWidth, topThick, topWidth);
    glutSolidCube(1.0);
    glPopMatrix();
}

void gateRoof2()
{
    glPushMatrix();
    glRotated(60,0,0,1);
    glTranslated(0,0.5,0);
    glTranslated(0,0,0.25);
    gateRoof();
    glPopMatrix();
}

void gateRoof3()
{
    glPushMatrix();
    glTranslatef(0.5,-0.1,0);
    glScalef(1,0.8,1);
    gateRoof2();
    glPopMatrix();
}

void gate2Roof()
{
    glPushMatrix();
    glTranslatef(0.6,0,0);
    glRotated(60,0,0,1);
    gateRoof3();
    glPopMatrix();
}

void gate2Roof2()
{
    glPushMatrix();
    glTranslated(-0.1,0.53,0);
    gate2Roof();
    glPopMatrix();
}


void gateRoofRoof()
{
    glPushMatrix();
    glRotated(65,0,0,1);
    gate2Roof();
    glPopMatrix();
}
void gateRoofRoof2()
{
    glPushMatrix();
    glTranslatef(0,0.38,0);
    gateRoofRoof();
    glPopMatrix();
}

void gate(double topWidth, double topThick, double legThick, double legLen)
{
    glPushMatrix();
    glTranslated(0,0.2,0);
    gateRoof3();
    glPopMatrix();

    glPushMatrix();
    glScalef(1,0.9,1);
    gate2Roof2();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.17,0,0);
    gateRoofRoof2();
    glPopMatrix();


    double dist = 0.95 * topWidth/2.0 - legThick/2.0;
    glPushMatrix();
    glTranslated(dist, 0, dist);
    tableLeg(legThick*8.5, legLen);
    glTranslated(0, 0, -2*dist);
    glTranslated(-2*dist, 0, 2*dist);
    tableLeg(legThick*8.5, legLen);

    glPopMatrix();
}


void gatefinal1()
{
    glPushMatrix();
    glTranslated(0.4, 0, 0.4);
    glBindTexture(GL_TEXTURE_2D,6);
    glEnable(GL_TEXTURE_2D);

    gate(0.6, 0.02, 0.02, 0.3);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}


void gatefinal2()
{
    glPushMatrix();
    glTranslated(-0.5,0,0.73);
    glScalef(0.7,0.7,0.7);
    gatefinal1();
    glPopMatrix();
}


void fenchPillar()
{

    double topWidth = 0.6;
    double topThick = 0.3;
    double legThick = 0.02;
    double legLen = 0.3;



    glPushMatrix();
    glTranslated(0, legLen, 0);
    glScaled(topWidth, topThick, topWidth);
    glutSolidCube(1.0);
    glPopMatrix();

}



#define SOLID_CLOSED_CYLINDER(QUAD, BASE, TOP, HEIGHT, SLICES, STACKS) \
gluCylinder(QUAD, BASE, TOP, HEIGHT, SLICES, STACKS); \
glRotatef(180, 1,0,0); \
gluDisk(QUAD, 0.0f, BASE, SLICES, 1); \
glRotatef(180, 1,0,0); \
glTranslatef(0.0f, 0.0f, HEIGHT); \
gluDisk(QUAD, 0.0f, TOP, SLICES, 1); \
glTranslatef(0.0f, 0.0f, -HEIGHT);

void solidCylinder()
{
    GLfloat lightIntensity[] = {2.7f, 2.7f, 2.7f, 1.0f};
    GLfloat lightPosition[] = {2.0f, 6.0f, 3.0f, 0.0f};


    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    GLUquadric *quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricOrientation(quadric, GLU_INSIDE);
    glBindTexture(GL_TEXTURE_2D,6);
    glEnable(GL_TEXTURE_2D);
    SOLID_CLOSED_CYLINDER(quadric, 0.17f, 0.17f, 0.15f, 20, 20)
    glDisable(GL_TEXTURE_2D);
    gluDeleteQuadric(quadric);
}


void solidCylinderMe()
{
    glPushMatrix();
    glRotated(90,1,0,0);
    glScaled(1,1,1.2);
    glTranslated(1,1,0);
    solidCylinder();
    glPopMatrix();

}

void solidCylinderFinal1()
{
    glPushMatrix();
    glScalef(0.5,1.3,0.5);
    glTranslated(1.35,0.151,1.35);
    solidCylinderMe();
    glPopMatrix();
}

void gateBoundary1Side()
{
    glPushMatrix();
    solidCylinderFinal1();
    glTranslated(-2.35,0,0);
    solidCylinderFinal1();
    glPopMatrix();
}


void roadFunc()
{
    double topWidth = 0.6;
    double topThick = 0.02;
    double legThick = 0.02;
    double legLen = 0.3;

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glTranslated(0, legLen, 0);
    glScaled(topWidth, topThick, topWidth);
    glutSolidCube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


void road()
{

    GLfloat lightIntensity[] = {2.7f, 2.7f, 2.7f, 1.0f};
    GLfloat lightPosition[] = {2.0f, 6.0f, 3.0f, 0.0f};


    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,7);
    glEnable(GL_TEXTURE_2D);
    roadFunc();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void roadInit()
{
    glPushMatrix();
    glScalef(1,1,2.75);
    glTranslated(-0.23, -0.32, 0.25);
    road();
    glPopMatrix();

}


void door()
{

    GLfloat lightIntensity[] = {2.7f, 2.7f, 2.7f, 1.0f};
    GLfloat lightPosition[] = {2.0f, 6.0f, 3.0f, 0.0f};


    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,8);
    glEnable(GL_TEXTURE_2D);
    roadFunc();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void door1()
{
    glPushMatrix();
    glRotated(90, 1, 0, 0);
    glScalef(0.5, 1, 1);
    glTranslated(0,.5,-0.5);
    door();
    glPopMatrix();
}

void doorFinal()
{

    glPushMatrix();
    glTranslated(-0.22,-0.18,-0.9);
    glScalef(1,0.8,1);
    door1();
    glPopMatrix();
}


void windowFunction()
{


    glPushMatrix();
    glScalef(0.5, 0.5, 0.5);
    glTranslated(1, 0.2, 0);
    doorFinal();

    glPushMatrix();
    glRotated(-90, 1, 0, 0);
    glScalef(1, 1, 0.8);
    glPushMatrix();
    glTranslated(0, 0, 0.65);
    doorFinal();
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();



}

void finalWindow1()
{
    glPushMatrix();
    glTranslated(-0.28,0.1,-0.07);
    windowFunction();
    glPopMatrix();
}

void twofinalWindow()
{
    glPushMatrix();
    finalWindow1();
    glPopMatrix();

    glPushMatrix();
    glTranslated(-0.67, 0, 0);
    finalWindow1();
    glPopMatrix();
}

void sideFinalWindow()
{
    glPushMatrix();
    glTranslated(0.48, 0.59, -0.3);
    glRotated(90, 0, 1, 0);
    finalWindow1();
    glPopMatrix();
}


void cycleFinal(){
    glPushMatrix();
        glScaled(0.2, 0.2, 0.2);
        glTranslated(1,2,0);
        displayCycle();
    glPopMatrix();
}


void cycleFinal2(){
    glPushMatrix();
        glScaled(0.4, 0.4, 0.4);
        glTranslated(1.3, -0.17, -0.7);
        glRotated(90, 0, 1, 0);
        cycleFinal();
    glPopMatrix();
}





///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

// Drawing tree

GLuint makeaTree;

float x, y, z;

void makeCylinder(float height, float base)
{
    GLUquadric *obj = gluNewQuadric();
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


void initTree(){
    
    makeaTree=glGenLists(1);
    glNewList(makeaTree, GL_COMPILE);
    makeTree(4,0.2);
    glEndList();
    
}

void treeDisplay(){
    
    GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat mat_defused[] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_defused);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


    GLfloat lightIntensity[] = {1.7f, 1.7f, 1.7f, 1.0f};
    GLfloat lightPosition[] = {2.0f, 6.0f, 3.0f, 0.0f};


    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    glViewport(0, 0, WindowSizX,WindowSizY );


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectR = (float)WindowSizX / WindowSizY;

    gluPerspective(fova,aspectR, Near, Far);


    glMatrixMode(GL_MODELVIEW);/////
    glLoadIdentity();//////

    gluLookAt(EyePosition_X,EyePosition_Y,EyePosition_Z,Center_X,Center_Y,Center_Z,Up_X,Up_Y,Up_Z);

    ///start drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    initTree();
    
    
    
    //glFlush();
    //glutSwapBuffers();
    
}


void treeReshape(int w, int h){
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLfloat) w/(GLfloat) h, 0.001, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0,-8.0,-50.0);
}



/// End Tree Code
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////













void display(void)
{

    GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat mat_defused[] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_defused);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


    GLfloat lightIntensity[] = {1.7f, 1.7f, 1.7f, 1.0f};
    GLfloat lightPosition[] = {2.0f, 6.0f, 3.0f, 0.0f};


    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    glViewport(0, 0, WindowSizX,WindowSizY );


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectR = (float)WindowSizX / WindowSizY;

    gluPerspective(fova,aspectR, Near, Far);


    glMatrixMode(GL_MODELVIEW);/////
    glLoadIdentity();//////

    gluLookAt(EyePosition_X,EyePosition_Y,EyePosition_Z,Center_X,Center_Y,Center_Z,Up_X,Up_Y,Up_Z);

    ///start drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     /// Trees
     glPushMatrix();
       // glutDisplayFunc(treeDisplay());
     glPopMatrix();


    /// Building Block

    glPushMatrix();
        glTranslated(-1,0,-0.8);
        glScalef(2, 2,1);
        building();
    glPopMatrix();


   
    


    /// Cycle Garage
    glPushMatrix();
        glTranslated(0.6,-0.3,-0.4);
        road();
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 6);

        cycleFinal2();
        glTranslated(0.2, 0, 0);
        cycleFinal2();

        glDisable(GL_TEXTURE_2D);
    glPopMatrix();




    /// Grass
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,4);
    glEnable(GL_TEXTURE_2D);
    glScalef(4,1,4);
    glTranslated(0,-0.33,0);
    grass();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    /// gate


    glPushMatrix();
    glTranslated(0,-0.04,0);
    gatefinal2();
    glPopMatrix();


    /// gate boundary

    glPushMatrix();
    gateBoundary1Side();
    glTranslated(0,0,-2.36);
    gateBoundary1Side();
    glPopMatrix();

    /// road Initial

    glPushMatrix();
    roadInit();
    glPopMatrix();

    /// door final

    glPushMatrix();
    doorFinal();
    glPopMatrix();

    /// window down

    glPushMatrix();
    twofinalWindow();
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, 0.6, 0);
    twofinalWindow();
    glPopMatrix();


    /// side final window

    glPushMatrix();
    sideFinalWindow();
    glPopMatrix();


    glPushMatrix();
    glTranslated(0, -0.6, 0);
    sideFinalWindow();
    glPopMatrix();


    glFlush();

    glutSwapBuffers();
}



void RepeatDisplay()
{
    glutPostRedisplay();
}


void Movenent()
{

    CalculationX_Y_Z();

    if(PressMovenent == Theta1)
        CalculationTH1();
    else
        CalculationTH2();

}


void CalculationX_Y_Z()
{
    ProRadious = Radious * cos(InitialTheta2);
    EyePosition_Y = Radious * sin(InitialTheta2);

    EyePosition_X = ProRadious * cos(InitialTheta1);
    EyePosition_Z = ProRadious * sin(InitialTheta1);

    printf("EyePosition_X: %f\n",EyePosition_X);
    printf("EyePosition_Y: %f\n",EyePosition_Y);
    printf("EyePosition_Z: %f\n\n",EyePosition_Z);
}



void CalculationTH1()
{
    if (InitialTheta1 > PI*2)
        InitialTheta1 = 0.0;
    if(direction == AntiClockWise)
    {
        InitialTheta1 += IncrementTheta;
        printf("InitialTheta1: %f\n",InitialTheta1);
    }
    else
        InitialTheta1 -= IncrementTheta;
    printf("InitialTheta1: %f\n",InitialTheta1);
}



void CalculationTH2()
{
    if (InitialTheta2 > PI*2)
        InitialTheta2 = 0.0;

    if(direction == AntiClockWise)
    {
        InitialTheta2 += IncrementTheta;
        printf("InitialTheta2: %f\n",InitialTheta2);
    }

    else
        InitialTheta2 -= IncrementTheta;
    printf("InitialTheta2: %f\n",InitialTheta2);

}



void CalculationRadious(unsigned int key)
{

    if(key == '-')
    {
        if(Radious > MaximumRadious)
            Radious = MaximumRadious;
        else
            Radious += 0.2;
        printf("Radious: %f\n",Radious);
    }
    else if(key == '+')
    {
        if(Radious <= MinimumRadious)
            Radious = MinimumRadious;
        else
            Radious -= 0.2;
        printf("Radious: %f\n",Radious);
    }

    CalculationX_Y_Z();
}


void KeybordFunction( unsigned char key, int x, int y )
{

    if(key=='-')

        CalculationRadious(key);

    else if(key=='+')
        CalculationRadious(key);

    else if(key == '4')
    {
        direction = ClockWise;
        PressMovenent = Theta1;
        Movenent();
    }

    else if(key == '6')
    {

        direction = AntiClockWise;
        PressMovenent = Theta1;
        Movenent();
    }
    else if(key == '8')
    {

        direction = ClockWise;
        PressMovenent = Theta2;
        Movenent();
    }
    else if(key == '2')
    {

        direction = AntiClockWise;
        PressMovenent = Theta2;
        Movenent();
    }
    else if(key == 27)  exit(0);

}



void Init()
{
    glEnable(GL_TEXTURE_2D);


    pix[0].makeCheckImage();
    pix[0].setTexture(1);	// create texture int parameter as TextureName


    //pix[0].readBMPFile("teapot.bmp");
    //	pix[0].setTexture(1);

    pix[1].readBMPFile("brick.bmp");
    pix[1].setTexture(2);

    pix[2].readBMPFile("wood.bmp");
    pix[2].setTexture(3);

    pix[3].readBMPFile("grass.bmp");
    pix[3].setTexture(4);

    pix[4].readBMPFile("wall.bmp");
    pix[4].setTexture(5);

    pix[5].readBMPFile("gate.bmp");
    pix[5].setTexture(6);

    pix[6].readBMPFile("road.bmp");
    pix[6].setTexture(7);

    pix[7].readBMPFile("door.bmp");
    pix[7].setTexture(8);

}





int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(windowPosX, windowPosY);
    glutInitWindowSize(WindowSizX, WindowSizY);
    glutCreateWindow("3D Home");
    glutKeyboardFunc(KeybordFunction);

    glutIdleFunc(RepeatDisplay);
    
    glutDisplayFunc(display);
    
    //glutReshapeFunc(treeReshape);
    //glutDisplayFunc(treeDisplay);
    

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glutSpecialFunc(specialKeys);
    glutFullScreen();

    Init();

    glutMainLoop();

    return 0;
}
