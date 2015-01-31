
/* Assignment 2
completed by Rajat Aggarwal, graduate student
Email ID : raaggar1@asu.edu
ASU ID : 1207673359

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
#include <time.h>
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

stickNode root, neck, la, ra, lt, head, ll, rl, ls, rs; //created instances for all parts, starting with root - the joint with torso, hands and neck
int time1[3] = { 0 }; //different time for different stickman , when created after clicking on screen.
float seedx, seedy; //selected seed in box.
float speed = 0.02; //default speed,
long int newtime, oldtime; //time values based on which 2s delay is made after subtracting delay in other unneccessary things.
double slope, theta; //for box 
unsigned char *image, *flippedImage; //for background image
int imageWidth = 500; imageHeight = 375;
int drawSquareflag = 0, drawCircleflag = 0, load1 = 1, loadtest = 0, walkingflag = 0, defaultflag = 0, backflag = 0, traverseflag = 0, fillflag = 0, clickflag = 0;
int clickx, clicky;
//float bbleftx, bbupy, bbrightx, bdowny;
void bgimage(void); //function to draw image when clicked on menu

const float DEG2RAD = 3.14159 / 180.;
int clickxqueue[3] = { 0, 0, 0 }, clickyqueue[3] = { 0, 0, 0 }; //stores coordinates of the click for all 3 stickmans
int count = 0, state = 0, increasing = 1; //state keeps track of states of stickman

//angles defined to create stickman, total 4 states for walking and 2 used for default
int latheta[6] = { 140, 170, 200, 240, 90, 60 };
int lltheta[6] = { 210, 170, 150, 220, 135, 135 };
int rltheta[6] = { 190, 240, 200, 170, -135, -135 };
int lstheta[6] = { -80, 0, 0, -40, 45, 80 };
int rstheta[6] = { -10, -30, 0, -30, -45, -80 };

clock_t start; //stores clock state at init() for start time
float d = 0.0; //actual distance travelled by stickman in oblique box in box's cartesian system

//all the corners of box, used for filling in, calculating slope,etc.

float rightx, righty, downx, downy, leftx, lefty, upx, upy, bottomleftx, bottomlefty, upleftx, uplefty, uprightx, uprighty, downrightx, downrighty;

void flipImag(char *, char *);

float length = 0; //total length of box in cartesian system of box.


//draw Axes
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



//draw unit circle
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

//the stickman creation calling function, taken from Dr. Ross's lecture slides
void traverse(stickNode *node)
{
	if (node == NULL)
		return;
	glPushMatrix();
	//glMultMatrixf(node->m);  //this was not required as nodes are defined relative to their parent and not their roots.
	node->f();
	if (node->child != NULL)
		traverse(node->child);
	glPopMatrix();
	if (node->sibling != NULL)
		traverse(node->sibling);

}

void walking() {
	glLoadIdentity(); //brings the stickman to starting position of new cartesian system before starting to walk.
	length = sqrt((downrightx - bottomleftx)*(downrightx - bottomleftx) + ((downrighty - bottomlefty)*(downrighty - bottomlefty)));
	glTranslatef(-0.5, -0.5, 0); //  0,0 is initially at chest level, bring it to waist level
	
	slope = (downrighty - bottomlefty) / (downrightx - bottomleftx);
	theta = (atan((downrighty - bottomlefty) / (downrightx - bottomleftx)) / DEG2RAD);

	// to translate it to box left corner and rotating - aligning it to box
	glTranslatef((2 * (bottomleftx / imageWidth)) - 0.5, (2 * ((bottomlefty) / imageHeight)) - 0.5, 1);
	glRotatef(theta, 0, 0, 1);
	
	//going forward termination condition
	if (d > ((length) / ((250 * (downrightx - bottomleftx) / (imageWidth - 1)))))
		backflag = 1;
	//moving backward termination condition, which is when d is 2 times the length in cartesian system
	if (d > 2 * ((length) / ((250 * (downrightx - bottomleftx) / (imageWidth - 1)))))
	{
		backflag == 0;
		time1[0] = 0; //start again at 0 time
		gluOrtho2D(-1.0, 1.0, -1.0, 1.0); //this again returns the cartesian to original orientation
		glTranslatef(4 * length / imageWidth, 0, 0); //required to bring stickman back to where it started from
	}
	if (backflag == 1)
	{
		gluOrtho2D(1.0, -1.0, -1.0, 1.0); //reflection, changes the cartesian direction
		//go to the negative direction twice to what you have travelled, i.e now stickman moves from negative x to positive x
		glTranslatef(-4 * length / imageWidth, 0, 0);
		backflag = 0;
	}

	//scale the stickman according to the length and width of box 
	glScalef(((downrightx - bottomleftx) / ((imageWidth))), (uplefty - bottomlefty) / ((imageHeight)), 1);

	//move only when defaultflag is not on
	if (defaultflag == 0)
	{
		glTranslatef(d = speed*time1[0], 0.0, 0.0);
		time1[0]++;
	}

	//create stickman at new location now.
	glTranslatef(0, 1, 0); //to get legs above the surface
	traverse(&root);
	glLoadIdentity();
}	


// set of all functions that create stickman
void rootfunc()
{
	//nothing to do for it	

}

void neckfunc()
{
	glScalef(0.25, 0.25, 1);
	drawLine();
}

void headfunc()
{
	glScalef(4, 4, 1);
	glTranslatef(0.0, 0.5, 0);
	glScalef(0.25, 0.25, 1);
	glColor3f(0.0, 0.0, 1.0);
	drawCircle();
}

void lafunc()
{
	glRotatef(latheta[state], 0, 0, 1);
	glScalef(0.5, 0.5, 1);
	drawLine();

}

void rafunc()
{
	glRotatef(360 - latheta[state], 0, 0, 1);
	glScalef(0.5, 0.5, 1);
	drawLine();

}

void ltfunc()
{
	glScalef(0.5, 0.5, 1);
	glTranslatef(0, -0.5, 0);
	drawLine();


}

void llfunc()
{
	glColor3f(0.0, 0.0, 1.0);
	glRotatef(lltheta[state], 0, 0, 1);
	glScalef(0.707, 0.707, 1);
	drawLine();
	glScalef(1.41, 1.41, 1);


}

void lsfunc()
{
	glColor3f(0.0, 0.0, 1.0); //stickman wearing blue pants :p
	glTranslatef(0, 0.707, 0);
	glRotatef(lstheta[state], 0, 0, 1);
	drawLine();
}

void rlfunc()
{
	glRotatef(rltheta[state], 0, 0, 1);
	glScalef(0.707, 0.707, 1);
	drawLine();
	glScalef(1.41, 1.41, 1);
}

void rsfunc()
{

	glTranslatef(0, 0.707, 0);
	glRotatef(rstheta[state], 0, 0, 1);
	drawLine();
}



void fillindex2() //function called to do actual fill
{

	glColor3f(0.0, 0.0, 1.0); //fill it blue
	glBegin(GL_POLYGON);
	glVertex2f((2.0*(uprightx - 250.0) / (float)imageWidth), (2 * (uprighty - 375 / 2) / (float)imageHeight));
	glVertex2f((2.0*(downrightx - 250) / (float)imageWidth), (2 * (downrighty - 375 / 2) / (float)imageHeight));
	glVertex2f((2.0*(bottomleftx - 250) / (float)imageWidth), (2 * (bottomlefty - 375 / 2) / (float)imageHeight));
	glVertex2f((2.0*(upleftx - 250) / (float)imageWidth), (2 * (uplefty - 375 / 2) / (float)imageHeight));
	glEnd();

	glutPostRedisplay();
}

void fillindexcard() //function to define borders where the fill needs to be made
{

	int tx, ty, border = 0;
	unsigned char *temp;

	//move right from seed
	tx = seedx; ty = seedy;
	while (border != 1)
	{
		tx = tx + 1;
		temp = &(image[3 * (ty * imageWidth + tx)]); //this is rgb value of tx,ty 

		if ((unsigned char)*temp < 20)  //means that rgb values are less than 20 and the pixel is black
			border = 1;
	}

	//store
	rightx = tx;
	righty = ty;
	//----------------------------------------------------------------------------------------------------------

	//move towards right up corner

	border = 0;
	tx = rightx - 1;
	ty = righty;
	temp = &(image[3 * (ty * imageWidth + tx)]);
	while (border != 1)
	{
		int cannotdoanything = 1; // only when it is not moving in either direction, we stop

		if (load1 == 1) //this became necessary as both the images had different orientation, one was slightly inclined towards left, other one was to right
		{
			if ((unsigned char)*(temp + 3) < 10)  //right side pixel is white (only checking red component for simplicity), move to right
			{
				tx = tx - 1; //move left
				cannotdoanything = 0;
			}

			if ((unsigned char)*(temp + (imageWidth * 3)) > 10) //up pixel is white
			{
				ty = ty + 1;//  move up
				cannotdoanything = 0;
			}

		}


		if (loadtest == 1) //it is for image2

		{
			if ((unsigned char)*(temp + 3) > 10)  //right side pixel is white (only checking red component for simplicity), move to right
			{
				tx = tx + 1;
				cannotdoanything = 0;
			}

			if ((unsigned char)*(temp + (imageWidth * 3)) > 10) //up pixel is white
			{
				ty = ty + 1;//  move up
				cannotdoanything = 0;
			}

		}


		temp = &(image[3 * (ty * imageWidth + tx)]); //this is rgb value of tx,ty
		if (cannotdoanything == 1)
			border = 1;

	}
	//store
	uprightx = tx;
	uprighty = ty;
	//===========================================================================================================================

	//move towards right down corner and find it.
	border = 0;
	tx = rightx - 1;
	ty = righty;
	temp = &(image[3 * (ty * imageWidth + tx)]);
	while (border != 1)
	{
		int cannotdoanything = 1;
		if (load1 == 1)
		{
			if ((unsigned char)*(temp + 3) > 20)  //right side pixel is black (only checking red component for simplicity), move to right
			{
				tx = tx + 1;
				cannotdoanything = 0;
			}
		}
		if (loadtest == 1)
		{
			if ((unsigned char)*(temp + 3) < 20)  //right side pixel is black (only checking red component for simplicity), move to right
			{
				tx = tx - 1;
				cannotdoanything = 0;
			}
		}
		if ((unsigned char)*(temp - (imageWidth * 3))>10) //down pixel is white
		{
			ty = ty - 1;//  move down
			cannotdoanything = 0;
		}

		temp = &(image[3 * (ty * imageWidth + tx)]); //this is rgb value of tx,ty

		if (cannotdoanything == 1)
			border = 1;

	}
	downrightx = tx;
	downrighty = ty;

	//=============================================================================================================================================		
	//move up from seed
	tx = seedx;
	ty = seedy;
	border = 0;
	while (border != 1)
	{
		tx = seedx;
		ty = ty + 1;
		temp = &(image[3 * (ty * imageWidth + tx)]); //this is rgb value of tx,ty  imagewidth or imagewidth-1?
		if ((unsigned char)*temp < 20)  //means that rgb values are less than 20 and the pixel is black
			border = 1;

	}

	upx = tx;
	upy = ty;
	//===========================================================================================================================================
	// move left from seed
	border = 0;
	tx = seedx; ty = seedy;
	while (border != 1)
	{
		ty = seedy;
		tx = tx - 1;
		temp = &(image[3 * (ty * imageWidth + tx)]); //this is rgb value of tx,ty
		if ((unsigned char)*(temp - 1) < 20)  //means that rgb values are less than 20 and the pixel is black
			border = 1;

	}
	leftx = tx;
	lefty = ty;
	border = 0;
	tx = tx + 1;
	temp = &(image[3 * (ty * imageWidth + tx)]);

	//tx ty remains at the border
	while (border != 1)
	{
		//translation - translate to down if white block seen, translate to left if white block seen on left
		if ((unsigned char)*(temp - 3) > 20)  //left side pixel is white (only checking red component for simplicity), move to left
			tx = tx - 1;

		if ((unsigned char)*temp < 20) //current pixel is white
			ty = ty - 1;//  move down

		temp = &(image[3 * (ty * imageWidth + tx)]); //this is rgb value of tx,ty 

		//termination condition - if black block seen anytime at present location
		if ((unsigned char)*temp < 20)  //means that rgb values are less than 40 and the pixel is black
			border = 1;

		*temp = 0;

	}
	bottomleftx = tx;
	bottomlefty = ty;

	//==============================================================================================================
	//move towards left up corner and find it.
	border = 0;
	tx = leftx;
	ty = lefty;
	tx = tx + 1;
	temp = &(image[3 * (ty * imageWidth + tx)]);

	//tx ty remains at the border
	while (border != 1)
	{
		//translation - translate to down if white block seen, translate to left if white block seen on left

		int cannotdoanything = 1;
		if ((unsigned char)*(temp - 3) < 20)  //left side pixel is black (only checking red component for simplicity), move to right
		{
			tx = tx + 1;
			cannotdoanything = 0;
		}

		if ((unsigned char)*(temp + (imageWidth * 3))>20) //up pixel is white
		{
			ty = ty + 1;//  move up
			cannotdoanything = 0;
		}


		temp = &(image[3 * (ty * imageWidth + tx)]); //this is rgb value of tx,ty
		if (cannotdoanything == 1)
			border = 1;

	}
	upleftx = tx;
	uplefty = ty;

	//==========================================================================================================
	//move down from seed
	border = 0;
	tx = seedx; ty = seedy;
	while (border != 1)
	{
		tx = seedx;
		ty = ty - 1;
		//tx remains same
		temp = &(image[3 * (ty * imageWidth + tx)]); //this is rgb value of tx,ty  imagewidth or imagewidth-1?
		if ((unsigned char)*temp < 20)  //means that rgb values are less than 40 and the pixel is black
			border = 1;

	}
	downx = tx;
	downy = ty;

}

//=======================================================================================================

//This function will be completed in Assignment 2, you will used drawCircle, drawLine as part of this
void renderStickMan() //this function is used to initialize the structure tree of stickman(called only once at program startup)
{
	glPushMatrix(); //for identity
	glColor3f(0.0, 0.0, 1.0);

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

	head.f = headfunc;
	head.sibling = NULL;
	head.child = NULL;

	glColor3f(0.0, 0.0, 1.0);
	drawCircle();
	glPopMatrix(); //move back to neck 
	glPopMatrix(); //move back to root node


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
	ls.child = NULL;

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

	glPopMatrix(); //move to right leg

	glPopMatrix(); //move to torso

	glPopMatrix(); //move to root node

}



//The rountine display() gets called each time the window is redrawn.  Note that this is linked to the command
//glutDisplayFunc(display); which sets this as the display callback
void display()
{
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, imageWidth, imageHeight);
	glRasterPos2i(-1, -1);  //Why use -1, -1 ? 
	glDrawPixels(imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, image);

	if (traverseflag == 1)
	{
		if (walkingflag == 0) //that means that user has clicked somewhere
		{

			for (int i = 0; i < 3 && i < count; i = i + 1)
			{
				glViewport(clickxqueue[i] - 155 / 2, ((clickyqueue[i] - ((117 / 2)*0.75))), 155, 117);
				if (defaultflag == 0)
					glTranslatef(d = speed*time1[i]++, 0.0, 0.0);
				traverse(&root);
				glLoadIdentity();
				glutPostRedisplay();
			}
		}


		if (walkingflag)
			walking();

	}

	if (fillflag == 1)
		fillindex2();


	if (defaultflag == 1)
	{
		glViewport(205, 103, 155, 117); //draws only on this location, size is also static
		traverse(&root);
	}

	glutPostRedisplay();
	glFlush();

}

//============================================== end of my display function===================================


void loadPNG(char* filename, int width, int height, int bitdepth)
{
	image = stbi_load(filename, &width, &height, &bitdepth, 0);

}

/* Initialize states -- called before */
void init()
{
	oldtime = GetTickCount(); //get current system time at startup
	bgimage(); //load background image
	seedx = 369; seedy = 164;
	glClearColor(1.0, 1.0, 1.0, 0.0);
	renderStickMan(); //one time initialization of stickman

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

	fillindexcard(); //this is called to initialize border coordinates
}

void animation()
{
	//If I want things to move on their own without pressing a button, I need to set up an idle callback function
	//If you look further in the code, you'll see that this function is linked to the glutIdleFunc(animation);

	if (traverseflag == 1 || defaultflag == 1)
	{
		long int dt;
		newtime = GetTickCount();
		dt = newtime - oldtime;
		Sleep((500 - dt / 1000)); //4 stages, 2 sec cycle, so 500 ms delay given
		oldtime = newtime;

		if (!defaultflag)
		{

			if (increasing == 1) state++;	else state--;
			if (state == 4) state = 0;
			if (state == 0) increasing = 1;
		}


	}

	if (defaultflag)
	if (state == 5)
		state = 4;
	else
		state = 5;

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
	case 3: // load image 1, Not changing resolution as both have the defined resoulution
		load1 = 1;
		loadtest = 0;
		fillflag = 0;
		traverseflag = 0;
		bgimage();
		fillindexcard();
		count = 0;
		break;
	case 4: //load image 2
		loadtest = 1;
		load1 = 0;
		fillflag = 0;
		traverseflag = 0;
		bgimage();
		fillindexcard(); //reinitialize border coordinates
		count = 0;
		break;
	case 5: //fill gets on
		fillflag = 1;
		traverseflag = 0;
		defaultflag = 0;
		count = 0;
		break;
	case 6: //fill gets off
		fillflag = 0;
		defaultflag = 0;
		count = 0;
		break;
	case 7: //default
		traverseflag = 0;
		fillflag = 0;
		defaultflag = 1;
		state = 5;
		walkingflag = 0;
		count = 0;
		break;
	case 8: //walking
		state = 0;
		traverseflag = 1;
		defaultflag = 0;
		fillflag = 0;
		walkingflag = 1;
		speed = 0.1; //speed is fast when walking in box
		time1[0] = 0;
		count = 0;
		break;
		glutPostRedisplay();
	}
	// Almost any menu selection requires a redraw
	glutPostRedisplay();
}

//Here I will initialize the menu, for Assignment 1 you will need to create other menu entries with sub-entries
//Specifically, you'll be creating a menu entry "Images" with sub entries "1" and "2" as well as a menu entry "Draw" with sub entries "square" and "circle"
int BuildPopupMenu(void)
{
	int menu, images, filled, animate;



	images = glutCreateMenu(SelectFromMenu);
	glutAddMenuEntry("1", 3);
	glutAddMenuEntry("2", 4);


	filled = glutCreateMenu(SelectFromMenu);
	glutAddMenuEntry("On", 5);
	glutAddMenuEntry("Off", 6);

	animate = glutCreateMenu(SelectFromMenu);
	glutAddMenuEntry("Default", 7);
	glutAddMenuEntry("Walking", 8);

	menu = glutCreateMenu(SelectFromMenu);  //This links a callback function "SelectFromMenu", and that link is saved as the integer menu which is then
	glutAddMenuEntry("Exit demo", 0); //This adds an entry into the menu, if you right click, you'll see "Exit Demo" and this will returned to be linked to the right mouse button
	//send the value MENU_EXIT to the callback function
	glutAddSubMenu("Images", images);
	glutAddSubMenu("filled", filled);
	glutAddSubMenu("Animate", animate);
	return menu;
}

//ALL STUDENTS: You need to fill out this function for getting the image color
//HINT: Use the img2.jpg image to help you debug
//not used in assignment 2, kept for debugging
void getImageColor(int pixel_x, int pixel_y)
{
	char *RED, *GREEN, *BLUE;
	//because opengl renders the image in oppposite to coordinate axes we subtract imageheight from y mod imageheight
	clickx = pixel_x;
	clicky = (imageHeight - 1 - pixel_y);
	RED = image[3 * (clicky * imageWidth + clickx)];
	GREEN = image[3 * (clicky * imageWidth + clickx) + 1];
	BLUE = image[3 * (clicky * imageWidth + clickx) + 2];
	printf("\nScreen click : R=%u, G=%u, B=%u x= %d, y=%d", (unsigned char)RED, (unsigned char)GREEN, (unsigned char)BLUE, clickx, clicky);

}

//GRADUATE STUDENTS: You need to fill out this function to flip the image
//flips a j coordinate with imageheight-1-j
void flipImag(char *image2, char *flippedImage)
{
	char temp;
	for (int i = 0; i < imageWidth * 3; i++)
	{
		for (int j = 0; j < imageHeight / 2; j++) //swap column positions j with imageHeight-j for a particular column
		{
			temp = image[((imageHeight - 1) * imageWidth * 3) - (j*imageWidth * 3) + i];
			image[((imageHeight - 1) * imageWidth * 3) - (j*imageWidth * 3) + i] = image[(j*imageWidth * 3) + i];
			image[(j*imageWidth * 3) + i] = temp;

		}

	}

}

//Here is the mouse callback function, you will need to use the x, y coordinate intelligently to get the color of the pixel that is clicked on.
//You must have a printf statement here as well that prints out the coordinate and the color from the background image at the location
void myMouse(int button, int state, int x, int y)
{
	clickflag = 1;
	walkingflag = 0;
	traverseflag = 1;
	//You need to think about the x and y coordinate that you get here and if they are what you expect
	switch (button)
	{
	case GLUT_LEFT_BUTTON: //only on left button down and not twice per click
		if (state == GLUT_DOWN && traverseflag == 1)
		{
			fillflag = 0;
			for (int i = 2; i >= 0; i = i - 1)
			{
				clickxqueue[i] = clickxqueue[i - 1];
				clickyqueue[i] = clickyqueue[i - 1];
			}

			clickxqueue[0] = x;
			clickyqueue[0] = imageHeight - 1 - y;
			time1[0] = 0; //to give it a new start from its waist, every stickman has its own time1
			count = count + 1;
		}
		glutPostRedisplay();
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
	glutCreateWindow("Assignment - 2");


	/* init some states */
	init();
	renderStickMan();

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
		seedx = 245;
		seedy = 180;
		loadPNG("img1.png", imageWidth, imageHeight, 24);
		flipImag(&image, &flippedImage);
	}

	if (loadtest == 1)
	{
		seedx = 369;
		seedy = 164;
		loadPNG("img2.png", imageWidth, imageHeight, 24);
		flipImag(&image, &flippedImage);


	}
}



