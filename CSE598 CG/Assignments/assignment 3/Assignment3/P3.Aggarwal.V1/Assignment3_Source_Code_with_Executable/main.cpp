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
#include "ilmbase-2.2.0\Imath\ImathVec.h"
#include "ilmbase-2.2.0\config\IlmBaseConfig.h"
#include <vector>
#include <conio.h>
#include "perlin.c"

using namespace Imath;
using namespace std;
#define PI 3.14159


//define material properties
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


//coordinates for camera movement
float zcoor = 3.0f,xcoor=1.0f;
float velocity = 0.5f;
float xcenter = 0.0f, zcenter = -1.0f;
float angle=0.0f;

// texture created for storing perlin noise, RGB values of noise.
unsigned char ptexture[16384];
int pdefaultflag = 1, perlinflag = 0, bdefaultflag = 0, bumpflag = 1;

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
	unsigned char *image,*my_face;
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
			case 4:
				Scene1(); //makes changes in scene 1 itself
				break;
		}
		
		glutSwapBuffers();
	}
	void renderSnowman(int s_no)
	{
		//==============================================
		if (s_no == 1) //1st snowman s_no=1
			glTranslatef(0.5, 0.0, 0.0);
		else
			glTranslatef(-0.3, 0.0, 0.5); //2nd snowman is diagonally arranged to 1st
		//===============================================
		shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt"); // load (and compile, link) from file
		//???glLoadIdentity() inclusion here does not render snowman??? Why

		GLUquadric* quadratic = gluNewQuadric(); //Create a pointer to the quadric object
		gluQuadricNormals(quadratic, GLU_SMOOTH); //Create smooth normal

		//=============================body start===================================
		if (this->scene == 1 || s_no == 1) //material for 1st scene, both snowman and both scene 1st snowman
		{
			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Obsidian_material_Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Obsidian_material_Kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Obsidian_material_Ks);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Obsidian_material_Ke);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Obsidian_material_Se);
			shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt");
		}

		else if (s_no == 2) // texture for snowman 2 for scene2
		{
			GLuint body;
			glGenTextures(1, &body);
			glBindTexture(GL_TEXTURE_2D, body);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			gluQuadricTexture(quadratic, 1);
			shader = SM.loadfromFile("vertexshader.txt", "bodyandhatfragmentshader.txt"); // load (and compile, link) from file
		}

		//======================================perlin noise code for snowman 1====================================
		if (perlinflag == 1 && s_no == 1) //for snowman 1 only when perlin flag is on
		{
			/* logic :
			1. First input perlin noise in a texture function for 128*128 ptexture matrix
			2. Load it and pass it to perlinfragmentshader
			3.  Just load the texture to body in shader
			*/

			gluQuadricTexture(quadratic, 0);

			//--------------------Creating a new texture----------------------------------
			
			GLuint pbody;
			glGenTextures(1, &pbody);
			glBindTexture(GL_TEXTURE_2D, pbody);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			gluQuadricNormals(quadratic, GL_SMOOTH);
			gluQuadricTexture(quadratic, 1);
			shader = SM.loadfromFile("vertexshader.txt", "perlinnoisefragmentshader.txt"); // load (and compile, link) from file


			//--------------------Create a texture ptexture------------------------------
			//R coordinates
			for (int i = 0; i < 128; i = i + 1)
			{
				for (int j = 0; j < 128; j = j + 3)
				{
					*(ptexture + (128 * i) + j) = (((PerlinNoise2D(float(i) / 128, float(j) / 128, 1.0, 2.0, 4) + 1.0) / 2) * 255);
				}
			}
			//G coordinates
			for (int i = 0; i < 128; i = i + 1)
			{
				for (int j = 1; j < 128; j = j + 3)
				{
					*(ptexture + (128 * i) + j) =  (((PerlinNoise2D(float(i) / 128, float(j) / 128, 1.0, 2.0, 4) + 1.0) / 2) * 255);
				}
			}
			//B coordinates
			for (int i = 0; i < 128; i = i + 1)
			{
				for (int j = 2; j < 128; j = j + 3)
				{
					*(ptexture + (128 * i) + j) =  (((PerlinNoise2D(float(i) / 128, float(j) / 128, 1.0, 2.0, 4) + 1.0) / 2) * 255);
				}
			}

			//=========== For some reason the texture is best seen at back side of snowman body=========================







			//for (int i = 0; i < 128; i++)
			//	for (int j = 0; j < 128; j++)
			//		cout << " " << j << " " <<*(my_face+(128 * i) + j) << endl;

			//_getch();
		/*	for (int i = 0; i < 128; i++)
				for (int j = 0; j < 128; j++)
					*(my_face + (128 * i) + j) = *(ptexture + (128 * i) + j);*/
					//cout << i<<" "<<j<<" "<<*(ptexture+(128 * i) + j)  << endl;
			////_getch();


			

						//for (int i = 0; i < 12; i++)
						//for (int j = 0; j < 128; j++)
						//	cout << i << " " << j << " " << "ptexture[" << (128 * i) + j << "] " << ((((PerlinNoise2D(float(i) / 128, float(j) / 128, 1.0, 2.0, 10)+1.0)/2)))/*ptexture[(128 * i) + j]*/ << endl;
						//
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, ptexture);
			glEnable(GL_TEXTURE_2D);
			if (shader)
				shader->begin();
			
		}



		//------------------------body common code for both scenes and both snowman---------------------------------
		glTranslatef(0.0f, 0.4f, 0.0f);
		if (shader)
			shader->begin();

		
		glRotatef(-90, 0, 0, 1);
		//glutSolidTeapot(0.5);
		gluSphere(quadratic, 0.4, 20, 20); 
		shader->end();
		glDisable(GL_COLOR_MATERIAL);
		glRotatef(90, 0, 0, 1);
		gluQuadricTexture(quadratic, 0);

		shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt"); //load originial shader back for rest of the code
		
		
		//==============================buttons=========================================
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, emerald_material_Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, emerald_material_Kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, emerald_material_Ks);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emerald_material_Ke);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, emerald_material_Se);

			//--------------------center button only for first snowman------------------------
			glPushMatrix(); //center of sphere of body
			glTranslatef(0.0f, 0.0f, 0.4f);
			if (shader)
				shader->begin();
			if (s_no == 1)
	     		gluSphere(quadratic, 0.03, 20, 20); 
			glPushMatrix(); //center button

			//-----------------------up button only for first snowman--------------------------
			glTranslatef(0.0f, 0.1f, 0.f);
			if (s_no == 1)
				gluSphere(quadratic, 0.03, 20, 20); 

			glPopMatrix(); //center button
			glPushMatrix(); //center button
			//---------------------- down button only for first snowman-------------------------
			glTranslatef(0.0f, -0.1f, 0.0f);
			if (s_no == 1)
				gluSphere(quadratic, 0.03, 20, 20); 

			glPopMatrix(); //center button

			shader->end();
			glDisable(GL_COLOR_MATERIAL);
		
			glPopMatrix();  //center of sphere
		//=====================================create head=======================
		
			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gold_material_Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold_material_Kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold_material_Ks);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, gold_material_Ke);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, gold_material_Se);
			glTranslatef(0.0f, 0.6f, 0.0f); //0.6 above center of body sphere

		glEnable(GL_NORMALIZE);
		
		//----------------------------snowman 1 head shaded--------------------------------------
		if (s_no==1)
		{ 
		shader->begin();
		gluSphere(quadratic, 0.2, 20, 20); // head
		shader->end();
		}
		//----------------------------snowman 2 head textured with my face----------------------------
		else 
		{
			//--------------------------define new texture---------------------------------
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glBindTexture(GL_TEXTURE_2D, this->texture);
			gluQuadricTexture(quadratic, 1);
			gluQuadricNormals(quadratic, GL_SMOOTH);
			glTexCoord2d(0, 1);
			
			shader = SM.loadfromFile("vertexshader.txt", "facefragmentshader.txt"); 
			if (shader) shader->begin(); // loads face texture
			//------------------------ Set face vertically using transformation--------------------------
			glRotatef(-90, 0, 0, 1);
			glRotatef(90, 0, 1, 0);
			gluSphere(quadratic, 0.2, 20, 20); // head
			glRotatef(-90, 0, 1, 0 );
			glRotatef(90, 0, 0, 1);
			gluQuadricTexture(quadratic, 0);
			shader->end();
		}
		
		glDisable(GL_COLOR_MATERIAL);
				
		glPushMatrix(); //center of head
		
		
		//============================create eyes and nose=================================================
		glTranslatef(-0.05f, 0.1f, 0.2f); //transform to left eye
		shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt"); // load (and compile, link) from file
			glEnable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ruby_material_Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ruby_material_Kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ruby_material_Ks);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, ruby_material_Ke);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, ruby_material_Se);

			if (shader)
				shader->begin();
			if (s_no == 1)
			gluSphere(quadratic, 0.03, 20, 20); // create left eye

			//--------------------------------right eye-----------------------------
			glPopMatrix();    //center of head
			glPushMatrix();   //center of head

			glTranslatef(0.05f, 0.1f, 0.2f);
			if (s_no == 1)
			gluSphere(quadratic, 0.03, 20, 20); // right eye

			//------------------------------------nose-------------------------------
			glPopMatrix();  //brings back to center of head
			glPushMatrix(); //center of head
			
			//-----------------------------------create nose-----------------------------
			glRotatef(180, 1, 0, 0);
			glTranslatef(0.0f, 0.0, -0.3f);
			if (s_no == 1)
			gluCylinder(quadratic, 0.01f, 0.03f, 0.1f, 20, 20); //nose
			glRotatef(-180, 1, 0, 0);
			
			glPopMatrix(); //brings back to center of head
			shader->end();
			glDisable(GL_COLOR_MATERIAL);

		
		//======================================create hat=====================================
		glPushMatrix(); //center of head
		if (this->scene != 4 || s_no!=2) //for snowman 1 and in snowman 2 in scene 1 only
		{
			glEnable(GL_COLOR_MATERIAL);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, turqoise_material_Ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, turqoise_material_Kd);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, turqoise_material_Ks);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, turqoise_material_Ke);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, turqoise_material_Se);
		}
		//========================================BUMP MAPPING CODE========================================
		if (bumpflag == 1 && s_no == 1 && this->scene == 1)
		{
			/*Logic
			1. Create a texture normal map with deviated normals
			2. Put the texture on the hat
			3. referred : Bump Maps UT-Austin
			*/
			GLuint NormalMapTextureID;
			shader = SM.loadfromFile("bumpvertexshader.txt", "bumpfragmentshader.txt"); 
			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, NormalMapTextureID);
			// Perform a Dot3 operation
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_DOT3_RGB_EXT);
			// between the N (of N.L) which is stored in a normal map texture
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
			// with the L (of N.L) which is stored in the vertex’s diffuse color
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT);
			glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);
			
		}



		//====================================procedural texture for hat==========================================
		else if (s_no == 2 && this->scene==4)
		{
			shader = SM.loadfromFile("vertexshader.txt", "bodyandhatfragmentshader.txt"); //procedure in fragment shader
			GLuint hat;
			glGenTextures(1, &hat);
			glBindTexture(GL_TEXTURE_2D, hat);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			gluQuadricTexture(quadratic, 1);
		}

			glTranslatef(0.0f, 0.2f, 0.0f);
			glRotatef(-90, 1, 0, 0);
	//----------------------------------------create hat,common code------------------------------------------		
		if (shader)
			shader->begin();
		gluCylinder(quadratic, 0.1f, 0.01f, 0.2f, 20, 20); 
		glDisable(GL_COLOR_MATERIAL);

		glPopMatrix();
		shader->end();
		shader = SM.loadfromFile("vertexshader.txt", "fragmentshader.txt"); //reload the original shader for rest of the code
		gluQuadricTexture(quadratic, 0);
		}
/*
=========================================================================================================
=======================================end of renderSnowman==============================================
=========================================================================================================
*/
	void Scene1(void)
	{
		glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	
       gluPerspective(45,1,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();

	   //Vertices for ground plane
	   vector<V3f> VertexArray;
	   VertexArray.push_back(V3f(1.0, 0.0, 1.0));
	   VertexArray.push_back(V3f(1.0, 0.0, -1.0));
	   VertexArray.push_back(V3f(-1.0, 0.0, 1.0));
	   VertexArray.push_back(V3f(-1.0, 0.0, -1.0));
	  
	   //???The order is incorrect still it renders correctly. Why?

	   //==============================This is where I position my CAMERA (only works in scene 1)====================================
	   zcenter = -4 * cos(angle * (PI/180));
	   xcenter = -4 * sin(angle * (PI/180));
	   //camera position
	   gluLookAt(xcoor,2.0f,zcoor, 
		          xcenter,0.0f,zcenter,
			       0.0f,1.0f,0.0f);
			
	   //------------------------Creating shader for ground - this will be procedural texture-------------------------------
	   GLuint vao;
	   glGenTextures(1, &vao);
	   glBindTexture(GL_TEXTURE_2D, vao);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	   shader = SM.loadfromFile("vertexshader.txt", "groundfragmentshader.txt"); // load (and compile, link) from file
	    if (shader) shader->begin();
	   
		//==============================Creating ground================================
		glBegin(GL_TRIANGLE_STRIP);
			int i = 0;
			for (float j = 0; j <= 1; j++)
				{
					for (float k = 0; k <= 1; k++)
					{
						glNormal3f(0.0, 1.0, 0.0); //???Why is this used??? Works even after removing this line.
						glTexCoord2f(j, k);
						glVertex3f(VertexArray[i].x, VertexArray[i].y, VertexArray[i].z);
						i++;
					}
				}
		glEnd();
		shader->end();
		
		glPushMatrix();
		renderSnowman(1);
		//--------------------bring back to origin---------------------------------------
		glPopMatrix();
		glPushMatrix();
		//--------------------------------------renderSnowman2---------------------------
		renderSnowman(2);
		glPopMatrix(); //origin
		
	}
	//Here is my menu callback function
	
	

	//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[----------------------]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

	//this code will not be of use for me right now
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
	
	
	// When OnInit is called, a render context (in this case GLUT-Window) is already available!
	//Rajat : Init calls demotexture and demolight at start and sets scene 1
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
		if(cAscii == '4')
			this->scene = 4;
		if (cAscii == '5')
			this->scene = 5;
		if (cAscii == 'w')
		{
			zcoor -= velocity * cos(angle * PI/180);
			xcoor -= velocity * sin(angle *PI/180);
			zcenter -= velocity * cos(angle*PI/180);
			xcenter -= velocity * sin(angle *PI/180);
		}
		if (cAscii == 's')
		{
			zcoor += velocity * cos(angle * PI/180);
			xcoor += velocity * sin(angle * PI/180);
			zcenter += velocity * cos(angle * PI/180);
			xcenter += velocity * sin(angle * PI/180);
		}
		if (cAscii == 'a')
		{
			angle = angle + 10;
		}
		if (cAscii == 'd')
		{
			angle = angle - 10;
			
			//xcenter += .4 * cos(angle/(PI));
			//zcenter -= .4 * sin(angle/(PI));
		}
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
	
	
	
	//================Ross: This is where I set up all the textures=======================
	void DemoTexture(void)
	{
		/* Load in a PNG image */
		int loadCorrectly = 0;
		//Use the stbi_image file to load an image
		image = stbi_load("Frodo.bmp", &this->imageWidth, &this->imageHeight, &this->bitDepth, 0);
		my_face = stbi_load("face2.bmp", &this->imageWidth, &this->imageHeight, &this->bitDepth, 0);
//		ptexture= stbi_load("face2.bmp", &this->imageWidth, &this->imageHeight, &this->bitDepth, 0);

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
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, my_face);
		glEnable (GL_TEXTURE_2D);
	}
   //==================Ross: This is where I set up the lights============================
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
	virtual void OnInit() { std::cout << "Hello World!\n";
	
		}





};

//-----------------------------------------------------------------------------


void SelectFromMenu(int idCommand)
{
	switch (idCommand)
	{
	case 0:
		pdefaultflag = 1;
		perlinflag = 0;
		break;
	case 1:
		pdefaultflag = 0;
		perlinflag = 1;
		break;
	case 2:
		bdefaultflag = 1;
		bumpflag = 0;
		break;
	case 3:
		bdefaultflag = 0;
		bumpflag = 1;
		break;
				
	}
	glutPostRedisplay();

}

int BuildPopupMenu(void)
{
	int T,L,menu;
	
	T = glutCreateMenu(SelectFromMenu);
	glutAddMenuEntry("Default", 0);
	glutAddMenuEntry("Perlin", 1);

	L = glutCreateMenu(SelectFromMenu);
	glutAddMenuEntry("Default", 2);
	glutAddMenuEntry("Bump", 3);

	menu = glutCreateMenu(SelectFromMenu);
	glutAddSubMenu("Texture", T);
	glutAddSubMenu("Lighting", L);
	
	return menu;
}

//MAINFUNC
int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();
	BuildPopupMenu();  //This is going to be a function you write to initialize the menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);  //This attaches the menu to the right mouse click  button
	pApp->run();
	delete pApp;
	return 0;
}



//-----------------------------------------------------------------------------

