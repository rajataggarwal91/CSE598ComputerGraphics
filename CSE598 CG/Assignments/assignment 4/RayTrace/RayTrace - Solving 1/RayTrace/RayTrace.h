/*
  NOTE: This is the file you will need to begin working with
		You will need to implement the RayTrace::CalculatePixel () function

  This file defines the following:
	RayTrace Class
*/

#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <conio.h>
#include "Utils.h"

/*
	RayTrace Class - The class containing the function you will need to implement

	This is the class with the function you need to implement
*/

class RayTrace
{
public:
	/* - Scene Variable for the Scene Definition - */
	Scene m_Scene;
	Camera m_Camera;
	Vector screen_pos[WINDOW_HEIGHT][WINDOW_WIDTH];
	// -- Constructors & Destructors --
	RayTrace (void) 
	{ 	}
	~RayTrace (void) {}

	// -- Main Functions --
	// - CalculatePixel - Returns the Computed Pixel for that screen coordinate
	Vector CalculatePixel(int screenX, int screenY)
	{
		/*
			-- How to Implement a Ray Tracer --

			This computed pixel will take into account the camera and the scene
			and return a Vector of <Red, Green, Blue>, each component ranging from 0.0 to 1.0

			In order to start working on computing the color of this pixel,
			you will need to cast a ray from your current camera position
			to the image-plane at the given screenX and screenY
			coordinates and figure out how/where this ray intersects with
			the objects in the scene descriptor.
			The Scene Class exposes all of the scene's variables for you
			through its functions such as m_Scene.GetBackground (), m_Scene.GetNumLights (),
			etc. so you will need to use those to learn about the World.

			To determine if your ray intersects with an object in the scene,
			you must calculate where your objects are in 3D-space [using
			the object's scale, rotation, and position is extra credit]
			and mathematically solving for an intersection point corresponding to that object.

			For example, for each of the spheres in the scene, you can use
			the equation of a sphere/ellipsoid to determine whether or not
			your ray from the camera's position to the screen-pixel intersects
			with the object, then from the incident angle to the normal at
			the contact, you can determine the reflected ray, and recursively
			repeat this process capped by a number of iterations (e.g. 10).

			Using the lighting equation to calculate the color at every
			intersection point and adding its fractional amount (determined by the material)
			will get you a final color that returns to the eye at this point.
			*/

		//1. First step would be to convert near window screen space into camera parameters.

		// We do this by calculating width and height of near window using FOV,near,AR parameters.
		m_Camera = m_Scene.GetCamera();
		float height = tan(m_Camera.GetFOV() / 2) * 2 * m_Camera.nearClip;
		float width = WINDOW_WIDTH / WINDOW_HEIGHT * height;
		//-----------------------------------------------------------------------------------------------------------------------------//
		//2. we need to move anywhere in our near plane window now, to do this we convert every screen position to camera position for every pixel

		for (int i = 0; i < WINDOW_HEIGHT; i++)
		{
			for (int j = 0; j < WINDOW_WIDTH; j++)
			{
				screen_pos[i][j] = m_Camera.position + ((m_Camera.target) * m_Camera.GetNearClip()) + (m_Camera.GetUp() * (0.5 - i / WINDOW_HEIGHT) * height);
			}
		}
		//screen_pos is now a matrix containing all camera coordinates[h][w] (range [-h/2 - h/2][-w/2 - w/2])
		//for every screen coordinate[h][w] (range [0-240][0-320]) on screen. 
		//------------------------------------------------------------------------------------------------------------------------------//
		//3. Calculate the ray vector direction(screen_pos[i][j]- Camera_pos)
		static Vector ray[WINDOW_HEIGHT][WINDOW_WIDTH];

		for (int i = 0; i < WINDOW_HEIGHT; i++)
		{
			for (int j = 0; j < WINDOW_WIDTH; j++)
			{
				ray[i][j] = screen_pos[i][j] - m_Camera.GetPosition();
			}
		}
		//--------------------------------------------------------------------------------------------------------------------------//
			//4. Iterating all the objects in scene and finding if the object was hit by the ray or not

			for (int i = 0; i < m_Scene.GetNumObjects(); i++)
			{
				SceneObject *sceneobject = m_Scene.GetObject(i);
				if (sceneobject->IsSphere())
				{
				//check intersection with sphere mathematical equations(ray_origin,ray_direction)
				//intersection point is given by p0+t*rayvector
				//Calculate t parameter by solving quadratic equations
					SceneSphere *sphere = (SceneSphere *)m_Scene.GetObjectA(i);
					/* source : http://www.ccs.neu.edu/home/fell/CSU540/programs/RayTracingFormulas.htm
					a = dx*dx + dy*dy + dz*dz;
					b = 2 * dx*(x0 - cx) + 2 * dy*(y0 - cy) + 2 * dz*(z0 - cz);
					c = cx*cx + cy*cy + cz*cz + x0*x0 + y0*y0 + z0*z0 +
						-2 * (cx*x0 + cy*y0 + cz*z0) - R*R;
						
					where dx,dy,dz = (screen coordinate - origin) or ray
						*/
					Vector eye = m_Camera.GetPosition();
					Vector C = sphere->position;
					double R = sphere->radius;
					double a = ray[screenX][screenY].x * ray[screenX][screenY].x + ray[screenX][screenY].y * ray[screenX][screenY].y + ray[screenX][screenY].z * ray[screenX][screenY].z;
					double b = 2 * ray[screenX][screenY].x * (eye.x - C.x) + 2 * ray[screenX][screenY].y * (eye.y - C.y) + 2 * ray[screenX][screenY].z * (eye.z - C.z);
					double c = C.x * C.x + C.y * C.y + C.z * C.z + eye.x * eye.x + eye.y * eye.y + eye.z * eye.z - 2 * (C.x * eye.x + C.y * eye.y + C.z * eye.z) - (R * R);

					printf("a=%lf\nb=%lf\c=n%lf\nd=%lf\nscreenX=%d\nscreenY=%d", a, b, c, (b*b) - (4 * a * c),screenX,screenY);
					

					
					if ((b*b) - (4 * a * c) > 0) //we ignore the tangent and negative determinants
					{
						double t = (-b + ((b * b) - (4 * a * c))) / 2 * a;
						printf("\n%lf", t);
						_getch();
					}
					
				
				
				
				
				
				
				
				
				
				
				
				}

			}
			//-------------------------------------------------------------------------------------------------------------------------//
			//5. Return the color :  Phong shading of the object that got hit 
			//for all the lights
			boolean shadowFlag = 0;
			for (int i = 0; i < m_Scene.GetNumLights(); i++)
			{
				//Do a shadow check too
				SceneObject *sceneobject = m_Scene.GetObject(i);
				if (sceneobject->IsSphere())
				{
					//check intersection with sphere mathematical equations(intersection_point + (epsilon * light direction ), light direcction) return t;
					// if there is an intersection that means the point is in shadow (like inside point of a sphere) and we can break out of for loop
						

				
				
				
				
				
				
				}

				Vector color;
			
				
				
				/*	color the intersection point
				color = Ia + (Id + Is) * shadowFlag;
*/


			}

			// Until this function is implemented, return white
			return Vector(1.0f, 1.0f, 1.0f);
		
		if ((screenX < 0 || screenX > WINDOW_WIDTH - 1) ||
			(screenY < 0 || screenY > WINDOW_HEIGHT - 1))
		{
			// Off the screen, return black
			return Vector(0.0f, 0.0f, 0.0f);
		}
		//RayTrace(origin,direction,depth) //depth is no. of iterations
	}
};

#endif // RAYTRACE_H
