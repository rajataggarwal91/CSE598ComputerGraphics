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
#include <math.h>

/*
	RayTrace Class - The class containing the function you will need to implement

	This is the class with the function you need to implement
*/

//public variables, never changing
double epsilon = 0.0001;
int lightflag[10] = { 0 };

void setlight(int light)
{
	lightflag[light] = 1;
	cout << "got it, enabling light " << light;
}




class RayTrace
{
public:
	/* - Scene Variable for the Scene Definition - */
	Scene m_Scene;
	Camera m_Camera;
	Vector screen_pos;
	// -- Constructors & Destructors --
	RayTrace(void)
	{
	}
	~RayTrace(void) {}

	void CameraInitialize()
	{
			m_Camera = m_Scene.GetCamera();


	}
	

	


	// -- Main Functions --
	// - CalculatePixel - Returns the Computed Pixel for that screen coordinate
	Vector CalculatePixel(int screenX, int screenY)
	{

		//int static temp = 1;
		////need to do this only one time :
		//if (temp == 1)
		//{
		//	m_Camera = m_Scene.GetCamera();
		//	temp++;
		//}






		double zdepth = 10000.0; //initial depth magnitude ~ infinity.
		int floorflag = 0, wallflag = 0;

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
		//=================================================================================================//
		//1. First step would be to convert near window screen space into camera parameters.
		// We do this by calculating width and height of near window using FOV,near,AR parameters.

		double height = tan(m_Camera.GetFOV()* 3.14159 / 360.) * 2. * m_Camera.GetNearClip();
		double width = (double)WINDOW_WIDTH / (double)WINDOW_HEIGHT * height;
		Vector lookat = (m_Camera.GetTarget() - m_Camera.position).Normalize();
		//-----------------------------------------------------------------------------------------------------------------------------//
		//2. we need to move anywhere in our near plane window now, to do this we convert every screen position to camera position for every pixel

		Vector hdir = m_Camera.GetUp().Cross(lookat);

		screen_pos = m_Camera.position + (((m_Camera.target - m_Camera.position).Normalize()) * m_Camera.GetNearClip())
			- (m_Camera.GetUp() * float(0.5 - float(screenY) / WINDOW_HEIGHT) * height)
			+ hdir.Normalize() * float(0.5 - float(screenX) / WINDOW_WIDTH) * width;

		//screen_pos is now a matrix containing all camera coordinates[h][w] (range [-h/2 - h/2][-w/2 - w/2])
		//for every screen coordinate[h][w] (range [0-240][0-320]) on screen. 
		//------------------------------------------------------------------------------------------------------------------------------//
		//3. Calculate the ray vector direction(screen_pos[i][j]- Camera_pos)
		Vector ray = screen_pos - m_Camera.GetPosition();
		//ray = ray.Normalize();
		//--------------------------------------------------------------------------------------------------------------------------//
		//4. Iterating all the objects in scene and finding if the object was hit by the ray or not

		Vector color,Ia, Is, Id;
		
		 color = Ia = Id = Is = Vector{ 0.0, 0.0, 0.0 };
		for (int i = 0; i < m_Scene.GetNumObjects(); i++)  //drawing loop
		{
			int shadowflag = 0;
			SceneObject *sceneobject = m_Scene.GetObject(i);
			Vector eye = m_Camera.GetPosition();
			Vector ray = (screen_pos - eye).Normalize();
			Vector IP;
			//cout << "here";
			/*cout << sceneobject->name;
			_getch();
			*/
			

			if (sceneobject->name == "wall" || sceneobject->name == "floor" || sceneobject->name=="ceiling")
			{

				//	_getch();
				//check intersection with sphere mathematical equations(ray_origin,ray_direction)
				//intersection point is given by p0+t*rayvector
				//Calculate t parameter by solving quadratic equations

				//===============================================================================================================
				/* source : http://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
				t = (IT-eye)/ray.n
				where n is normal of plane of triangle.

				Logic :
				1. Calculate normal of triangle by (p2-p1) cross (p1-p0)
				2. Calculate t for triangle for intersection of ray with triangle plane
				3. Now check where the point is relative to the 3 edges vectors. If it is on the same side for all, that means it intersects the triangle
				4. Draw the shade of triangle at screen according to Phong equation.

				*/
				//===============================================================================================================
				SceneTriangle *triangle = (SceneTriangle *)m_Scene.GetObjectA(i);

				Vector p0 = triangle->vertex[0];
				Vector p1 = triangle->vertex[1];
				Vector p2 = triangle->vertex[2];

				Vector n = ((p1 - p0).Cross(p2 - p1)).Normalize();
				//if (triangle->name == "ceiling")
				//{
				//	n = n*-1;
				//	//				triangle->name = "wall";    //treat it like any other wall now
				//}

				double t = ((p0 - eye).Dot(n) / ray.Dot(n));
				IP = eye + ray * t;

				//============================Now we know the intersection point===================================
				//============================Lets shade it========================================================		

				SceneMaterial material[] =
				{ m_Scene.GetMaterial(triangle->material[0]), m_Scene.GetMaterial(triangle->material[1]), m_Scene.GetMaterial(triangle->material[2]) };

				// now we have present coordinates of IP and triangle, 
				//we find out the area of opposite sector using cross product and calculate the ratio.

				SceneMaterial scenematerial = m_Scene.GetMaterial(triangle->material[0]);
				//cout << triangle->name;
				//cout << "m here";
					
					if ((((p1 - p0).Cross(IP - p0))).Dot(((p2 - p1).Cross(IP - p1))) >= 0)
					{
						if ((((p2 - p1).Cross(IP - p1))).Dot(((p0 - p2).Cross(IP - p2))) >= 0)   //check if plane intersects triangle intersects ray
						{


							if (triangle->name == "floor" && wallflag==0 && floorflag==0)
							{

								if (int((IP - p0).Dot((p2 - p0).Cross(n).Normalize())) % 2 + int((IP - p0).Dot((p1 - p0).Cross(n).Normalize())) % 2 == 0)
									scenematerial.diffuse = Vector{ 0.0, 0.0, 1.0 };
								else
									scenematerial.diffuse = Vector{ 1.0, 1.0, 1.0 };
								floorflag = 1;
								
							}
							else if ((triangle->name == "wall" || triangle->name == "ceiling") && floorflag == 0 && wallflag==0)
							{
								cout << triangle->name;
								scenematerial.diffuse = Vector{ 1.0, 1.0, 1.0 };
								if (int((IP - p0).Dot((p2 - p0).Cross(n).Normalize())) % 2 == 0)
									scenematerial.diffuse = Vector{ 1.0, 0.0, 0.0 };
								wallflag = 1;
								//cout << triangle->name;
							}






							for (int j = 0; j < m_Scene.GetNumLights(); j++)
							{
								if ((lightflag[0] == 1 && j == 0) || (lightflag[1] ==1 && (j==1 || j==2)))
								{
									SceneLight light = m_Scene.GetLight(j);
									Vector l = (light.position - IP).Normalize();
									Vector v = (eye - IP).Normalize();
									Vector r = (n * (2 * l.Dot(n)) - l).Normalize();



									shadowflag = 0;
									//=================================================================
									//check intersection for shadows now
									for (int k = 0; k < m_Scene.GetNumObjects(); k++)
									{

										SceneObject *intersectobject = m_Scene.GetObject(k);
										if (intersectobject->IsSphere())
											shadowflag = sphereintersection(light, IP, k);

										if (shadowflag == 1)
										{
											cout << "breaking";
											color = Ia + (Id + Is)*(!shadowflag);
											return color;

										}
										//end of intersecting objects for loop

										/*if (intersectobject->IsTriangle())
										  shadowflag = triangleintersection(light, IP, i);

										  */
									}
									//===========================================================================


									//===========================================================================
									double depth = (IP - eye).Magnitude();
									if (depth <= zdepth && shadowflag == 0)
									{

										Ia = m_Scene.GetBackground().ambientLight * 0.1;
										Id = light.color * scenematerial.diffuse * max(l.Dot(n), 0.0);
										Is = light.color * scenematerial.specular * max(pow(v.Dot(r), scenematerial.shininess), 0.0);
										color = color + Ia + (Id + Is)*(!shadowflag);
										//cout << color.x << color.y << color.z;
										zdepth = depth;
										//return color;
									}
								}
								} //end of lights for loop
					} //end of first if blocks for triangle out of plane
					

					
				} 
				
				
			//	cout << color.z;
				//_getch();
		} //end of if loop for wall and floor
		//return color;


			if (sceneobject->IsSphere())
			{
				//check intersection with sphere mathema	tical equations(ray_origin,ray_direction)
				//intersection point is given by p0+t*rayvector
				//Calculate t parameter by solving quadratic equations

				//===============================================================================================================
				/* source : http://www.ccs.neu.edu/home/fell/CSU540/programs/RayTracingFormulas.htm
				a = dx*dx + dy*dy + dz*dz;
				b = 2 * dx*(x0 - cx) + 2 * dy*(y0 - cy) + 2 * dz*(z0 - cz);
				c = cx*cx + cy*cy + cz*cz + x0*x0 + y0*y0 + z0*z0 +
				-2 * (cx*x0 + cy*y0 + cz*z0) - R*R;
				where dx,dy,dz = (screen coordinate - origin) or ray
				*/
				//===============================================================================================================
				SceneSphere *sphere = (SceneSphere *)m_Scene.GetObjectA(i);
				Vector eye = m_Camera.GetPosition();
				Vector C = sphere->center;
				double R = sphere->radius;
				double a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
				double b = 2 * ray.x * (eye.x - C.x) + 2 * ray.y * (eye.y - C.y) + 2 * ray.z * (eye.z - C.z);
				double c = C.x * C.x + C.y* C.y + C.z * C.z + eye.x * eye.x + eye.y * eye.y + eye.z * eye.z - 2 * (C.x * eye.x + C.y * eye.y + C.z * eye.z) - (R * R);


				double t = (-b - sqrt((b * b) - (4 * a * c))) / (2 * a);
				Vector IP = eye + ray * t;
				double depth = (IP - eye).Magnitude();

				//============================Now we know the intersection point===================================
				//============================Lets shade it========================================================		


				if (!((b*b) - (4 * a * c) < 0)) //we ignore negative determinants
				{
					if (t > 0)
					{
						SceneMaterial scenematerial = m_Scene.GetMaterial(sphere->material);
						
						
						for (int j = 0; j < m_Scene.GetNumLights(); j++)
						{


							SceneLight light = m_Scene.GetLight(j);
							Vector l = (light.position - IP).Normalize();
							Vector n = (IP - sphere->center).Normalize();
							Vector v = (eye - IP).Normalize();
							Vector r = (n * (2 * l.Dot(n)) - l).Normalize();
							//color the intersection point



							//=====================Logic to check if IP is in shadow===========================
							//1. Cast a ray from IP to light ray and check for intersection with any object.
							//2. Redefine variables and vectors ray,a,b,c

							shadowflag = 0;
							for (int i = 0; i < m_Scene.GetNumObjects(); i++)
							{
								SceneObject *intersectobject = m_Scene.GetObject(i);
								/*if (intersectobject->IsSphere())
									shadowflag = sphereintersection(light, IP, i);
									*/if (shadowflag == 1)
											break;
								/*if (intersectobject->IsTriangle())
									shadowflag = triangleintersection(light,IP,i);
									*///sphere intersection check complete
							} //end of for loop

							Ia = m_Scene.GetBackground().ambientLight * 0.1;
							Id = light.color * scenematerial.diffuse * max(l.Dot(n), 0.0);
							Is = light.color * scenematerial.specular * max(pow(v.Dot(r), scenematerial.shininess), 0.0);

							double depth = (v).Magnitude();
							if (depth <= zdepth)
							{

								color = color + Ia + (Id + Is)*(!shadowflag);
								zdepth = depth;
								//return color;
							}
						}
					} //end of draw sphere if bracket
					//cout << color.x << color.y << color.z << endl;
					//_getch();
				}


			}//end of issphere()	
			
			/*_getch();
			cout << color.x << color.y << color.z;
			*/
		
		}
		//if (color.x == 0.0 && color.y == 0.0 && color.z == 0.0)
		//return Vector(0.0f, 0.0f, 1.0f);
		
		

		//why do we need to worry about this?

		if ((screenX < 0 || screenX > WINDOW_WIDTH - 1) ||
			(screenY < 0 || screenY > WINDOW_HEIGHT - 1))
		{
			// Off the screen, return black
			return Vector(0.0f, 0.0f, 0.0f);
		}
		//RayTrace(origin,direction,depth) //depth is no. of iterations
		//_getch();
		//cout << color.x << color.y << color.z;
		//_getch();
		return color;
		
	}

	int sphereintersection(SceneLight light, Vector IP, int objectno)
	{
		Vector ray = (light.position - IP).Normalize();
		Vector eye = IP + ray * epsilon; //eye represents the point near IP.
		double t;
		
		SceneSphere *intersectsphere = (SceneSphere *)m_Scene.GetObjectA(objectno);
			Vector C = intersectsphere->center;
			int shadowflag = 0;
			double R = intersectsphere->radius;

			double a = ray.x * ray.x + ray.y * ray.y + ray.z * ray.z;
			double b = 2 * ray.x * (eye.x - C.x) + 2 * ray.y * (eye.y - C.y) + 2 * ray.z * (eye.z - C.z);
			double c = C.x * C.x + C.y* C.y + C.z * C.z + eye.x * eye.x + eye.y * eye.y + eye.z * eye.z - 2 * (C.x * eye.x + C.y * eye.y + C.z * eye.z) - (R * R);

			if ((b*b) - (4 * a*c) > 0)  //we got an intersection
			{
				t = (-b - sqrt((b * b) - (4 * a * c))) / (2 * a);
				if (t > 0 )
				{
					cout << "hurray";
					shadowflag = 1;
				}
				
			}
			
			return (shadowflag);

	}

	boolean triangleintersection(SceneLight light, Vector IP, int objectno)
	{
		boolean shadowflag = 0;
			SceneTriangle *intersecttriangle = (SceneTriangle *)m_Scene.GetObjectA(objectno);
			Vector p0 = intersecttriangle->vertex[0];
			Vector p1 = intersecttriangle->vertex[1];
			Vector p2 = intersecttriangle->vertex[2];

			Vector ray = (light.position - IP).Normalize();
			Vector eye = IP + ray * epsilon;
			Vector n = ((p1 - p0).Cross(p2 - p1)).Normalize();
			double t = ((p0 - eye).Dot(n) / ray.Dot(n));
			if (t > 0)
			{
				if ((((p1 - p0).Cross(IP - p0))).Dot(((p2 - p1).Cross(IP - p1))) >= 0)
				{
					if ((((p2 - p1).Cross(IP - p1))).Dot(((p0 - p2).Cross(IP - p2))) >= 0)
					{
						shadowflag = 1;
						return (shadowflag);
					}
				}
			}
			return(shadowflag);
		}
	
};




#endif // RAYTRACE_H

