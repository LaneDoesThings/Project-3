#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup-aug24.h"
#include <iostream>
#include <math.h>

#define canvas_Width 800
#define canvas_Height 600
#define zPlane -50
#define shipLength 25
#define framerate 15
#define groundLevel 7

float shipX = canvas_Width / 2;
float shipY = canvas_Height - shipLength;
float initialShipY = shipY;
float timeEllapsed = 0.0f;
bool thrust = false;

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

void ground()
{
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINES);
  glVertex3i(0, groundLevel, zPlane);
  glVertex3i(canvas_Width, groundLevel, zPlane);
  glEnd();
}

void display_CB()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  
  ship();
  ground();

  glFlush();
}

void timer_CB(int id)
{
  if(id == 0)
  {
    if(shipY - shipLength > groundLevel)
    {
      if(thrust)
      {
        initialShipY += 5;
        thrust = false;
      }

      shipY = initialShipY - (16.0f * timeEllapsed * timeEllapsed);

      timeEllapsed += 1.0f/framerate;
    }
    else shipY = groundLevel + shipLength;

    glutTimerFunc(1000 / framerate, timer_CB, 0);
    glutPostRedisplay();
  }
}

void keyboard_CB(unsigned char key, int x, int y)
{
  if(key == 'u')
  {
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

int main(int argc, char *argv[]) {
  char canvas_Name[] = "Project 2 - Lane Wright";
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);

  // Set up event handlers
  glutDisplayFunc(display_CB);
  glutTimerFunc(1000 / framerate, timer_CB, 0);
  glutKeyboardFunc(keyboard_CB);

  glutMainLoop();
  return 0;
}