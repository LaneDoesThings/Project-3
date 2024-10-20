/*
CS 445/545 Prog 3 for Lane Wright

*/


#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup-aug24.h"
#include <iostream>
#include <math.h>
#include <string>

#define canvas_Width 800
#define canvas_Height 600
#define zPlane -50
#define shipLength 25
#define framerate 15
#define groundLevel 7
#define landingWidth 40
#define landingHeight 10

float shipX = canvas_Width / 2;
float shipY = canvas_Height - shipLength;
float initialShipY = shipY;
float timeEllapsed = 0.0f;
float gravity = 0.0f;
int upForce = 0;
int fuel = 200;
bool thrust = false;
bool planetChosen = false;
bool grounded = false;
bool win = false;

void ship()
{
  glColor3ub(0, 119, 200); //UAH blue
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(shipX, shipY, zPlane);
  glScalef(shipLength, shipLength, shipLength);
  glutWireOctahedron();
  glPopMatrix();

}

void landingPad()
{
  glColor3f(0.1f, 0.8f, 0.1f);
  glBegin(GL_LINES);
  glVertex3i(1, groundLevel, zPlane);
  glVertex3i(1, groundLevel + landingHeight, zPlane);
  glVertex3i(1, groundLevel + landingHeight, zPlane);
  glVertex3i(landingWidth / 2, groundLevel, zPlane);
  glVertex3i(landingWidth / 2, groundLevel, zPlane);
  glVertex3i(landingWidth + 1, groundLevel + landingHeight, zPlane);
  glVertex3i(landingWidth + 1, groundLevel + landingHeight, zPlane);
  glVertex3i(landingWidth + 1, groundLevel, zPlane);
  glEnd();
}

void ground()
{
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINES);
  glVertex3i(0, groundLevel, zPlane);
  glVertex3i(canvas_Width, groundLevel, zPlane);
  glEnd();
}

void fuelDisplay()
{
  glColor3f(0.0f,0.0f,0.0f);
  glRasterPos2i(canvas_Width - 75, canvas_Height - 20);
  std::string fuelString = "Fuel: " + std::to_string(fuel);
  const unsigned char* fuelCharArray = (const unsigned char *)fuelString.c_str();
  glutBitmapString(GLUT_BITMAP_8_BY_13, fuelCharArray);
}

void display_CB()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);


  ship();
  ground();
  landingPad();
  fuelDisplay();

  if(!planetChosen)
  {
    glCallList(1);
  }
  if(win)
  {
    glCallList(2);
  }
  glFlush();
}

void timer_CB(int id)
{
  if(id == 0)
  {
    if((shipY - shipLength) < landingHeight && shipX < landingWidth)
    {
      grounded = true;
      win = true;
      shipX = landingWidth / 2;
      shipY = groundLevel + shipLength;
      
    }
    else if(shipY - shipLength > groundLevel)
    {
      if (thrust)
      {
        shipY += 5;
        upForce += 5;
        thrust = false;
      }
      else if (planetChosen)
      {
        shipY = upForce + (initialShipY - (gravity * timeEllapsed * timeEllapsed));

        timeEllapsed += 1.0f/framerate;
      }
    }
    else
    { 
      grounded = true;
      shipY = groundLevel + shipLength;
    }

    glutTimerFunc(1000 / framerate, timer_CB, 0);
    glutPostRedisplay();
  }
}

void keyboard_CB(unsigned char key, int x, int y)
{
  if(!planetChosen)
  {
    if (key == 'v')
    {
      gravity = 29.0f;
      planetChosen = true;
    }
    if (key == 'i')
    {
      gravity = 5.9f;
      planetChosen = true;
    }
  }
  if(!grounded)
  {
    if(key == 'u')
    {
      fuel -=5;
      thrust = true;
    }
    if(key == 'h')
    {
      shipX -= 4;
    }
    else if(key == 'j')
    {
      shipX += 4;
    }
  }
}

int main(int argc, char *argv[]) {
  char canvas_Name[] = "Project 2 - Lane Wright";
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);

  glGenLists(3);

  //Start List
  glNewList(1, GL_COMPILE);
  glColor3f(0.0f,0.0f,0.0f);
  glRasterPos2i((canvas_Width / 2) - 75, canvas_Height - 20);
  glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)"Press V or I to Start");
  glEndList();

  //Win List
  glNewList(2, GL_COMPILE);
  glColor3f(0.0f,0.0f,0.0f);
  glRasterPos2i((canvas_Width / 2) - 25, canvas_Height / 2);
  glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)"YOU WIN!");
  glEndList();

  // Set up event handlers
  glutDisplayFunc(display_CB);
  glutTimerFunc(1000 / framerate, timer_CB, 0);
  glutKeyboardFunc(keyboard_CB);

  glutMainLoop();
  return 0;
}