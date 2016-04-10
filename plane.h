#ifdef WIN_COMPILE
#include "glew.h"
#endif
#include <math.h>
#include <vector>
using namespace glm;

class Plane : public Object {
  vec3 p0;
  vec3 p1;
  vec3 p2;
  vec3 Normal;
  vec3 Color;
  public:

  Plane(vec3 normal, vec3 p0, vec3 p1, vec3 p2, vec3 color): Normal(normal), p0(p0), p1(p1), p2(p2), Color(color) {}

bool Intersect(vec3 Origin, vec3 Direction,
    float *t, vec3 *normal, vec3* tpix)
{
  vec3 e1 = p1 - p0;
  vec3 e2 = p2 - p0;
  vec3 P = cross(Direction, e2);
  float a = dot(e1, P);

  if (a > -1E-5f && a < 1E-5f) return false;
  float f = 1 / a;
  vec3 s = Origin - p0;
  float u = f * dot(s, P);

  if (u < 0.0f || u > 1.0f) return false;
  vec3 q = cross(s, e1);
  float p = f * dot(Direction, q);
  if (p < 0.0f || u + p > 1.0f) return false;

  if(f * dot(e2, q)>0)
  {
    *t = f * dot(e2, q);
    *tpix = ObjectColor;
    *normal = Normal;

    return true;
  }
  else
    return false;

      /*float denom = dot(Normal, Direction);

        if(denom > 1e-6) { //very small number
        vec3 p0 = Point - Origin;
       *t = dot(p0, Normal)/denom;
       *normal = Normal;
       return(t>=0);
       }

       return false;*/

}
};
