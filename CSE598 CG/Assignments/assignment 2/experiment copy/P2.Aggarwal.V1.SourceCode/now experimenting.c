	
/* Assignment 2
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

Assignment no 2, completed by Rajat Aggarwal, graduate student

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
#include <time.h>

/* Include the GLUT library. This file (glut.h) contains gl.h and glu.h */
/* See how this is "glut.h" as opposed to <glut.h> - this is due to the fact that you need
glut in your local directory. */
#include "glut.h"

// Structure for hierarchical StickMan in Assignment 2, this is only a starting point for the hierarchical structure
// You may need other items here or less items
typedef struct stickManNode
{
	GLfloat m[16];
	void(*f)();
	struct stickManNode *sibling;
	struct stickManNode *child;
}stickNode;

stickNode root, neck, la, ra, lt, head, ll, rl, ls, rs;
int temp = 0;
clock_t diff;


/* These are the variables used to define the images being used in the example, you may need to redefine your own widths and heights */
unsigned char *image;
int imageWidth = 500;
int imageHeight = 375;
int drawSquareflag = 0, drawCircleflag = 0, load1 = 0, loadtest = 0, clickx, clicky;
void bgimage(void); //function to draw image when clicked on menu
void flipImag(char *, char *);
char *flippedImage;
/* We will have lots of angle calculations in class, this converts degrees to radians PI/180 */
const float DEG2RAD = 3.14159 / 180.;
int clickxqueue[3] = { 0, 0, 0 }, clickyqueue[3] = { 0, 0, 0 };
int count = 0, state=0, increasing=1;
int latheta[4] = { 110, 150, 190, 230 }, /*lltheta[4] = {}, rltheta[4] = {},*/ lstheta[4] = {-45,-30,-60,0}, /*lstheta[4] = {135, 150, 120, 110}*/, rstheta[4] = { 0, 20, -40, -80 };
clock_t start;
// You may have lots of global variables to control states and animation, these would go below here

//You are REQUIRED to modularize your programs!  Here you see I've created a function stub to draw a unit circle outline (NOT FILLED)

void drawAxes()
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(-1.0, 0.0);
	glVertex2f(1.0, 0.0);
	glEnd();


	glBegin(GL_LINE_LOOP);
	glVertex2f(0.0, 1.0);
	glVertex2f(0.0, -1.0);
	glEnd();
}




void drawCircle()
{
	//as1 : draw a circle with 360 points of polygon

	glBegin(GL_LINE_LOOP);
	glColor3f(1.0, 0.0, 0.0);
	double x = 1.0, y = 0.0;
	double theta;
	for (int i = 0; i < 360; i++)
	{
		theta = i * DEG2RAD;
		x = 1 * cos(theta); //radius = 1
		y = 1 * sin(theta);
		glVertex2f(x, y);

	}

	glEnd();

}

//An example function which draws a filled unit square (1x1) 
void drawSquare()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(-.5, -.5);
	glVertex2f(.5, -.5);
	glVertex2f(.5, .5);
	glVertex2f(-.5, .5);
	glEnd();

}

//An example function which draws a unit line (length 1)
void drawLine()
{
	//glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 1.0);
	glEnd();
}


void rootfunc()
{
		return;
}

void neckfunc()
{
	glColor3d(1.0, 0.0, 0.0);
	glScalef(0.25, 0.25, 1); //for neck
	drawLine();
	/*printf("\nNeckfunc");
	for (int k = 0; k < 16; k++)
		printf("neck.m[%d]=%f  ", k, neck.m[k]);
	*/
}

void headfunc()
{
	glScalef(4, 4, 1);
	glTranslatef(0.0, 0.5, 0);
	glScalef(0.25, 0.25, 1);
	glColor3f(0.0, 0.0, 1.0);
	//drawAxes();
	drawCircle();
	printf("\nheadfunc");
}

void lafunc()
{
	glRotatef(latheta[state], 0, 0, 1);
	glScalef(0.5, 0.5, 1);
	drawLine();
	
	}

void rafunc()
{

	glRotatef(-1*latheta[state], 0, 0, 1);
	glScalef(0.5, 0.5, 1);
	drawLine();
	
	}

void ltfunc()
{
	glScalef(0.5, 0.5, 1);
	glTranslatef(0, -0.5, 0);
	drawLine();
	//glScalef(2, 2, 1);

}

void llfunc()
{
	glColor3f(0.0, 0.0, 1.0);
	glRotatef(latheta[state], 0, 0, 1);
	glScalef(0.707, 0.707, 1);
	drawLine(); 
	glScalef(1.41, 1.41, 1);
	printf("latheta=%d\n", latheta[state]);
}

void lsfunc()
{
	////glLoadIdentity();
	glTranslatef(-0.5, -0.5, 0);
	glRotatef(lstheta[state], 0, 0, 1);
	glColor3f(0.0, 0.0, 1.0);
	//glScalef(1.41*2, 1.41*2, 1);
	drawLine();
	printf("lstheta=%d\n",lstheta[state]);
	getch();

	//glRotatef(135, 0, 0, 1);
	//glScalef(0.707, 0.707, 1);
	//drawLine();
	//glGetFloatv(GL_MODELVIEW_MATRIX, ll.m);
	//ll.f = llfunc;
	//ll.sibling = &rl;
	//ll.child = &ls;

	//glPushMatrix(); //move to left shin
	//glTranslatef(-0.5, -0.5, 0);
	//glRotatef(lstheta[state], 0, 0, 1);
	//drawLine();




}

void rlfunc()
{
	glColor3f(0.0, 1.0, 0.0);
	glRotatef(-1 * latheta[state], 0, 0, 1);
	glScalef(0.707, 0.707, 1);
	drawLine();

	
	/*glColor3f(0, 1.0, 0.0);
	glScalef(0.707, 0.707, 1);
	glRotatef(-1 * latheta[state], 0, 0, 1);
	drawLine();
	*/
	}

void rsfunc()
{
	glColor3f(0.0, 1.0, 0.0);
	glScalef(1.41, 1.41, 1);
	glRotatef(rstheta[state], 0, 0, 1);
	glTranslatef(-0.5, -0.5, 0);
	glRotatef(rstheta[state], 0, 0, 1);
	
	drawLine();


	/*glColor3f(0, 1.0, 0.5);
	glRotatef(rstheta[state], 0, 0, 1);
	glScalef(1.41, 1.41, 1);
	glTranslatef(0.5, -0.5, 0);
	
	drawLine();
	*/
}
void traverse(stickNode *node)
{
	if (node == NULL)
	{
		printf("returning");
		return;
	}
	glPushMatrix();
	//glMultMatrixf(node->m);
	node->f();
	printf("node->name %x", &node);
	if (node->child != NULL)
		traverse(node->child);
	glPopMatrix();
	if (node->sibling != NULL)
		traverse(node->sibling);
}	

//This function will be completed in Assignment 2, you will used drawCircle, drawLine as part of this
void renderStickMan()
{
	glPushMatrix(); //for identity
	//translating root node to point of click.
	/*double ty = -1;
	double tx = -1;
	glTranslatef(tx, ty, 0.0);
	glPushMatrix();
	*/

	glColor3f(0.0, 0.0, 1.0);
	drawAxes();
	glGetFloatv(GL_MODELVIEW_MATRIX, root.m);
	root.f = rootfunc; /* will call rootfunc() */
	root.sibling = NULL;
	root.child = &neck;

	glScalef(0.25, 0.25, 1); //for neck
	glGetFloatv(GL_MODELVIEW_MATRIX, neck.m);
	drawLine();
	
	neck.f = neckfunc; 
	neck.sibling = &la;
	neck.child = &head;

	

	glPushMatrix(); //push and move towards head
	glScalef(4, 4, 1);
	glTranslatef(0.0, 1.0, 0);
	glScalef(0.25, 0.25, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, head.m);
	/*for (int k = 0; k < 16; k++)
		printf("head.m[%d]=%f  ", k, head.m[k]);
*/
	head.f = headfunc;
	head.sibling = NULL;
	head.child = NULL;

	glColor3f(0.0, 0.0, 1.0);
	//drawAxes();
	drawCircle();
	glPopMatrix(); //move back to neck 
	glPopMatrix(); //move back to root node


	//int ltheta[4] = { 90, 135, 180, 225 }, state = 0;
	glPushMatrix(); //move to left arm
	glRotatef(90, 0, 0, 1);
	glScalef(0.5, 0.5, 1);
	drawLine();
	glGetFloatv(GL_MODELVIEW_MATRIX, la.m);
	la.f = lafunc;
	la.sibling = &ra;
	la.child = NULL;

	glPopMatrix(); //move back to root node

	
	glPushMatrix(); //move to right arm
	glRotatef(-90, 0, 0, 1);
	glScalef(0.5, 0.5, 1);
	drawLine();
	glGetFloatv(GL_MODELVIEW_MATRIX, ra.m);
	ra.f = rafunc;
	ra.sibling = &lt;
	ra.child = NULL;

	glPopMatrix(); //move back to root node
	glPushMatrix(); //move to torso

	glScalef(0.5, 0.5, 1);
	glTranslatef(0, -0.5, 0);
	drawLine();
	glGetFloatv(GL_MODELVIEW_MATRIX, lt.m);
	lt.f = ltfunc;
	lt.sibling = NULL;
	lt.child = &ll;



	glPushMatrix(); //move to left leg
	//glScalef(2, 2, 1);
	glRotatef(135, 0, 0, 1);
	glScalef(0.707, 0.707, 1);
	drawLine();
	glGetFloatv(GL_MODELVIEW_MATRIX, ll.m);
	ll.f = llfunc;
	ll.sibling = &rl;
	ll.child = &ls;

	glPushMatrix(); //move to left shin
	glTranslatef(-0.5, -0.5, 0);
	glRotatef(lstheta[state], 0, 0, 1);
	drawLine();
	glGetFloatv(GL_MODELVIEW_MATRIX, ls.m);
	ls.f = lsfunc;
	ls.sibling = NULL;
	ls.child =  NULL;

	glPopMatrix(); //move to left leg
	glPopMatrix(); //move to lower torso

	glPushMatrix(); //move to right leg
	glScalef(0.707, 0.707, 1);
	glRotatef(225, 0, 0, 1);
	drawLine();
	glGetFloatv(GL_MODELVIEW_MATRIX, rl.m);
	rl.f = rlfunc;
	rl.sibling = NULL;
	rl.child = &rs;

	glPushMatrix(); //move to right shin
	glRotatef(-45, 0, 0, 1);
	glScalef(1.41, 1.41, 1);
	glTranslatef(0.5, -0.5, 0);
	glRotatef(180, 0, 0, 1);
	drawLine();
	glGetFloatv(GL_MODELVIEW_MATRIX, rs.m);
	rs.f = rsfunc;
	rs.sibling = NULL;
	rs.child = NULL;
	printf("Blah balh!!");

	glPopMatrix(); //move to right leg

	glPopMatrix(); //move to torso

	glPopMatrix(); //move to root node
	
}




//The rountine display() gets called each time the window is redrawn.  Note that this is linked to the command
//glutDisplayFunc(display); which sets this as the display callback
void display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* the window will correspond to these world coorinates - this means that the window will have a coordinate system that goes from -1 to 1 in the x and y direction */
	/* what effect do you think this will have on drawing a square if the window size is not square? */
	//gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	/* identify the modeling and viewing matrix that can be modified from here on */
	/* we leave the routine in this mode in case we want to move the object around */
	/* or specify the camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT);

	//The glViewport command defines the window that we will be drawing in, here it is the entire screen space
	glViewport(0, 0, imageWidth, imageHeight);

	//In order to draw the image I read in, I am going to draw from the raster position starting at (-1, -1)
	glRasterPos2i(-1, -1);  //Why use -1, -1 ? 

	glDrawPixels(imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, image);

	//Once I'm done drawing the background image now I want to set up a new drawing area on the screen
	//specifically, I want to draw on top of the index card
	//glViewport(205,103,155,117);

	//code to store three different clickx and clicky values and render 3 stickman
	//draw stickman nodes.
	//if (count++==0)
	//renderStickMan();
	for (int i = 0; i < 3 && i < count; i = i + 1)
	{

		glViewport(clickxqueue[i] - 155 / 2, ((clickyqueue[i] - ((117 / 2)*0.75))), 155, 117);
		//printf("clickxqueue[%d] =%d clickyqueue[%d]=%d \n", i, clickxqueue[i], i, clickyqueue[i]);
		//printf("Count= %d", count);
	   glLoadIdentity();
	   traverse(&root);
	   

	}
	
	//traverse(&root);
	//traverse(&root);
	/* ready to draw now! forces buffered OGL commands to execute */
	glFlush();
}

//If you're using a .png image like I provided you should be fine using this, my images were converted to .png using GIMP
void loadPNG(char* filename, int width, int height, int bitdepth)
{
	image = stbi_load(filename, &width, &height, &bitdepth, 0);

}

/* Initialize states -- called before */
void init()
{
	loadPNG("img1.png", imageWidth, imageHeight, 24);
	flipImag(&image, &flippedImage);
	/* set background clear color to white */
	glClearColor(1.0, 1.0, 1.0, 0.0);
	start = clock();
	renderStickMan();

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
	glLoadIdentity();
	//traverse(&root);
	diff = clock() -start; //reference : http://stackoverflow.com/questions/459691/best-timing-method-in-c
	printf("Diff=%l", diff);

	if (increasing == 1) state++;	else state--;
	if (state == 3) increasing = 0;
	if (state == 0) increasing = 1;
	
	
	Sleep(1000);
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
		exit(0);
		break;
	case 1:
		drawSquareflag = 1;
		drawCircleflag = 0;
		break;
	case 2:
		drawCircleflag = 1;
		drawSquareflag = 0;
		break;
	case 3: // we are not changing resolution as both have the defined resoulution
		load1 = 1;
		loadtest = 0;
		bgimage();
		break;
	case 4:
		loadtest = 1;
		load1 = 0;
		bgimage();
		break;
	case 5:
		flipImag(&image, &flippedImage);
		break;

	}
	// Almost any menu selection requires a redraw
	glutPostRedisplay();
}

//Here I will initialize the menu, for Assignment 1 you will need to create other menu entries with sub-entries
//Specifically, you'll be creating a menu entry "Images" with sub entries "1" and "2" as well as a menu entry "Draw" with sub entries "square" and "circle"
int BuildPopupMenu(void)
{
	int menu, draw, images;



	images = glutCreateMenu(SelectFromMenu);
	glutAddMenuEntry("1", 3);
	glutAddMenuEntry("2", 4);

	menu = glutCreateMenu(SelectFromMenu);  //This links a callback function "SelectFromMenu", and that link is saved as the integer menu which is then
	glutAddMenuEntry("Exit demo", 0); //This adds an entry into the menu, if you right click, you'll see "Exit Demo" and this will returned to be linked to the right mouse button
	//send the value MENU_EXIT to the callback function
	glutAddSubMenu("Images", images);
	return menu;
}

//ALL STUDENTS: You need to fill out this function for getting the image color
//HINT: Use the img2.jpg image to help you debug
void getImageColor(char* image, int pixel_x, int pixel_y, char *RED, char *GREEN, char *BLUE)
{
	//because opengl renders the image in oppposite to coordinate axes we subtract imageheight from y mod imageheight
	clicky = (imageHeight - clicky);
	RED = image[3 * (clicky * imageWidth + clickx)];
	GREEN = image[3 * (clicky * imageWidth + clickx) + 1];
	BLUE = image[3 * (clicky * imageWidth + clickx) + 2];
	printf("\nScreen click, R=%u, G=%u, B=%u", (unsigned char)RED, (unsigned char)GREEN, (unsigned char)BLUE);

}

//GRADUATE STUDENTS: You need to fill out this function to flip the image
void flipImag(char *image2, char *flippedImage)
{
	/*flippedImage = (char *)malloc(strlen(image) + 1);*/
	char temp;
	for (int i = 0; i < imageWidth * 3; i++)
	{
		for (int j = 0; j < imageHeight / 2; j++) //swap column positions j with imageHeight-j for a particular column
		{
			temp = image[((imageHeight - 1) * imageWidth * 3) - (j*imageWidth * 3) + i];
			image[((imageHeight - 1) * imageWidth * 3) - (j*imageWidth * 3) + i] = image[(j*imageWidth * 3) + i];
			image[(j*imageWidth * 3) + i] = temp;
			//printf("	\n %d %d", j+i, image[j]);
		}

		//	flippedImage[i] = image2[((imageWidth * imageHeight - 1)) - i];
		//	printf("\n%c", flippedImage[i]);		//	printf("%u\n", (unsigned char)image2[((imageWidth - 1)*(imageHeight - 1)) - i]);
	}

}
//Here is the mouse callback function, you will need to use the x, y coordinate intelligently to get the color of the pixel that is clicked on.
//You must have a printf statement here as well that prints out the coordinate and the color from the background image at the location
void myMouse(int button, int state, int x, int y)
{
	//You need to think about the x and y coordinate that you get here and if they are what you expect
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		//code to shift elements of queue one by one

		if (state == GLUT_DOWN)
		{
			for (int i = 2; i >= 0; i = i - 1)
			{
				clickxqueue[i] = clickxqueue[i - 1];
				clickyqueue[i] = clickyqueue[i - 1];
			}

			clickxqueue[0] = x;
			clickyqueue[0] = imageHeight - 1 - y;
			count = count + 1;
			printf("\nCount : %d", count);
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


	/* init some states */
	init();
	//renderStickMan();

	/* create a callback routine for when the mouse is clicked */
	glutMouseFunc(myMouse);


	
	/* create a callback routine for when the program is idle */
	glutIdleFunc(animation);



	/* create a callback routine for (re-)display */
	glutDisplayFunc(display);

	BuildPopupMenu();  //This is going to be a function you write to initialize the menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);  //This attaches the menu to the right mouse click  button

	/* entering the event loop */
	glutMainLoop();

	/* code here will not be executed */
	}

void bgimage()
{
	if (load1 == 1)
	{
		loadPNG("img1.png", imageWidth, imageHeight, 24);
		flipImag(&image, &flippedImage);
		printf("img1.png loaded");
	}
	if (loadtest == 1)
	{
		loadPNG("img2.png", imageWidth, imageHeight, 24);
		flipImag(&image, &flippedImage);
		printf("img2.png loaded");
	}
}



