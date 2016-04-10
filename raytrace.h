#include "common.h"
#include "Image.h"
#include "plane.h"
#include "sphere.h"
#include "light.h"
#include <deque>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/mat3x3.hpp>
using namespace glm;

ObjectStruct Ref;
std::deque<ObjectStruct> ObjectList;
std::vector<Object *> pObjectList;
std::vector<Light *> pLightList;
std::vector<Object *> RefObjectList;
std::deque<LightStruct> LightList;

float TMin = 99999999;
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


    Plane lwall1(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 512, 0), vec3(0, 512, 512), vec3(255, 0, 0));
    Plane lwall2(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 512, 512), vec3(0, 0, 512), vec3(255, 0, 0));

    Plane rwall1(vec3(-1, 0, 0), vec3(512, 0, 0), vec3(512, 512, 0), vec3(512, 512, 512), vec3(0, 0, 255));
    Plane rwall2(vec3(-1, 0, 0), vec3(512, 0, 0), vec3(512, 512, 512), vec3(512, 0, 512), vec3(0, 0, 255));

    Plane bwall1(vec3(0, 0, -1), vec3(0, 0, 512), vec3(0, 512, 512), vec3(512, 512, 512), vec3(0, 255, 0));
    Plane bwall2(vec3(0, 0, -1), vec3(0, 0, 512), vec3(512, 512, 512), vec3(512, 0, 512), vec3(0, 255, 0));

    Plane ceiling1(vec3(0, -1, 0), vec3(0, 512, 0), vec3(0, 512, 512), vec3(512, 512, 512), vec3(0, 255, 255));
    Plane ceiling2(vec3(0, -1, 0), vec3(0, 512, 0), vec3(512, 512, 512), vec3(512, 512, 0), vec3(0, 255, 255));

    Plane floor1(vec3(0, 1, 0), vec3(0, 0, 0), vec3(0, 0, 512), vec3(512, 0, 512), vec3(255, 0, 255));
    Plane floor2(vec3(0, 1, 0), vec3(0, 0, 0), vec3(512, 0, 512), vec3(512, 0, 0), vec3(255, 0, 255));



    pObjectList.push_back(&ceiling1);
    pObjectList.push_back(&ceiling2);
    pObjectList.push_back(&floor1);
    pObjectList.push_back(&floor2);
    pObjectList.push_back(&bwall1);
    pObjectList.push_back(&bwall2);
    pObjectList.push_back(&lwall1);
    pObjectList.push_back(&lwall2);
    pObjectList.push_back(&rwall1);
    pObjectList.push_back(&rwall2);

    Sphere Sphere1(vec3(350, 200, 50), 30, vec3(60, 100, 200));
    Sphere Sphere2(vec3(200, 200, 200), 100, vec3(90, 20, 0));

    pObjectList.push_back(&Sphere1);
    pObjectList.push_back(&Sphere2);


    //struct LightStruct
    Light Light1(vec3(256, 256, -200), vec3(0,0,0), vec3(0,0,0), 0.0, false);
    //struct LightStruct
    Light Light2(vec3(300, 200, -150), vec3(0,0,0), vec3(0,0,0), 0.0, false);

    pLightList.push_back(&Light1);
    pLightList.push_back(&Light2);

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


    struct ObjectStruct Sphere11;
    Sphere11.SphereInit(30, vec3(350, 200, 50), vec3(60, 100, 200));
    struct ObjectStruct Sphere22;
    Sphere22.SphereInit(100, vec3(200, 200, 200), vec3(90, 20, 0));

    ObjectList.push_back(Sphere11);
    ObjectList.push_back(Sphere22);

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
    for (int i = 0; i < pObjectList.size(); ++i)
    {
        //if(i==6)
        //    continue;
        float t;
        vec3 Normal;
        vec3 Temp;
//bool DoesIntersect = pObjectList[k]->Intersect(Camera, Direction, &t, &normal, &Temp);

        bool DoesIntersect;
        DoesIntersect = pObjectList[i]->Intersect(Camera, Direction, &t, &Normal, &Temp);

        if (DoesIntersect)
        {
            IntersectCheck = true;
            if (TMin > t)
            {
                PixelColour = Temp;
                //RefObjectList[0] = pObjectList[i];
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
    for (int k = 0; k < pLightList.size(); k++)
    {
        pLightList[k]->LightVector = pLightList[k]->Lightpos - Intersection;
        pLightList[k]->Length = length(pLightList[k]->LightVector);
        pLightList[k]->LightVector = normalize(pLightList[k]->LightVector);
    }
    for (int k = 0; k < pObjectList.size(); ++k)
    {
       // if (pObjectList[k] == RefObjectList[0])
       // {
        //    continue;
        //}
       // if(k==6)
       //     continue;

        //InitShapes();
        float t;
        vec3 Normal;
        vec3 Temp;
        bool IntersectCheck;

        for (int i = 0; i < pLightList.size(); i++)
        {
            pLightList[i]->EyeVector = Intersection + (pLightList[i]->LightVector * 2.0f);

            IntersectCheck = pObjectList[k]->Intersect(pLightList[i]->EyeVector, pLightList[i]->LightVector, &t, &Normal, &Temp);
            //else IntersectCheck = pObjectList[k].IsPlaneIntersect(pLightList[i].EyeVector, pLightList[i].LightVector, &t, &Normal, &Temp);

            if (IntersectCheck && t < pLightList[i]->Length)
            {
                pLightList[i]->IsBlocked = true;
                NumShadows++;
            }
        }
    }

    for (int i = 0; i < pLightList.size(); i++)
    {
        pLightList[i]->IsBlocked = false;
    }
    return NumShadows;
}

inline void CalcColour(int NumShadows)
{
    float DiffuseTerm = 0;
    int i;
    for (i = 0; i < pLightList.size(); i++)
    {
        DiffuseTerm += dot(pLightList[i]->LightVector, NormMin);
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
                TMin = 999999;
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
