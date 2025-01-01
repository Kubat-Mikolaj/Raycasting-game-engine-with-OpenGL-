/***********************************************************************************************************

******************************************Raycast 3D renderer***********************************************


************************************************************************************************************/

/************************************************************************************************************
 * Library Include Files
************************************************************************************************************/
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/************************************************************************************************************
 * Feature Level Type Definitions
************************************************************************************************************/
float px, py, pdx, pdy, pa; // Player position X and Y
int mapX = 8;
int mapY = 8;
int mapS = 64;

// Map
int map[] = 
{
	1,1,1,1,1,1,1,1,
	1,0,0,0,1,1,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,1,0,0,1,
	1,0,1,0,0,1,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,1,1,1,1,1,1,1,
};

/************************************************************************************************************
 * Feature Macro Definitions
************************************************************************************************************/
#define PI 3.1415925435
#define P2 PI/2
#define P3 3 * PI/2
#define DR 0.0174533 // One degree in radians

/************************************************************************************************************
 * Local Function Prototypes
************************************************************************************************************/
void drawPlayer();
void init();
void display();
void buttons(unsigned char key, int x, int y);
void drawMap2D();
void drawRays2D();
float distance(float ax, float ay, float bx, float by, float angle);

/***********************************************************************************************************/


// Main function
int main(int argc, char** argv)
{ 
 glutInit(&argc, argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 glutInitWindowSize(1024, 512);
 glutCreateWindow("TEST_1");
 init();
 glutDisplayFunc(display);
 glutKeyboardFunc(buttons); // Movement
 glutMainLoop();
 return 0;
}

// Initiate the window
void init()
{
	// Set the background color to gray:
	glClearColor(0.3, 0.3, 0.3, 0.3);
	
	// Set window to 1024 and 512:
	gluOrtho2D(0, 1024, 512, 0);
	
	// set the position of player:
	px = 300;
	py = 300;
	pdx = cos(pa) * 5;
	pdy = sin(pa) * 5;
}

// Player
void drawPlayer()
{
	glColor3f(1,1,0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(px,py);
	glEnd();
	
	
	// PI
	
	glLineWidth(3.0);
	glBegin(GL_LINES);
	glVertex2i(px,py);
	glVertex2i(px + pdx * 5, py + pdy * 5);
	glEnd();
}

// Movement
void buttons(unsigned char key, int x, int y)
{
	if (key == 'a') // LEFT
	{
	  pa -= 0.1;
	  if (pa < 0)
	  {
	    pa += 2*PI;
	  }
	  pdx = cos(pa) * 5;
	  pdy = sin(pa) * 5;
	}

	if (key == 'd') // RIGHT
	{
	  pa += 0.1;
	  if (pa > 2 * PI)
	  {
	    pa -= 2*PI;
	  }
	  pdx = cos(pa) * 5;
	  pdy = sin(pa) * 5;
	
	}
	
	if (key == 'w') // FORWARD
	{
	  px += pdx;
	  py += pdy;
	}
	
	if (key == 's') // BACKWARD
	{
	  px -= pdx;
	  py -= pdy;
	}
	
	glutPostRedisplay();
}

float distance(float ax, float ay, float bx, float by, float angle)
{
	return (sqrt((bx - ax) * (bx - ax)+ (by - ay) * (by - ay)));
}

// The Map
void drawMap2D()
{
  int x,y, xo, yo;

  for (y = 0; y < mapY; y++)
  {
    for (x = 0; x < mapX; x++)
    {
	  if (map[y * mapX + x] == 1)
	  {
	    glColor3f(1,1,1);
	  }
	  else
	  {
	    glColor3f(0,0,0);
	  }
	
	   xo = x * mapS;
	   yo = y * mapS;
	  
	   glBegin(GL_QUADS);
	   glVertex2i(xo        + 1,        yo + 1);
	   glVertex2i(xo        + 1, yo + mapS - 1);
	   glVertex2i(xo + mapS - 1, yo + mapS - 1);
	   glVertex2i(xo + mapS - 1,        yo + 1);
	   glEnd();
    }
  }
}

// Rays
void drawRays2D()
{
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo, ca, lineH, lineO;
	float aTan, nTan, disT;
	
	ra = pa - DR*30; // Set the rays angle to players angle
	
	if (ra < 0)
	{
	  ra += 2*PI;	
	}
	if (ra > 2*PI)
	{
	  ra -= 2*PI;
	}
	
	for (r = 0; r < 60; r++)
	{
	  // Check horizontal lines
	  
	  dof = 0;
	  float disH = 100000;
	  float hx, hy;
	  
	  hx = px;
	  hy = py;
	  
	  aTan = -1/tan(ra); // Tangens
	  if (ra > PI) // Check if ray is looking up
	  {
	    ry = (((int)py >> 6) << 6) - 0.0001;
	    rx = (py - ry) * aTan + px;
	    yo = -64;
	    xo = -yo * aTan;
	  }
	  
	  if (ra < PI) // Check if ray is looking down
	  {
	    ry = (((int)py >> 6) << 6) + 64;
	    rx = (py - ry) * aTan + px;
	    yo = 64;
	    xo = -yo * aTan;
	  }
	  
	  if (ra == 0 || ra == PI) // Check if ray is looking left or right
	  {
	  	rx = px;
	  	ry = py;
	  	dof = 8;
	  	
	  }
	  
	  while (dof < 8)
	  {
	  	mx = (int)(rx) >> 6;
	  	my = (int)(ry) >> 6;
	  	
	  	mp = my * mapX + mx;
	  	if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
	    {
	  	  hx = rx;
		  hy = ry;
		  disH = distance(px,py,hx,hy,ra);
		  dof = 8; // Hit wall
	    }
	  	else // Draw next line
	    {
	  	  rx += xo;
	  	  ry += yo;
	  	  dof += 1;
	  	}
	  }
	  
	 // Check vertical lines
	 
	  dof = 0;
	  float disV = 100000;
	  float vx, vy;
	  
	  vx = px;
	  vy = py;
	  nTan = -tan(ra); // Tangens
	  if (ra > P2 && ra < P3) // Check if ray is looking left
	  {
	    rx = (((int)px >> 6) << 6) - 0.0001;
	    ry = (px - rx) * nTan + py;
	    xo = -64;
	    yo = -xo * nTan;
	  }
	  
	  if (ra < P2 || ra > P3) // Check if ray is looking right
	  {
	    rx = (((int)px >> 6) << 6) + 64;
	    ry = (px - rx) * nTan + py;
	    xo = 64;
	    yo = -xo * nTan;
	  }
	  
	  if (ra == 0 || ra == PI) // Check if ray is looking up or down
	  {
	  	rx = px;
	  	ry = py;
	  	dof = 8;
	  	
	  }
	  
	  while (dof < 8)
	  {
	  	mx = (int)(rx) >> 6;
	  	my = (int)(ry) >> 6;
	  	
	  	mp = my * mapX + mx;
	  	
	  	if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
	    {
	      vx = rx;
		  vy = ry;
		  disV = distance(px,py,vx,vy,ra);
	  	  dof = 8; // Hit wall
	    }
	  	else // Draw next line
	    {
	  	  rx += xo;
	  	  ry += yo;
	  	  dof += 1;
	  	}
	  }
	  
	  if (disV < disH)
	  {
	  	rx = vx;
	  	ry = vy;
	  	disT = disV;
	  	glColor3f(0.9,0,12);
	  }
	  
	  if (disV > disH)
	  {
	  	rx = hx;
	  	ry = hy;
	  	disT = disH;
	  	glColor3f(0.6,0,11);
	  }
	  

	 glLineWidth(3);
	 glBegin(GL_LINES);
	 glVertex2i(px,py);
	 glVertex2i(rx,ry);
	 glEnd();
	 
	 // Draw 3D Wall
	 
	 ca = pa - ra;
	 if (ca < 0)
	 {
	   ca += 2*PI;
	 }
	 
	 if (ca > 2*PI)
	 {
	   ca -= 2*PI;
	 }
	 
	 disT = disT * cos(ca); // fix fisheye effect
	 
	 lineH = (mapS * 320)/disT;
	 if (lineH > 320)
	 {
	  lineH = 320; // Line height
	 }
	 
	 lineO = 160 - lineH/2; // Line offset
	 
	 glLineWidth(8);
	 glBegin(GL_LINES);
	 glVertex2i( r * 8 + 530, lineO);
	 glVertex2i( r * 8 + 530, lineH + lineO);
	 glEnd();
	 
	 
	 
	 ra += DR; // Increase ray by 1 degree
	 if (ra < 0)
	 {
	   ra += 2*PI;
	 }
	 
	 if (ra > 2*PI)
	 {
	   ra -= 2*PI;
	 }
	 
	  
	}
}

// The display
void display()
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 drawMap2D();
 drawPlayer();
 drawRays2D();
 glutSwapBuffers();
}


	
	
	
	



