#pragma once
using namespace glm;

class Object
{

public:

    vec3 Center;
    float Radius;
    vec3 ObjectColor;
    bool Reflect;
    vec3 IntP;
    vec3 Normal;

    Object(vec3 Center_in=vec3(0,0,0), float Radius_in=0, vec3 ObjectColor_in=vec3(0,0,0), bool reflect_in=false) : Center(Center_in), Radius(Radius_in) , ObjectColor(ObjectColor_in), Reflect(reflect_in) {}
    //camera was origin
    virtual bool Intersect(vec3 Camera, vec3 Direction, float  *t, vec3 * normal, vec3* tpix) = 0;

};
