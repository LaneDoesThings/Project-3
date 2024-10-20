/*
CS 445/545 Prog 3 for Lane Wright

The ship is animated using the timer_CB function that updates its global position shipY and calls glutPostRedisplay.
The ship is also animated using the keyboard_CB function that updates its global position shipX through the 'h' and 'j' keys and the shipY position through the 'u' key.

Text is generated using bitmaps and is shown via display lists.

The display_CB function handles updating the display and draws the objects as well as calling the display lists for text.

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

//Draws the ship at shipX, shipY
void ship()
{
  glColor3ub(0, 119, 200); //UAH blue
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(shipX, shipY, zPlane); //Move the ship
  glScalef(shipLength, shipLength, shipLength); //Scale the ship
  glutWireOctahedron();
  glPopMatrix();

}

//Draws the landing pad at the bottom left of the screen
void landingPad()
{
  glColor3f(0.1f, 0.8f, 0.1f); //Green
  glBegin(GL_LINES);

  //Vertical Line
  glVertex3i(1, groundLevel, zPlane);
  glVertex3i(1, groundLevel + landingHeight, zPlane);
  
  //Diagonal lines
  glVertex3i(1, groundLevel + landingHeight, zPlane);
  glVertex3i(landingWidth / 2, groundLevel, zPlane);
  
  glVertex3i(landingWidth / 2, groundLevel, zPlane);
  glVertex3i(landingWidth + 1, groundLevel + landingHeight, zPlane);

  //Vertical Line
  glVertex3i(landingWidth + 1, groundLevel + landingHeight, zPlane);
  glVertex3i(landingWidth + 1, groundLevel, zPlane);
  glEnd();
}

//Draws the ground line
void ground()
{
  glColor3f(1.0f, 0.0f, 0.0f); //Red
  glBegin(GL_LINES);
  glVertex3i(0, groundLevel, zPlane);
  glVertex3i(canvas_Width, groundLevel, zPlane);
  glEnd();
}

//Displays the fuel remaining in the top right
void fuelDisplay()
{
  glColor3f(0.0f,0.0f,0.0f); //Black
  glRasterPos2i(canvas_Width - 75, canvas_Height - 20);

  //Wacky casting to get the fuel as a const char * for glut
  std::string fuelString = "Fuel: " + std::to_string(fuel);
  const unsigned char* fuelCharArray = (const unsigned char *)fuelString.c_str();
  glutBitmapString(GLUT_BITMAP_8_BY_13, fuelCharArray);
}

//Callback function for display event
void display_CB()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0); //White
  glClear(GL_COLOR_BUFFER_BIT);


  ship();
  ground();
  landingPad();
  fuelDisplay();

  //If you haven't pressed 'v' or 'i' show the start message
  if(!planetChosen)
  {
    glCallList(1);
  }

  //If you have won show the win message
  if(win)
  {
    glCallList(2);
  }

  glFlush();
}

//Callback for timer event
void timer_CB(int id)
{
  if(id == 0) //Framerate timer
  {
    //The ship is within the bounding box of the landing pad
    if((shipY - shipLength) < landingHeight && shipX < landingWidth)
    {
      grounded = true;
      win = true;
      shipX = landingWidth / 2;
      shipY = groundLevel + shipLength;
      
    }
    //The ship isn't on the ground
    else if(shipY - shipLength > groundLevel)
    {
      if (thrust)
      {
        shipY += 5;
        upForce += 5; //Used to update the position in the gravity calculations
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

//Callback for keyboard event
void keyboard_CB(unsigned char key, int x, int y)
{

  //Pick a planet/moon and set the gravity accordingly
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

  //Change ship position
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

  glGenLists(2); //Allocate the display lists

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