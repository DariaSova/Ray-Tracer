#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/mat3x3.hpp>
#include <string>
#include "Image.h"
#include "plane.h"
#include "sphere.h"
#include "light.h"

using namespace glm;

std::vector<Object *> pObjectList;
std::vector<Light *> pLightList;

float t_min = 99999999;
vec3 Norm_min;
vec3 PixelColour;
Pixel px;
vec3 DirArray[4];
vec3 PixelArray[4];
vec3 Camera(256, 256, -400);
vec3 DiffuseColour(255, 128, 128);

/*inline vec3 CalcReflection(vec3 IP, vec3 r)
{
    bool IntersectCheck = false;
    vec3 Colour;
    for (int i = 0; i < pObjectList.size(); ++i)
    {
        float t;
        vec3 Normal;
        vec3 Temp;
        bool DoesIntersect;
        //if (pObjectList[i].Object == 0) DoesIntersect = pObjectList[i].IsSphereIntersect(IP, r, &t, &Normal, &Temp);
        DoesIntersect = pObjectList[i]->Intersect(IP, r, &t, &Normal, &Temp);


        if (DoesIntersect)
        {
            IntersectCheck = true;
            if (t_min > t)
            {
                Colour = Temp;
                t_min = t;
            }
        }
    }

    if (!IntersectCheck) return vec3(0, 0, 0) - DiffuseColour;
    return Colour;
}*/
inline vec3 Reflection(vec3 IP, vec3 r)
{
    vec3 Color(0,0,0);
    bool IntersectCheck = false;
    for (int i = 0; i < pObjectList.size(); ++i)
    {

        float t;
        vec3 Normal;
        vec3 Temp;

        bool DoesIntersect;
        DoesIntersect = pObjectList[i]->Intersect(IP, r, &t, &Normal, &Temp);

        if (DoesIntersect)
        {
            IntersectCheck = true;
            if (t_min > t)
            {

                Color = Temp;
                t_min = t;
                Norm_min = Normal;
            }
        }

    }
    return Color;
}

inline vec3 CalcReflection(vec3 IP, vec3 r)
{
    float TMin2 = 999999;
    vec3 NormMin2;
    vec3 PixelColour2;
    bool IntersectCheck = false;
    vec3 Colour;
    for (int j = 0; j < pObjectList.size(); j++)
    {

        float t;
        vec3 Normal;
        vec3 Temp;
        bool DoesIntersect;
        DoesIntersect = pObjectList[j]->Intersect(IP, r, &t, &Normal, &Temp);


        if (DoesIntersect)
        {
            IntersectCheck = true;
            if (TMin2 > t)
            {
                PixelColour2 = Temp;
                TMin2 = t;
            }
        }
    }

    if (!IntersectCheck) return vec3(0, 0, 0) - DiffuseColour;
    if (IntersectCheck)
    {
        int NumShadows = 0;


        vec3 Intersection = r * TMin2;
        Intersection = Intersection + IP;
        for (int k = 0; k < pLightList.size(); k++)
        {
            pLightList[k]->LightVector = pLightList[k]->Lightpos - Intersection;
            pLightList[k]->Length = length(pLightList[k]->LightVector);
            pLightList[k]->LightVector = normalize(pLightList[k]->LightVector);
        }
        for (int k = 0; k < pObjectList.size(); ++k)
        {

            float t;
            vec3 Normal;
            vec3 Temp;
            bool IntersectCheck;

            for (int i = 0; i < pLightList.size(); i++)
            {

                pLightList[i]->EyeVector = Intersection + (pLightList[i]->LightVector * 2.0f);


                //if (pObjectList[k].Object == 0) IntersectCheck = pObjectList[k].IsSphereIntersect(pLightList[i].EyeVector, pLightList[i].LightVector, &t, &Normal, &Temp);
                IntersectCheck = pObjectList[k]->Intersect(pLightList[i]->EyeVector, pLightList[i]->LightVector, &t, &Normal, &Temp);

                if (IntersectCheck && t < pLightList[i]->Length)
                {
                    pLightList[i]->IsBlocked = true;
                    NumShadows++;
                }
            }
        }


        float DiffuseTerm = 0;
        int i;
        for (i = 0; i < pLightList.size(); i++)
        {
            DiffuseTerm += dot(pLightList[i]->LightVector, NormMin2);
        }
        DiffuseTerm = DiffuseTerm / i;
        if (DiffuseTerm > 0)
        {
            //*0.5
            vec3 PixelDiffuseColour = DiffuseColour * DiffuseTerm;
            PixelColour2 = PixelColour2 + PixelDiffuseColour;
        }

        for (i = 0; i < NumShadows; i++)
        {
            //Shadow Intensity
            PixelColour2 = PixelColour2 * 0.4f;
        }
        Colour = PixelColour2;
    }
    else
    {
        Colour = vec3(0, 0, 0);
    }
    return Colour;
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

inline void InitializeObjects()
{


    Plane lwall1(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 512, 0), vec3(0, 512, 512), vec3(255, 128, 0), false);
    Plane lwall2(vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 512, 512), vec3(0, 0, 512), vec3(255, 128, 0), false);

    Plane rwall1(vec3(-1, 0, 0), vec3(512, 0, 0), vec3(512, 512, 0), vec3(512, 512, 512), vec3(102, 204, 0), false);
    Plane rwall2(vec3(-1, 0, 0), vec3(512, 0, 0), vec3(512, 512, 512), vec3(512, 0, 512), vec3(102, 204, 0), false);

    Plane bwall1(vec3(0, 0, -1), vec3(0, 0, 512), vec3(0, 512, 512), vec3(512, 512, 512), vec3(204, 0, 0), false);
    Plane bwall2(vec3(0, 0, -1), vec3(0, 0, 512), vec3(512, 512, 512), vec3(512, 0, 512), vec3(204, 0, 0), false);

    Plane ceiling1(vec3(0, -1, 0), vec3(0, 512, 0), vec3(0, 512, 512), vec3(512, 512, 512), vec3(76, 0, 153), false);
    Plane ceiling2(vec3(0, -1, 0), vec3(0, 512, 0), vec3(512, 512, 512), vec3(512, 512, 0), vec3(76, 0, 153), false);

    Plane floor1(vec3(0, 1, 0), vec3(0, 0, 0), vec3(0, 0, 512), vec3(512, 0, 512), vec3(204, 204, 0), false);
    Plane floor2(vec3(0, 1, 0), vec3(0, 0, 0), vec3(512, 0, 512), vec3(512, 0, 0), vec3(204, 204, 0), false);



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

    Sphere Sphere1(vec3(400, 400, 50), 45, vec3(255, 30, 0), false);
    Sphere Sphere2(vec3(200, 200, 200), 100, vec3(153, 0, 76), true);

    pObjectList.push_back(&Sphere1);
    pObjectList.push_back(&Sphere2);

    Light Light1(vec3(256, 256, -300), vec3(0,0,0), vec3(0,0,0), 0.0, false);
    Light Light2(vec3(300, 200, -150), vec3(0,0,0), vec3(0,0,0), 0.0, false);

    pLightList.push_back(&Light1);
    pLightList.push_back(&Light2);


}

inline void InitializeAntiAliasing(float xp, float yp, vec3 DirArray[4])
{
    vec3 PxPos(yp - 0.25f, xp - 0.25f, 0);
    vec3 Direction = PxPos - Camera;
    Direction = normalize(Direction);
    DirArray[0] = Direction;

    PxPos = vec3(yp - 0.25f, xp + 0.25f, 0);
    Direction = PxPos - Camera;
    Direction = normalize(Direction);
    DirArray[1] = Direction;

    PxPos = vec3(yp + 0.25f, xp + 0.25f, 0);
    Direction = PxPos - Camera;
    Direction = normalize(Direction);
    DirArray[2] = Direction;

    PxPos = vec3(yp + 0.25f, xp - 0.25f, 0);
    Direction = PxPos - Camera;
    Direction = normalize(Direction);
    DirArray[3] = Direction;
}

inline bool ClosestIntersection(vec3 Direction)
{
    bool IntersectCheck = false;
    for (int i = 0; i < pObjectList.size(); ++i)
    {

        float t;
        vec3 Normal;
        vec3 Temp;

        bool DoesIntersect;
        DoesIntersect = pObjectList[i]->Intersect(Camera, Direction, &t, &Normal, &Temp);

        if (DoesIntersect)
        {
            IntersectCheck = true;
            if (t_min > t)
            {
                /*if (pObjectList[i].reflect == true)
                {
                    vec3 IntersectionPoint = pObjectList[i].ip;
                    vec3 r = Direction - 2 * dot(Direction, ObjectList[i].Normal) * ObjectList[i].Normal;
                        Temp  = CalcReflection(IntersectionPoint, r);
                 }*/
                //calc reflection colour
                if (pObjectList[i]->Reflect == true)
                {
                    vec3 IntersectionPoint = pObjectList[i]->IntP;
                    vec3 r = Direction - 2 * dot(Direction, pObjectList[i]->Normal) * pObjectList[i]->Normal;
                    Temp  = Reflection(IntersectionPoint, r);
                }


                PixelColour = Temp;
                t_min = t;
                Norm_min = Normal;
            }
        }
    }
    return IntersectCheck;
}

inline int CalculateLight(int NumShadows, vec3 Direction)
{

    vec3 Intersection = Direction * t_min;
    Intersection = Intersection + Camera;
    for (int k = 0; k < pLightList.size(); k++)
    {
        pLightList[k]->LightVector = pLightList[k]->Lightpos - Intersection;
        pLightList[k]->Length = length(pLightList[k]->LightVector);
        pLightList[k]->LightVector = normalize(pLightList[k]->LightVector);
    }
    for (int k = 0; k < pObjectList.size(); ++k)
    {
        float t;
        vec3 Normal;
        vec3 Temp;
        bool IntersectCheck;

        for (int i = 0; i < pLightList.size(); i++)
        {
            pLightList[i]->EyeVector = Intersection + (pLightList[i]->LightVector * 2.0f);

            IntersectCheck = pObjectList[k]->Intersect(pLightList[i]->EyeVector, pLightList[i]->LightVector, &t, &Normal, &Temp);

            if (IntersectCheck && t < pLightList[i]->Length)
            {
                //pLightList[i]->IsBlocked = true;
                NumShadows++;
            }
        }
    }

    /*for (int i = 0; i < pLightList.size(); i++)
    {
        pLightList[i]->IsBlocked = false;
    }*/
    return NumShadows;
}

inline void CalculateColor(int NumShadows)
{
    float DiffuseTerm = 0;
    int i;
    for (i = 0; i < pLightList.size(); i++)
    {
        DiffuseTerm += dot(pLightList[i]->LightVector, Norm_min);
    }
    DiffuseTerm = DiffuseTerm / i;
    if (DiffuseTerm > 0)
    {
        vec3 PixelDiffuseColour = DiffuseColour * DiffuseTerm;
        PixelColour = PixelColour + PixelDiffuseColour;
    }

    for (i = 0; i < NumShadows; i++)
    {
        //Shadow Intensity
        PixelColour = PixelColour * 0.3f;
    }
}

inline void RayTrace(Image* pImage)
{
    InitializeObjects();

    for (int i = 0; i < 512; ++i)
        for (int j = 0; j < 512; ++j)
        {
            float xpos = i;
            float ypos = j;
            bool NotBackground = false;

            InitializeAntiAliasing(xpos, ypos, DirArray);

            //anti-ailiasing
            for (int k = 0; k < 4; k++){
                t_min = 999999;
                //check ray intersections
                if (ClosestIntersection(DirArray[k]))
                {
                    int ShadowsNum = 0;
                    ShadowsNum = CalculateLight(ShadowsNum, DirArray[k]);

                    CalculateColor(ShadowsNum);
                    PixelArray[k] = PixelColour;
                    NotBackground = true;
                }
                else
                {
                    (*pImage)(i, j) = Pixel(0, 0, 0);
                }
            }
            if (NotBackground){
                vec3 ColourTemp = PixelArray[0] + PixelArray[1] + PixelArray[2] + PixelArray[3];
                //ColourTemp = ColourTemp + PixelArray[2];
                //ColourTemp = ColourTemp + PixelArray[3];

                ColourTemp /= 4.0f;

                SetColour(px, ColourTemp);
                (*pImage)(i, j) = px;
            }
        }
}
