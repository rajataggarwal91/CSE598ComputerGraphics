//=============================================================================
// Sample Application for GLEW, and cwc Application/Window class using freeglut
// Sample Application using files from clockwork coders
// Modified by Ross Maciejewski 2014
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include "Imath\ImathVec.h"
#include <vector>
#include <conio.h>
using namespace Imath;
using namespace std;



GLfloat emerald_material_Ka[] = { 0.0215, 0.1745, 0.0215, 0.55 };
GLfloat emerald_material_Kd[] = { 0.07568, 0.61424, 0.07568, 0.55 };
GLfloat emerald_material_Ks[] = { 0.633, 0.727811, 0.633, 0.55 };
GLfloat emerald_material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
GLfloat emerald_material_Se = 76.8f;

GLfloat gold_material_Ka[] = { 0.24725, 0.1995, 0.0745, 1.0 };
GLfloat gold_material_Kd[] = { 0.75164, 0.60648, 0.22648, 1.0 };
GLfloat gold_material_Ks[] = { 0.628281, 0.555802, 0.366065, 1.0 };
GLfloat gold_material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
GLfloat gold_material_Se = 51.2f;

GLfloat turqoise_material_Ka[] = { 0.1, 0.18725, 0.1745, 0.8 };
GLfloat turqoise_material_Kd[] = { 0.396, 0.74151, 0.69102, 0.8 };
GLfloat turqoise_material_Ks[] = { 0.297254, 0.30829, 0.306678, 0.80 };
GLfloat turqoise_material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
GLfloat turqoise_material_Se = 12.8f;

GLfloat silver_material_Ka[] = { 0.23125, 0.23125, 0.23125, 1.0 };
GLfloat silver_material_Kd[] = { 0.2775, 0.2775, 0.2775, 1.0 };
GLfloat silver_material_Ks[] = { 0.773911, 0.773911, 0.773911, 1.0 };
GLfloat silver_material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
GLfloat silver_material_Se = 89.6f;

GLfloat jade_material_Ka[] = { 0.135, 0.2225, 0.1575, 0.95 };
GLfloat jade_material_Kd[] = { 0.54, 0.89, 0.63, 0.95 };
GLfloat jade_material_Ks[] = { 0.316228, 0.316228, 0.316228, 0.95 };
GLfloat jade_material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
GLfloat jade_material_Se = 12.8f ;

GLfloat ruby_material_Ka[] = { 0.1745, 0.01175, 0.01175, 0.55 };
GLfloat ruby_material_Kd[] = { 0.61424, 0.04136, 0.04136, 0.55 };
GLfloat ruby_material_Ks[] = { 0.727811, 0.626959, 0.626959, 0.55 };
GLfloat ruby_material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
GLfloat ruby_material_Se = 76.8f;

GLfloat Obsidian_material_Ka[] = { 0.05375, 0.05, 0.06625, 0.82 };
GLfloat Obsidian_material_Kd[] = { 0.18275, 0.17, 0.22525, 0.82 };
GLfloat Obsidian_material_Ks[] = { 0.332741, 0.328634, 0.346435, 0.82 };
GLfloat Obsidian_material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
GLfloat Obsidian_material_Se = 38.4f;











//-----------------------------------------------------------------------------

extern "C" unsigned char *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);

class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   cwc::glShader *shader;

protected: 
	int scene;

	//Ross: Some variables I created for texturing
	unsigned char *image;
	int imageWidth;
	int imageHeight;
	int bitDepth;
	GLuint texture;

public:
	myWindow(){}

	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Ross:  This is where I draw things.  I have three different scene examples.
		switch(scene)
		{
			case 1:
				Scene1();
				break;
			case 2:
				Scene2();
				break;
			case 3:
				Scene3();
				break;
		}
		
		glutSwapBuffers();
	}

	void renderSnowman(int s_no)
	{
		
		if (s_no == 1)
			glTranslatef(0.5, 0.0, 0.0);
		else
			glTranslatef(-0.3, 0.0, 0.5); //2nd snowman is diagonally inclined to 1st
		
		
		//???glLoadIdentity() inclusion here does not render snowman??? Why
		GLUquadric* quadratic = gluNewQuadric(); //Create a pointer to the quadric object
		gluQuadricNormals(quadratic, GLU_SMOOTH); //Create smooth normal

		//-------body--------
		//glPushMatrix();


		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Obsidian_material_Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Obsidian_material_Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Obsidian_material_Ks);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Obsidian_material_Ke);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Obsidian_material_Se);

		shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt"); // load (and compile, link) from file



		//----body----------		
		glTranslatef(0.0f, 0.4f, 0.0f);
		if (shader)
			shader->begin();
		gluSphere(quadratic, 0.4, 20, 20); 
		shader->end();
		glDisable(GL_COLOR_MATERIAL);


		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, emerald_material_Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, emerald_material_Kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, emerald_material_Ks);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emerald_material_Ke);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, emerald_material_Se);

			glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.4f);
			if (shader)
				shader->begin();
			if (s_no == 1)
	     		gluSphere(quadratic, 0.03, 20, 20); // center button
			glPushMatrix();

			glTranslatef(0.0f, 0.1f, 0.f);
			if (s_no == 1)
				gluSphere(quadratic, 0.03, 20, 20); // up button

			glPopMatrix();
			glPushMatrix();

			glTranslatef(0.0f, -0.1f, 0.0f);
			if (s_no == 1)
				gluSphere(quadratic, 0.03, 20, 20); // down button

			glPopMatrix();

			shader->end();
			glDisable(GL_COLOR_MATERIAL);
		
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gold_material_Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold_material_Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold_material_Ks);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, gold_material_Ke);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, gold_material_Se);
		
		glPopMatrix();
		glTranslatef(0.0f, 0.6f, 0.0f);

		//-----create head----------
		
		shader->begin();
		glEnable(GL_NORMALIZE);
		gluSphere(quadratic, 0.2, 20, 20); // head
		shader->end();

		glDisable(GL_COLOR_MATERIAL);

		
		glPushMatrix();
		//create eyes and nose
		glTranslatef(-0.05f, 0.1f, 0.2f);
		//shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt"); // load (and compile, link) from file
		
			glEnable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ruby_material_Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ruby_material_Kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ruby_material_Ks);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ruby_material_Ke);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, ruby_material_Se);



			if (shader)
				shader->begin();
			if (s_no == 1)
			gluSphere(quadratic, 0.03, 20, 20); // left eye

			glPopMatrix();
			glPushMatrix();

			glTranslatef(0.05f, 0.1f, 0.2f);
			//shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt"); // load (and compile, link) from file
			if (s_no == 1)
			gluSphere(quadratic, 0.03, 20, 20); // right eye

			glPopMatrix(); //brings back to center of head
			glPushMatrix();
			glRotatef(180, 1, 0, 0);
			glTranslatef(0.0f, 0.0, -0.4f);
			glEnable(GL_NORMALIZE);
			glNormal3f(0.0f, 0.0f, 1.0f);
			if (s_no == 1)
			gluCylinder(quadratic, 0.01f, 0.03f, 0.1f, 20, 20); //nose
			glRotatef(-180, 1, 0, 0);
			glPopMatrix(); //brings back to center of head

			shader->end();
			glDisable(GL_COLOR_MATERIAL);

		
		//----create hat---
		glPushMatrix();
		
		glEnable(GL_COLOR_MATERIAL);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, turqoise_material_Ka);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, turqoise_material_Kd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, turqoise_material_Ks);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, turqoise_material_Ke);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, turqoise_material_Se);


			glTranslatef(0.0f, 0.2f, 0.0f);
			glRotatef(-90, 1, 0, 0);
		
		if (shader)
			shader->begin();
		//glRotatef(-90, 1, 0, 0);
		gluCylinder(quadratic, 0.1f, 0.01f, 0.2f, 20, 20);
		glDisable(GL_COLOR_MATERIAL);

		glPopMatrix();
		//glPopMatrix();
		shader->end();
		

		
		
		//gluDeleteQuadric(quadratic);
	}


	void Scene1(void)
	{
		glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	
       gluPerspective(45,1,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();

	   //creating a plane:
	/*   Vec3<float> v0;
	   Vec3<float> v1;
	   Vec3<float> v2;
	   Vec3<float> v3;
*/
	   vector<V3f> VertexArray;
	   VertexArray.push_back(V3f(1.0, 0.0, 1.0));
	   VertexArray.push_back(V3f(1.0, 0.0, -1.0));
	   VertexArray.push_back(V3f(-1.0, 0.0, 1.0));
	   VertexArray.push_back(V3f(-1.0, 0.0, -1.0));
	   //???The order is incorrect still it renders correctly. Why?

	  
	   gluLookAt(1.0f,2.0f,3.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
			//Essentially I need to tell the shader what texture to use
			//glBindTexture(GL_TEXTURE_2D, this->texture);
		
	   
	   glEnable(GL_COLOR_MATERIAL);
	   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, emerald_material_Ka);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, emerald_material_Kd);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, emerald_material_Ks);
	   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emerald_material_Ke);
	   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, emerald_material_Se);
	   if (shader) shader->begin();
	   	   glBegin(GL_TRIANGLE_STRIP);
			glNormal3f(0.0, 1.0, 0.0); //???Why is this used??? Works even after removing this line.
			for (int i = 0; i < VertexArray.size(); i++)
				glVertex3f(VertexArray[i].x, VertexArray[i].y, VertexArray[i].z);
			glEnd();

		//glutSolidTeapot(1.);
		shader->end();
		glPushMatrix();
		renderSnowman(1);
		glPopMatrix();
		glPushMatrix();
		renderSnowman(2);
		glPopMatrix();



	}

	void Scene2(void)
	{
		glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	
       gluPerspective(45,1,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   gluLookAt(0.0f,0.0f,4.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
		if (shader) shader->begin();
			//Essentially I need to tell the shader what texture to use
			glBindTexture(GL_TEXTURE_2D, this->texture);
			glutSolidSphere(1.0, 100, 100);
		if (shader) shader->end();
	}

	void Scene3(void)
	{
		glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	
       gluPerspective(45,1,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   gluLookAt(0.0f,0.0f,4.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
	   glTranslatef(0, 0, -4);
	   glRotatef(-90, 1, 0, 0);
	   glRotatef(90, 0, 1, 0);
		//Essentially I need to tell the shader what texture to use
		glBindTexture(GL_TEXTURE_2D, this->texture);

		if (shader) shader->begin();
			glutSolidCone(1, 2, 100, 100);
		if (shader) shader->end();
	}


	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		//Ross: Here is where I declare some variables you need for textures (not scene, that's for changing the scene)
		this->scene = 1;
		this->imageWidth = 128;
		this->imageHeight = 128;
		this->bitDepth = 24;

		//Ross:  This is where I set up the lights
		DemoLight();

		//Ross:  This is where I set up the textures
		DemoTexture();

		//Ross: The GLSL loaders initialization is already handled by the CWC files linked.
		//If you change the name of your shader you would need to modify here
		shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
		  if (shader==0) 
			  std::cout << "Error Loading, compiling or linking shader\n";

	}

	//Ross: Callbacks for most everything you need
	virtual void OnResize(int w, int h) {}
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	//Ross: This should be obvious, but these are the keyboard listeners
	virtual void OnKeyDown(int nKey, char cAscii)
	{       
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		} 

		if(cAscii == '1')
			this->scene = 1;
		if(cAscii == '2')
			this->scene = 2;
		if(cAscii == '3')
			this->scene = 3;

		glutPostRedisplay();
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
		if (cAscii == 'f')
		{
			SetFullscreen(true);
		}
		else if (cAscii == 'w')
		{
			SetFullscreen(false);
		}
	};

	//Ross: This is where I set up all the textures
	void DemoTexture(void)
	{
		/* Load in a PNG image */
		int loadCorrectly = 0;
		//Use the stbi_image file to load an image
		image = stbi_load("Frodo.bmp", &this->imageWidth, &this->imageHeight, &this->bitDepth, 0);

		glGenTextures(1, &this->texture); //generate the texture with the loaded data

		//The first thing that must take place in the process of uploading the texture is a call to glBindTexture. 
		//What glBindTexture does is it tells OpenGL which texture "id" we will be working with. 
		//A texture "id" is just a number that you will use to access your textures. Here is a sample call.
		glBindTexture(GL_TEXTURE_2D, texture); //bind texture to its array


		//The glPixelStorei call tells OpenGL how the data that is going to be uploaded is aligned. 
		//This call tells OpenGL that the pixel data which is going to be passed to it is aligned in byte order, 
		//this means that the data has one byte for each component, one for red, green and blue
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
		//The glTexEnvf call sets environment variables for the current texture. 
		//What this does is tell OpenGL how the texture will act when it is rendered into a scene.
		//What this does is sets the active texture to GL_MODULATE. 
		//The GL_MODULATE attribute allows you to apply effects such as lighting and coloring to your texture. 
		//If you do not want lighting and coloring to effect your texture and you would like to display the texture unchanged when 
		//coloring is applied replace GL_MODULATE with GL_DECAL.
		//This is not needed if you use shaders!!
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//The glTexImage2D call is our goal. 
		//This call will upload the texture to the video memory where it will be ready for us to use in our programs. 
		//I am going to explain this call parameter by parameter since it is so important to what we are doing.
		//target - The target of this call, it will always be GL_TEXTURE_2D.
		//level - The level of detail number, this should be left at 0 for our purposes. 
			//Once you become more adept at OpenGL texture mapping this parameter will be something that you might change.
		//internalformat - Internal components parameter. 
			//This tells OpenGL how many color components to represent internally from the texture that is uploaded. 
			//There are many symbolic constants for this parameter but the one which is most widely used is GL_RGB; this constant is equal to 3.
		//width & height - The width and height of the image data. These must be integers that are equal to 2n+2(border) for some integer n.
			//What this basically means is that the texture width and height must be a power of two (2,4,8,16,32,63,128,256,512, etc).
		//border - Image border, must be 0 or 1. I always use 0 in my code since I do not use image borders.
		//format - Format of the pixel data that will be uploaded. There are many constants which are accepted but GL_RGB is the value that is widely used.
		//type - Type of data that will be uploaded. Again there are several symbolic constants but the one which I use is GL_UNSIGNED_BYTE.
		//pixels - Pointer to the image data. 
			//This is the image data that will be uploaded to the video memory. 
			//Note that after your call to glTexImage2D you can free this memory since the texture is already uploaded into video memory.
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glEnable (GL_TEXTURE_2D);
	}

   //Ross: This is where I set up the lights
   void DemoLight(void)
   {
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);
     
     // Light model parameters:
     // -----------------------------	--------------
     
     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;
     
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
     
     
     // ------------------------------------------- 
     // Lighting parameters:

     GLfloat light_pos[] = {5.0f, 5.0f, 5.0f, 1.0f};
     GLfloat light_Ka[]  = {0.5f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {0.1f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

	 
	 
     GLfloat material_Ka[] = {0.0f, 0.0f, 0.5f, 1.0f};
     GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;
	 
	 
     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
   }
};

//-----------------------------------------------------------------------------
class myApplication : public cwc::glApplication
{
public:
	//Ross: Here is a simple output for checking that things are working
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------

