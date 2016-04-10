#include "common.h"
#include "Image.h"
#include <deque>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/mat3x3.hpp>
using namespace glm;

struct ObjectStruct Ref;
std::deque<ObjectStruct> ObjectList;
std::deque<LightStruct> LightList;

float TMin = 999999;
vec3 NormMin;
vec3 PixelColour;
Pixel px;
vec3 DirArray[4];
vec3 PixelArray[4];

inline void CopyObject(ObjectStruct a, ObjectStruct b)
{
	a.Object = b.Object;
	a.Radius = b.Radius;
	a.Center = b.Center;
	a.SphereColour = b.SphereColour;
	a.Normal = b.Normal;
	a.v0 = b.v0;
	a.v1 = b.v1;
	a.v2 = b.v2;
	a.Point = b.Point;
	a.PlaneColour = b.PlaneColour;
}

inline void SetColour(Pixel& px, vec3 CalculatedColor)
{
	if (CalculatedColor.x < 0) px.R = 0;
	else if (CalculatedColor.x > 255) px.R = 255;
	else px.R = (unsigned char)CalculatedColor.x;

	if (CalculatedColor.y < 0) px.G = 0;
	else if (CalculatedColor.y > 255) px.G = 255;
	else px.G = (unsigned char)CalculatedColor.y;

	if (CalculatedColor.z < 0) px.B = 0;
	else if (CalculatedColor.z > 255) px.B = 255;
	else px.B = (unsigned char)CalculatedColor.z;

	px.A = 255;
}

inline void InitShapes()
{

	struct ObjectStruct LeftWall1;
	LeftWall1.TriangleInit(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 512, 0), vec3(0, 512, 512), vec3(255, 0, 0));
	struct ObjectStruct LeftWall2;
	LeftWall2.TriangleInit(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 512, 512), vec3(0, 0, 512), vec3(255, 0, 0));
	
	struct ObjectStruct RightWall1;
	RightWall1.TriangleInit(vec3(-1, 0, 0), vec3(512, 0, 0), vec3(512, 512, 0), vec3(512, 512, 512), vec3(0, 0, 255));
	struct ObjectStruct RightWall2;				 
	RightWall2.TriangleInit(vec3(-1, 0, 0), vec3(512, 0, 0), vec3(512, 512, 512), vec3(512, 0, 512), vec3(0, 0, 255));

	struct ObjectStruct BackWall1;
	BackWall1.TriangleInit(vec3(0, 0, -1), vec3(0, 0, 512), vec3(0, 512, 512), vec3(512, 512, 512), vec3(0, 255, 0));
	struct ObjectStruct BackWall2;
	BackWall2.TriangleInit(vec3(0, 0, -1), vec3(0, 0, 512), vec3(512, 512, 512), vec3(512, 0, 512), vec3(0, 255, 0));

	struct ObjectStruct Roof1;
	Roof1.TriangleInit(vec3(0, -1, 0), vec3(0, 512, 0), vec3(0, 512, 512), vec3(512, 512, 512), vec3(0, 255, 255));
	struct ObjectStruct Roof2;
	Roof2.TriangleInit(vec3(0, -1, 0), vec3(0, 512, 0), vec3(512, 512, 512), vec3(512, 512, 0), vec3(0, 255, 255));

	struct ObjectStruct Floor1;
	Floor1.TriangleInit(vec3(0, 1, 0), vec3(0, 0, 0), vec3(0, 0, 512), vec3(512, 0, 512), vec3(255, 0, 255));
	struct ObjectStruct Floor2;
	Floor2.TriangleInit(vec3(0, 1, 0), vec3(0, 0, 0), vec3(512, 0, 512), vec3(512, 0, 0), vec3(255, 0, 255));


	struct ObjectStruct Sphere1;
	Sphere1.SphereInit(30, vec3(350, 200, 50), vec3(60, 100, 200));
	struct ObjectStruct Sphere2;
	Sphere2.SphereInit(100, vec3(200, 200, 200), vec3(90, 20, 0));

	ObjectList.push_back(Sphere1);
	ObjectList.push_back(Sphere2);
	
	ObjectList.push_back(Floor1);
	ObjectList.push_back(Floor2);
	ObjectList.push_back(Roof1);
	ObjectList.push_back(Roof2);
	ObjectList.push_back(BackWall1);
	ObjectList.push_back(BackWall2);
	ObjectList.push_back(RightWall1);
	ObjectList.push_back(RightWall2);
	ObjectList.push_back(LeftWall1);
	ObjectList.push_back(LeftWall2);


	//ObjectList.push_back(BackWall);
	/*ObjectList.push_back(Floor);
	ObjectList.push_back(Roof);
	ObjectList.push_back(BackWall);
	ObjectList.push_back(RightWall);
	ObjectList.push_back(LeftWall);*/

	struct LightStruct Light1(256, 256, -200);
	struct LightStruct Light2(300, 200, -150);

	LightList.push_back(Light1);
	LightList.push_back(Light2);
}

inline void InitAA(float xpos, float ypos, vec3 DirArray[4])
{
	vec3 PixelPos(ypos - 0.25f, xpos - 0.25f, 0);
	vec3 Direction = PixelPos - Camera;
	Direction = normalize(Direction);
	DirArray[0] = Direction;

	PixelPos = vec3(ypos - 0.25f, xpos + 0.25f, 0);
	Direction = PixelPos - Camera;
	Direction = normalize(Direction);
	DirArray[1] = Direction;

	PixelPos = vec3(ypos + 0.25f, xpos + 0.25f, 0);
	Direction = PixelPos - Camera;
	Direction = normalize(Direction);
	DirArray[2] = Direction;

	PixelPos = vec3(ypos + 0.25f, xpos - 0.25f, 0);
	Direction = PixelPos - Camera;
	Direction = normalize(Direction);
	DirArray[3] = Direction;
}

inline bool CalcClosestIntersection(vec3 Direction)
{
	bool IntersectCheck = false;
	for (int i = 0; i < ObjectList.size(); ++i)
	{
		float t;
		vec3 Normal;
		vec3 Temp;

		bool DoesIntersect;
		if (ObjectList[i].Object == 0) DoesIntersect = ObjectList[i].IsSphereIntersect(Camera, Direction, &t, &Normal, &Temp);
		else DoesIntersect = ObjectList[i].IsPlaneIntersect(Camera, Direction, &t, &Normal, &Temp);

		if (DoesIntersect)
		{
			IntersectCheck = true;
			if (TMin > t)
			{
				PixelColour = Temp;
				CopyObject(Ref, ObjectList[i]);
				TMin = t;
				NormMin = Normal;
			}
		}
	}
	return IntersectCheck;
}

inline int CalcLight(int NumShadows, vec3 Direction)
{
	vec3 Intersection = Direction * TMin;
	Intersection = Intersection + Camera;
	for (int k = 0; k < LightList.size(); k++)
	{
		LightList[k].LightVector = LightList[k].Light - Intersection;
		LightList[k].Length = length(LightList[k].LightVector);
		LightList[k].LightVector = normalize(LightList[k].LightVector);
	}
	for (int k = 0; k < ObjectList.size(); ++k)
	{
		if (ObjectEqual(ObjectList[k], Ref))
		{
			continue;
		}
		float t;
		vec3 Normal;
		vec3 Temp;
		bool IntersectCheck;

		for (int i = 0; i < LightList.size(); i++)
		{
			LightList[i].EyeVector = Intersection + (LightList[i].LightVector * 2.0f);

			if (ObjectList[k].Object == 0) IntersectCheck = ObjectList[k].IsSphereIntersect(LightList[i].EyeVector, LightList[i].LightVector, &t, &Normal, &Temp);
			else IntersectCheck = ObjectList[k].IsPlaneIntersect(LightList[i].EyeVector, LightList[i].LightVector, &t, &Normal, &Temp);

			if (IntersectCheck && t < LightList[i].Length)
			{
				LightList[i].IsBlocked = true;
				NumShadows++;
			}
		}
	}

	for (int i = 0; i < LightList.size(); i++)
	{
		LightList[i].IsBlocked = false;
	}
	return NumShadows;
}

inline void CalcColour(int NumShadows)
{
	float DiffuseTerm = 0;
	int i;
	for (i = 0; i < LightList.size(); i++)
	{
		DiffuseTerm += dot(LightList[i].LightVector, NormMin);
	}
	DiffuseTerm = DiffuseTerm / i;
	if (DiffuseTerm > 0)
	{
		//*0.5
		vec3 PixelDiffuseColour = DiffuseColour * DiffuseTerm;
		PixelColour = PixelColour + PixelDiffuseColour;
	}

	for (i = 0; i < NumShadows; i++)
	{
		//Shadow Intensity
		PixelColour = PixelColour * 0.4f;
	}
}

inline void RayTrace(Image* pImage)
{
	InitShapes();

	for (int i = 0; i < 512; ++i)
		for (int j = 0; j < 512; ++j)
		{
			float xpos = i;
			float ypos = j;
			bool NotBackground = false;

			InitAA(xpos, ypos, DirArray);

			//anti-ailiasing
			for (int k = 0; k < 4; k++){
				TMin = 99999;
				//check if and what ray is intersecting
				if (CalcClosestIntersection(DirArray[k]))
				{
					int NumShadows = 0;
					NumShadows = CalcLight(NumShadows, DirArray[k]);

					CalcColour(NumShadows);
					PixelArray[k] = PixelColour;
					NotBackground = true;
				}
				else
				{
					//If intersect fails paint black
					(*pImage)(i, j) = Pixel(0, 0, 0);
				}
			}
			if (NotBackground){
				vec3 ColourTemp = PixelArray[0] + PixelArray[1];
				ColourTemp = ColourTemp + PixelArray[2];
				ColourTemp = ColourTemp + PixelArray[3];

				ColourTemp /= 4.0f;

				SetColour(px, ColourTemp);
				(*pImage)(i, j) = px;
			}
		}
}
