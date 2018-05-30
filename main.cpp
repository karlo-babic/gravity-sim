//g++ main.cpp -w -lGL -lGLU -lglut -o sol

#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

#include "objects.cpp"
#include "draw.cpp"

using namespace std;


//Screen Constants
const int SCREEN_WIDTH = 768;
const int SCREEN_HEIGHT = 768;
const int SCREEN_FPS = 30;



vector<Obj> obj;
int stationarySun;
int whichSun = 0;
int objectPreviewSize;
bool inertia;
bool border;
extern float scale;
extern int initialSpin;
extern bool collisionON;
extern bool fragment;
extern int trace;

extern int loop;




bool initGL()
{
  //Initialize Projection Matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //Initialize Modelview Matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //Initialize clear color
  glClearColor(0.f, 0.f, 0.f, 1.f);

  //Set scale
  gluOrtho2D(-50, 50, -50, 50);

  //Check for error
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
    {
      cout << "Error initializing OpenGL!\n" << gluErrorString( error );
      return false;
    }

  return true;
}



void update()
{
  int objSize = obj.size();

  if (inertia)
    {
      for (int i=0; i<objSize; i++) obj[i].calcInertPosition();
    }

  for (int i=0; i<objSize; i++)
    {
      for (int j=i+1; j<objSize; j++) obj[i].calcGravityPosition(obj[j]);
    }

  int whichRemove;
  if (collisionON)
    {
      for (int i=0; i<objSize; i++)
	{
	  for (int j=i+1; j<objSize; j++)
	    {
	      if (obj[i].radius >= obj[j].radius)
		{
		  int remove = obj[i].collision(obj[j]);
		  whichRemove = 2 * remove;
		}
	      else
		{
		  int remove = obj[j].collision(obj[i]);
		  whichRemove = 1 * remove;
		}
	      if (whichRemove)
		{
		  int n1, n2;
		  if (whichRemove == 1)
		    {
		      n1 = i;
		      n2 = j;
		    }
		  else if (whichRemove == 2)
		    {
		      n1 = j;
		      n2 = i;
		    }
		  if (fragment && obj[n1].radius > scale/10.)
		    {
		      obj[n1].new_position_x = obj[n1].position_x;
		      obj[n1].new_position_y = obj[n1].position_y;
		    }
		  else
		    {
		      obj.erase(obj.begin() + n1);
		      //cout << "erased!\n";
		      objSize--;
		      if (n1 == whichSun) whichSun = n2;
		      n1--;
		      if (n2 > n1) n2--;
		      if (whichSun > n1) whichSun--;
		      break;
		    }
		  if (whichRemove == 1)
		    {
		      i = n1;
		      j = n2;
		    }
		  else if (whichRemove == 2)
		    {
		      i = n2;
		      j = n1;
		    }
		}
	    }
	}
    }

  for (int i=0; i<objSize; i++)
    {
      if (border)
	{
	  if (obj[i].new_position_x > 50*scale)
	    {
	      obj[i].new_position_x = 50*scale;
	      obj[i].position_x = 50*scale;
	    }
	  else if (obj[i].new_position_x < -50*scale)
	    {
	      obj[i].new_position_x = -50*scale;
	      obj[i].position_x = -50*scale;
	    }
	  if (obj[i].new_position_y > 50*scale)
	    {
	      obj[i].new_position_y = 50*scale;
	      obj[i].position_y = 50*scale;
	    }
	  else if (obj[i].new_position_y < -50*scale)
	    {
	      obj[i].new_position_y = -50*scale;
	      obj[i].position_y = -50*scale;
	    }
	}
      if (obj[i].outOfBounds())    // delete object if out of bounds
	{
	  //cout << "\n---------------- out of bounds\n";
	  obj.erase(obj.begin() + i);
	  objSize--;
	  i--;
	  if (whichSun > i) whichSun--;
	}
    }

  for (int i=0; i<obj.size(); i++)
    {
      if (!(stationarySun && i == whichSun))
	{
	  obj[i].prevPosition_x = obj[i].position_x;
	  obj[i].prevPosition_y = obj[i].position_y;

	  obj[i].position_x = obj[i].new_position_x;
	  obj[i].position_y = obj[i].new_position_y;

	  if (trace) obj[i].makeTrace();
	}
      else
	{
	  obj[i].position_x = obj[i].prevPosition_x;
	  obj[i].position_y = obj[i].prevPosition_y;
	}
    }

  /*for (int i=0; i<objSize; i++)
    {
      if (i == whichSun) cout << "SUN ";
      cout << obj[i].mass << "  |  ";
    }
  cout << "\n";*/
}




void render()
{
  //Clear buffer
  glClear(GL_COLOR_BUFFER_BIT);

  //Render
  for (int i=0; i<obj.size(); i++)
    {
      glBegin(GL_POLYGON);
      {
	drawCircle(obj[i].position_x / scale, obj[i].position_y / scale, obj[i].radius * objectPreviewSize / scale);
      }
      glEnd();

      if (trace && !(stationarySun && i == whichSun))
	{
	  glLineWidth(obj[i].radius * objectPreviewSize / scale);
	  glBegin(GL_LINES);
	  {
	    obj[i].drawTrace();
	  }
	  glEnd();
	}
    }

  //Update screen
  glutSwapBuffers();
}



void mainLoop(int val)
{
  update();
  render();
  loop++;

  glutTimerFunc(1000 / SCREEN_FPS, mainLoop, val);
}



void config()
{
  ifstream configFile;
  configFile.open("configs/config");

  int numberOfObjects;
  int avgSizeOfObjects;
  float sizeOfSun;
  int distanceFromSun;

  string input;
  do{ configFile >> input; }while(input != ":");
  configFile >> scale;
  do{ configFile >> input; }while(input != ":");
  configFile >> border;
  do{ configFile >> input; }while(input != ":");
  configFile >> sizeOfSun;
  do{ configFile >> input; }while(input != ":");
  configFile >> stationarySun;
  do{ configFile >> input; }while(input != ":");
  configFile >> numberOfObjects;
  do{ configFile >> input; }while(input != ":");
  configFile >> avgSizeOfObjects;
  do{ configFile >> input; }while(input != ":");
  configFile >> objectPreviewSize;
  do{ configFile >> input; }while(input != ":");
  configFile >> initialSpin;
  do{ configFile >> input; }while(input != ":");
  configFile >> distanceFromSun;
  do{ configFile >> input; }while(input != ":");
  configFile >> inertia;
  do{ configFile >> input; }while(input != ":");
  configFile >> collisionON;
  do{ configFile >> input; }while(input != ":");
  configFile >> fragment;
  do{ configFile >> input; }while(input != ":");
  configFile >> trace;

  obj.push_back(Obj(0, 0, 0, 0, sizeOfSun));    // Sun

  srand(time(NULL));
  for (int i=1; i<=numberOfObjects; i++)
    {
      obj.push_back(Obj(avgSizeOfObjects, distanceFromSun));    // planets
    }

  for (int i=0; i<obj.size(); i++)
    {
      obj[i].new_position_x = obj[i].position_x;
      obj[i].new_position_y = obj[i].position_y;
    }

  configFile.close();
}






int main(int argc, char* args[])
{
  config();

  //Initialize FreeGLUT
  glutInit(&argc, args);

  //Create OpenGL 2.1 context
  glutInitContextVersion(2, 1);

  //Create Double Buffered Window
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutCreateWindow("Sol");

  //Do post window/context creation initialization
  if (!initGL())
    {
      cout << "Unable to initialize graphics library!\n";
      return 1;
    }

  //Set rendering function
  glutDisplayFunc(render);

  //Set main loop
  glutTimerFunc(1000 / SCREEN_FPS, mainLoop, 0);

  //Start GLUT main loop
  glutMainLoop();

  return 0;
}
