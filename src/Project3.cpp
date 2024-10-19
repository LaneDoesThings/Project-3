#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup-aug24.h"
#include <iostream>
#include <math.h>

#define canvas_Width 600
#define canvas_Height 600

void display_CB()
{

}

void timer_CB(int id)
{

}

void keyboard_CB(unsigned char key, int x, int y)
{

}

int main(int argc, char *argv[]) {
  char canvas_Name[] = "Project 2 - Lane Wright";
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);

  // Set up event handlers
  glutDisplayFunc(display_CB);
  glutTimerFunc(1000, timer_CB, 0);
  glutKeyboardFunc(keyboard_CB);

  glutMainLoop();
  return 0;
}