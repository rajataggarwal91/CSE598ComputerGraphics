#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"


// The Width & Height have been reduced to help you debug
//	more quickly by reducing the resolution 
//  Your code should work for any dimension, and should be set back
//	to 640x480 for submission.

#define WINDOW_WIDTH 250
#define WINDOW_HEIGHT 250
#define PI 3.14159

#include "Scene.h"
#include "RayTrace.h"
#include "NormalRenderer.h"

/* --- Global State Variables --- */


/* - Menu State Identifier - */
int g_iMenuId;


/* - Mouse State Variables - */
int g_vMousePos[2] = {0, 0};
int g_iLeftMouseButton = 0;    /* 1 if pressed, 0 if not */
int g_iMiddleMouseButton = 0;
int g_iRightMouseButton = 0;

/* - RayTrace Variable for the RayTrace Image Generation - */
RayTrace g_RayTrace;

/* - NormalRenderer Variable for drawing with OpenGL calls instead of the RayTracer - */
NormalRenderer g_NormalRenderer;


/* - RayTrace Buffer - */
Vector g_ScreenBuffer[WINDOW_HEIGHT][WINDOW_WIDTH];

unsigned int g_X = 0, g_Y = 0;
bool g_bRayTrace = false;
bool g_bRenderNormal = true;

void myinit()
{
	// Default to these camera settings
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 1, -1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the Scene Variable for the NormalRenderer
	g_NormalRenderer.SetScene (&g_RayTrace.m_Scene);
	cout << "this was reached";
	glClearColor(0, 0, 0, 0);

	g_RayTrace.CameraInitialize();

}

void display()
{
	if (g_bRenderNormal)
	{
		g_NormalRenderer.RenderScene ();
	}
	else
	{
		// Set up the camera to render pixel-by-pixel
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity ();
		glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 1, -1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBegin(GL_POINTS);
		{
			for (int y = 0; y < WINDOW_HEIGHT; y++)
			{
				for (int x = 0; x < WINDOW_WIDTH; x++)
				{
					glColor3f(g_ScreenBuffer[y][x].x, g_ScreenBuffer[y][x].y, g_ScreenBuffer[y][x].z);
					glVertex2i(x, y);
				}
			}
		}

		glEnd();
	}

	glFlush();

	glutSwapBuffers ();
}

void menufunc(int value)
{
	switch (value)
	{
	case 0:
		// Start the Ray Tracing
		g_bRayTrace = true;
		g_bRenderNormal = false;
		break;
	case 1:
		// Render Normal
		g_bRayTrace = false;
		g_X = 0;
		g_Y = 0;
		g_bRenderNormal = true;
		glutPostRedisplay ();
		break;
	case 2:
		// Quit Program
		exit(0);
		break;
	}
}

void doIdle()
{

	
	if (g_bRayTrace)
	{
		g_ScreenBuffer[g_Y][g_X] = g_RayTrace.CalculatePixel (g_X, g_Y);
		
		// Move to the next pixel
		g_X++;
		if (g_X >= WINDOW_WIDTH)
		{
			// Move to the next row
			g_X = 0;
			g_Y++;

			//You can uncomment the next line to see the raytrace update each step
			glutPostRedisplay();
		}

		// Check for the end of the screen
		if (g_Y >= WINDOW_HEIGHT)
		{
			g_bRayTrace = false;
			glutPostRedisplay ();
		}
	}
	else
	{
		glutPostRedisplay ();
	}
}

void mousebutton(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		g_iLeftMouseButton = (state==GLUT_DOWN);
		break;
	case GLUT_MIDDLE_BUTTON:
		g_iMiddleMouseButton = (state==GLUT_DOWN);
		break;
	case GLUT_RIGHT_BUTTON:
		g_iRightMouseButton = (state==GLUT_DOWN);
		break;
	}

	g_vMousePos[0] = x;
	g_vMousePos[1] = y;
}

void keybd_event1(unsigned char cAscii, int nKey, int idonknowwhattodowoththisvariable )
{
	//if (cAscii == 27) // 0x1b = ESC
	//{
	//	this->Close(); // Close Window!
	//}
	
	

	float velocity = 1.0;
	static float angle = 0.0, vangle = 0.0;
	
	if (cAscii == 'w')
	{
		Vector fdir = (g_NormalRenderer.sceneCam.target - g_NormalRenderer.sceneCam.position).Normalize();
		g_NormalRenderer.sceneCam.position = g_NormalRenderer.sceneCam.position + (fdir * velocity);
		g_NormalRenderer.sceneCam.position = g_NormalRenderer.sceneCam.position + (fdir * velocity);
		g_NormalRenderer.sceneCam.target = g_NormalRenderer.sceneCam.target + (fdir * velocity);
		g_NormalRenderer.sceneCam.target = g_NormalRenderer.sceneCam.target + (fdir * velocity);
		g_RayTrace.m_Camera.position = g_RayTrace.m_Camera.position + (fdir * velocity);
		g_RayTrace.m_Camera.position = g_RayTrace.m_Camera.position + (fdir * velocity);
		g_RayTrace.m_Camera.target = g_RayTrace.m_Camera.position + (fdir * velocity);
		g_RayTrace.m_Camera.target = g_RayTrace.m_Camera.position + (fdir * velocity);
		
	}
	if (cAscii == 's')
	{
		Vector fdir = (g_NormalRenderer.sceneCam.target - g_NormalRenderer.sceneCam.position).Normalize();
		g_NormalRenderer.sceneCam.position = g_NormalRenderer.sceneCam.position - (fdir * velocity);
		g_NormalRenderer.sceneCam.position = g_NormalRenderer.sceneCam.position - (fdir * velocity);
		g_NormalRenderer.sceneCam.target = g_NormalRenderer.sceneCam.target - (fdir * velocity);
		g_NormalRenderer.sceneCam.target = g_NormalRenderer.sceneCam.target - (fdir * velocity);
		g_RayTrace.m_Camera.position = g_RayTrace.m_Camera.position - (fdir * velocity);
		g_RayTrace.m_Camera.position = g_RayTrace.m_Camera.position - (fdir * velocity);
		g_RayTrace.m_Camera.target = g_RayTrace.m_Camera.target - (fdir * velocity);
		g_RayTrace.m_Camera.target = g_RayTrace.m_Camera.target - (fdir * velocity);
	}
	if (cAscii == 'a')
	{
		double dist = (g_NormalRenderer.sceneCam.GetPosition() - g_NormalRenderer.sceneCam.GetTarget()).Magnitude();
		angle = angle + 10;
		g_NormalRenderer.sceneCam.target.z  = -dist * cos(angle * (PI / 180));
		g_NormalRenderer.sceneCam.target.x  = -dist * sin(angle * (PI / 180));
	
		g_RayTrace.m_Camera.target.z = -dist * cos(angle * (PI / 180));
		g_RayTrace.m_Camera.target.x = -dist * sin(angle * (PI / 180));

	
	}	
	if (cAscii == 'd')
	{
		double dist = (g_NormalRenderer.sceneCam.GetPosition() - g_NormalRenderer.sceneCam.GetTarget()).Magnitude();
		angle = angle - 10;
		g_NormalRenderer.sceneCam.target.z = -dist * cos(angle * (PI / 180));
		g_NormalRenderer.sceneCam.target.x = -dist* sin(angle * (PI / 180));
		
		g_RayTrace.m_Camera.target.z = -dist * cos(angle * (PI / 180));
		g_RayTrace.m_Camera.target.x = -dist * sin(angle * (PI / 180));

	}

	if (cAscii == 'q')
	{
		double dist = (g_NormalRenderer.sceneCam.GetPosition() - g_NormalRenderer.sceneCam.GetTarget()).Magnitude();
		vangle = vangle - 10;
		g_NormalRenderer.sceneCam.target.z = -dist * cos(vangle * (PI / 180));
		g_NormalRenderer.sceneCam.target.y = -dist* sin(vangle * (PI / 180));

		g_RayTrace.m_Camera.target.z = -dist * cos(vangle * (PI / 180));
		g_RayTrace.m_Camera.target.y = -dist * sin(vangle * (PI / 180));



	}

	if (cAscii == 'z')
	{
		double dist = (g_NormalRenderer.sceneCam.GetPosition() - g_NormalRenderer.sceneCam.GetTarget()).Magnitude();
		vangle = vangle + 10;
		g_NormalRenderer.sceneCam.target.z = -dist * cos(vangle * (PI / 180));
		g_NormalRenderer.sceneCam.target.y = -dist* sin(vangle * (PI / 180));

		g_RayTrace.m_Camera.target.z = -dist * cos(vangle * (PI / 180));
		g_RayTrace.m_Camera.target.y = -dist * sin(vangle * (PI / 180));
		
	}

	if (cAscii == '1')
	{
		setlight(0);
		cout << "r";
	}
	if (cAscii == '2')
	{
		setlight(1);
		setlight(2);
		cout << "t";
	}

	g_NormalRenderer.RenderScene();
	glutPostRedisplay();
	return;
	}










int main (int argc, char ** argv)
{

	//You will be creating a menu to load in scenes
	//The test.xml is the default scene and you will modify this code
	if (!g_RayTrace.m_Scene.Load ("test.xml"))
	{
		printf ("failed to load scene\n");
		exit(1);
	}

	glutInit(&argc,argv);


	/* create a window */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow("Assignment 4 - Ray Tracer");

	/* tells glut to use a particular display function to redraw */
	glutDisplayFunc(display);

	/* create a right mouse button menu */
	g_iMenuId = glutCreateMenu(menufunc);
	glutSetMenu(g_iMenuId);
	glutAddMenuEntry("Render RayTrace",0);
	glutAddMenuEntry("Render Normal",1);
	glutAddMenuEntry("Quit",2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* callback for mouse button changes */
	glutMouseFunc(mousebutton);

	/*callback for keyboard button press*/
	glutKeyboardFunc(keybd_event1);

	/* callback for idle function */
	glutIdleFunc(doIdle);

	/* do initialization */
	myinit();

	glutMainLoop();
	return 0;
}
