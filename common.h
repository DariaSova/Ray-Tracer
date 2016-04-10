#pragma once
#include "Image.h"
#include <math.h>
#include <iostream>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/mat3x3.hpp>
#include <string>

using namespace glm;

vec3 Camera(256, 256, -400);
vec3 DiffuseColour(255, 128, 128);

struct ObjectStruct
{
        //0 = sphere, 1 = triangle
        int Object;

        //Sphere
        float Radius;
        vec3 Center;
        vec3 SphereColour;

        //Plane
        vec3 Normal;
        vec3 v0;
        vec3 v1;
        vec3 v2;
        vec3 PlaneColour;

        vec3 Point;

        ObjectStruct()
        {
                Object = 0;

                //SphereTval = 0;
                Radius = 0;
                Center = vec3(0, 0, 0);
                SphereColour = vec3(0, 0, 0);

                //Triangle
                Normal = vec3(0, 0, 0);
                v0 = vec3(0, 0, 0);
                v1 = vec3(0, 0, 0);
                v2 = vec3(0, 0, 0);
                PlaneColour = vec3(0, 0, 0);

                Point = vec3(0, 0, 0);
        }

        void SphereInit(float x, vec3 y, vec3 z)
        {
                Object = 0;
                Radius = x;
                Center = y;
                SphereColour = z;
        }

        void PlaneInit(vec3 x, vec3 y, vec3 z)
        {
                Object = 1;
                Normal = x;
                Point = y;
                PlaneColour = z;
        }

        void TriangleInit(vec3 n, vec3 x, vec3 y, vec3 z, vec3 c)
        {
                Object = 1;
                Normal = n;
                v0 = x;
                v1 = y;
                v2 = z;
                PlaneColour = c;
        }

        virtual bool IsSphereIntersect(vec3 Origin, vec3 Direction, float* t_out, vec3* normal_out, vec3* tpix)
        {
                vec3 EO = Center - Origin;
                float v = dot(EO, Direction);
                float RadiusSquare = Radius * Radius;
                float EO_Square = dot(EO, EO);
                float discriminant = RadiusSquare - (EO_Square - v * v);

                float t = -1;
                if (discriminant > 0)
                {
                        float d = sqrt(discriminant);
                        t = v - d;
                }

                if (t > 0)
                {
                        *t_out = t;
                        vec3 IntersectionPoint = Direction * t;
                        IntersectionPoint = IntersectionPoint + Origin;
                        vec3 SurfaceNormal = IntersectionPoint - Center;
                        *normal_out = normalize(SurfaceNormal);
                        *tpix = SphereColour;
                        return true;
                }
                return false;
        }

        virtual bool IsPlaneIntersect(vec3 Origin, vec3 Direction, float* t, vec3* normal, vec3* tpix)
        {
                vec3 e1 = v1 - v0;
                vec3 e2 = v2 - v0;
                vec3 P = cross(Direction, e2);
                float a = dot(e1, P);

                if (a > -1E-5f && a < 1E-5f) return false;
                float f = 1 / a;
                vec3 s = Origin - v0;
                float u = f * dot(s, P);

                if (u < 0.0f || u > 1.0f) return false;
                vec3 q = cross(s, e1);
                float v = f * dot(Direction, q);
                if (v < 0.0f || u + v > 1.0f) return false;

                if(f * dot(e2, q)>0)
                {
                 *t = f * dot(e2, q);
                 *tpix = PlaneColour;
                 *normal = Normal;

                 return true;
                 }
                 else
                 return false;

                /*mat3 AMat = {
                        v0.x - v1.x, v0.x - v2.x, Direction.x,
                        v0.y - v1.y, v0.y - v2.y, Direction.y,
                        v0.z - v1.z, v0.z - v2.z, Direction.z
                };
                AMat = transpose(AMat);
                float ADet = determinant(AMat);

                mat3 YMat = {
                        v0.x - v1.x, v0.x - Origin.x, Direction.x,
                        v0.y - v1.y, v0.y - Origin.y, Direction.y,
                        v0.z - v1.z, v0.z - Origin.z, Direction.z
                };
                YMat = transpose(YMat);
                float Y = determinant(YMat) / ADet;

                if (Y < 0 || Y > 1) return false;

                mat3 BMat = {
                        v0.x - Origin.x, v0.x - v2.x, Direction.x,
                        v0.y - Origin.y, v0.y - v2.y, Direction.y,
                        v0.z - Origin.z, v0.z - v2.z, Direction.z
                };
                BMat = transpose(BMat);
                float B = determinant(BMat) / ADet;

                if (B < 0 || B > 1 - Y) return false;

                mat3 TMat = {
                        v0.x - v1.x, v0.x - v2.x, v0.x - Origin.x,
                        v0.y - v1.y, v0.y - v2.y, v0.y - Origin.y,
                        v0.z - v1.z, v0.z - v2.z, v0.z - Origin.z
                };
                TMat = transpose(TMat);
                int tval = determinant(TMat) / ADet;

                if (tval > 0){
                        *t = determinant(TMat) / ADet;
                        *tpix = PlaneColour;
                        *normal = Normal;
                        return true;
                }
                return false;*/
        }

        /*virtual float CalculatePlaneT(vec3 Origin, vec3 Direction)
        {
                return ((dot(Point, Normal) - dot(Normal, Origin)) /
                (dot(Normal, Direction)));
        }

        bool IsPlaneIntersect(vec3 Origin, vec3 Direction, float* t, vec3* normal, vec3* tpix)
        {
                if (CalculatePlaneT(Origin, Direction) >= 0)
                {
                        *tpix = PlaneColour;
                        *t = CalculatePlaneT(Origin, Direction);
                        (*normal) = Normal;
                        return true;
                }
                return false;
        }*/
};

inline bool ObjectEqual(ObjectStruct a, ObjectStruct b)
{
        if (a.Object == b.Object &&
                a.Radius == b.Radius &&
                a.Center == b.Center &&
                a.SphereColour == b.SphereColour &&
                a.Normal == b.Normal &&
                a.v0 == b.v0 &&
                a.v1 == b.v1 &&
                a.v2 == b.v2 &&
                a.Point == b.Point &&
                a.PlaneColour == b.PlaneColour) return true;
        return false;
}

class LightStruct
{
public:
        vec3 Light;
        vec3 LightVector;
        vec3 EyeVector;
        float Length;
        bool IsBlocked;

        LightStruct(float x, float y, float z)
        {
                Light = vec3(x, y, z);
                IsBlocked = false;
        }
};

