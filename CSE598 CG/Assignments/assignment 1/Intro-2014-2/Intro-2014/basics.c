
/* Assignment 1 
 * 
 * A basic, first OpenGL program.
 * 
 * This program illustrates how you open an window, specify its position and size,
 * and draw something in the window.
 *
 * Also illustrated: the state-machine operation of OGL. 
 *
 * When you resize the window so it is not square, the box changes shape. This might
 * not be desired -- we will look into fixing this later. (Keep in mind, this has
 * something to do with the aspect ratio.)
 *
 * Try changing the size of gluOrtho2D -- see in code below -- and you'll see how the
 * relative size of the box changes within the window.
 *
 *
 * Dianne Hansford, August 2004
 * Modified by Ross Maciejewski August 2011
 */

#include <windows.h>
#include <ole2.h>
#include <ocidl.h>
#include <olectl.h>
#include <gl/gl.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/* Include the GLUT library. This file (glut.h) contains gl.h and glu.h */
/* See how this is "glut.h" as opposed to <glut.h> - this is due to the fact that you need
   glut in your local directory. */
#include "glut.h"

// Structure for hierarchical StickMan in Assignment 2, this is only a starting point for the hierarchical structure
// You may need other items here or less items
typedef struct stickManNode
{
   GLfloat m[16];
   void (*f)();
   struct stickManNode *sibling;
   struct stickManNode *child;
} stickManNode;


/* These are the variables used to define the images being used in the example, you may need to redefine your own widths and heights */
unsigned char *image;
int imageWidth = 500;
int imageHeight = 375;

/* We will have lots of angle calculations in class, this converts degrees to radians PI/180 */
const float DEG2RAD = 3.14159/180.;

// You may have lots of global variables to control states and animation, these would go below here

//You are REQUIRED to modularize your programs!  Here you see I've created a function stub to draw a unit circle outline (NOT FILLED)
void drawCircle()
{
	glBegin(GL_POLYGON);
		double x = 1.0, y = 0.0;
		double theta;
		for (int i = 0; i < 360; i++)
		{
			theta = i * DEG2RAD;
			x = x * cos(theta);
			y = y * sin(theta);
			glVertex2f(x, y);
		}
	glEnd();
}

void drawSquare()
{
	glBegin(GL_POLYGON);
	glVertex2f(-.5, -.5);
	glVertex2f(.5, -.5);
	glVertex2f(.5, .5);
	glVertex2f(-.5, .5);
	glEnd();

}

//An example function which draws a filled unit square (1x1) 


//An example function which draws a unit line (length 1)
void drawLine()
{
	glBegin(GL_LINES);
		glVertex2f(0,0);
		glVertex2f(0,1);
	glEnd();
}

//This function will be completed in Assignment 2, you will used drawCircle, drawLine as part of this
void renderStickMan()
{
}

//The rountine display() gets called each time the window is redrawn.  Note that this is linked to the command
//glutDisplayFunc(display); which sets this as the display callback
void display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* the window will correspond to these world coorinates - this means that the window will have a coordinate system that goes from -1 to 1 in the x and y direction */
	/* what effect do you think this will have on drawing a square if the window size is not square? */
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	/* identify the modeling and viewing matrix that can be modified from here on */
	/* we leave the routine in this mode in case we want to move the object around */
	/* or specify the camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT);

	//The glViewport command defines the window that we will be drawing in, here it is the entire screen space
	glViewport(0,0,imageWidth,imageHeight);

	//In order to draw the image I read in, I am going to draw from the raster position starting at (-1, -1)
	glRasterPos2i(-1,-1);  //Why use -1, -1 ? 
	glDrawPixels(imageWidth,imageHeight,GL_RGB, GL_UNSIGNED_BYTE, image);

	//Once I'm done drawing the background image now I want to set up a new drawing area on the screen
	//specifically, I want to draw on top of the index card
	glViewport(205,103,155,117);
	glColor3f(1.0, 0.0, 0.0);
	drawSquare(1);
	printf("hello");
	/* ready to draw now! forces buffered OGL commands to execute */
	glFlush();
}

//If you're using a .png image like I provided you should be fine using this, my images were converted to .png using GIMP
void loadPNG(char* filename,  int width, int height, int bitdepth)
{
	image = stbi_load(filename, &width, &height, &bitdepth, 0);
}

/* Initialize states -- called before */
void init()
{
	loadPNG("1.png",  imageWidth, imageHeight, 24);

	/* set background clear color to white */
	glClearColor(1.0, 1.0, 1.0, 0.0);

	/* identify the projection matrix that we would like to alter */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* the window will correspond to these world coorinates */
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	/* identify the modeling and viewing matrix that can be modified from here on */
	/* we leave the routine in this mode in case we want to move the object around */
	/* or specify the camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void animation()
{
	//If I want things to move on their own without pressing a button, I need to set up an idle callback function
	//If you look further in the code, you'll see that this function is linked to the glutIdleFunc(animation);


	//This command then forces the program to go to the display function (or whatever function you linked to glutDisplayFunc)
	//Thus, you use this function to update global variables that can cause animation in the rendering
	glutPostRedisplay();
}

//Here is my menu callback function
void SelectFromMenu(int idCommand)
{
  switch (idCommand)
    {
    case 0:
      exit (0);
      break;
    }
  // Almost any menu selection requires a redraw
  glutPostRedisplay();
}

//Here I will initialize the menu, for Assignment 1 you will need to create other menu entries with sub-entries
//Specifically, you'll be creating a menu entry "Images" with sub entries "1" and "2" as well as a menu entry "Draw" with sub entries "square" and "circle"
int BuildPopupMenu (void)
{
  int menu;
  menu = glutCreateMenu (SelectFromMenu);  //This links a callback function "SelectFromMenu", and that link is saved as the integer menu which is then
                                           //returned to be linked to the right mouse button
  glutAddMenuEntry ("Exit demo", 0); //This adds an entry into the menu, if you right click, you'll see "Exit Demo" and this will
                                             //send the value MENU_EXIT to the callback function
 return menu;
}

//ALL STUDENTS: You need to fill out this function for getting the image color
//HINT: Use the test.png image to help you debug
void getImageColor(char* image, int pixel_x, int pixel_y, char *RED, char *GREEN, char *BLUE)
{
	//*RED = image[??? - You fill out a formula here];
	//*GREEN = image[];
	//*BLUE = image[];
}

//GRADUATE STUDENTS: You need to fill out this function to flip the image
void flipImag(char *image, char *flippedImage)
{
}

//Here is the mouse callback function, you will need to use the x, y coordinate intelligently to get the color of the pixel that is clicked on.
//You must have a printf statement here as well that prints out the coordinate and the color from the background image at the location
void myMouse(int button, int state, int x, int y)
{
	//You need to think about the x and y coordinate that you get here and if they are what you expect
	switch (button)
	{
	    case GLUT_LEFT_BUTTON:

		    if(state == GLUT_DOWN)
			{
				//Do something (this is just example if/else statements and not intended to demonstrate anything)
			}
		break;
	}
}


/* The main program */
int main(int argc, char** argv)
{

	/* create a window, position it, and name it */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(imageWidth, imageHeight);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Assignment - 1");

	/* create a callback routine for when the mouse is clicked */
	glutMouseFunc(myMouse);


	/* create a callback routine for when the program is idle */
	glutIdleFunc(animation);

	/* create a callback routine for (re-)display */
	glutDisplayFunc(display);

	BuildPopupMenu ();  //This is going to be a function you write to initialize the menu
    glutAttachMenu (GLUT_RIGHT_BUTTON);  //This attaches the menu to the right mouse click  button

	/* init some states */
	init();

	/* entering the event loop */
	glutMainLoop();

	/* code here will not be executed */
}

