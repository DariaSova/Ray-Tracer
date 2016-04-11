#pragma once
#include <math.h>
#include "object.h"


class Sphere : public Object {
public:

   Sphere(vec3 Center_in, float Radius_in, vec3 ObjectColor_in, bool reflect_in) : Object(Center_in, Radius_in, ObjectColor_in, reflect_in) {}

    bool Intersect(vec3 Origin, vec3 Direction,
                           float *t, vec3 *normal, vec3* tpix)
    {
        vec3 EO = Center - Origin;
        float v = dot(EO, Direction);
        float RadiusSquare = Radius * Radius;
        float EO_Square = dot(EO, EO);
        float discriminant = RadiusSquare - (EO_Square - v * v);

        float t_min = -1;
        if (discriminant > 0)
        {
            float d = sqrt(discriminant);
            t_min = v - d;
        }

        if (t_min > 0) {
            *t = t_min;
            vec3 IntersectionPoint = Direction*(t_min);
            IntersectionPoint = IntersectionPoint + Origin;
            IntP= IntersectionPoint;
            vec3 SurfaceNormal = IntersectionPoint - Center;
            Normal = normalize(SurfaceNormal);
            (*normal) = normalize(SurfaceNormal);
            *tpix = ObjectColor;
            return true;
        } else
        {
            return false;
        }
    }
};


