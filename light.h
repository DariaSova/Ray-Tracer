#include <math.h>
#include <vector>
#include "object.h"
using namespace glm;

class Light : public Object {

  public:
    vec3 Lightpos;
    vec3 LightVector;
    vec3 EyeVector;
    float Length;
    bool IsBlocked;

  Light(vec3 l, vec3 lvector, vec3 eye, float length, bool blocked): Lightpos(l), LightVector(lvector), EyeVector(eye), Length(length), IsBlocked(blocked) {}
//Light(vec3 l, bool blocked=false): Lightpos(l), IsBlocked(blocked) {}

  bool Intersect(vec3 Origin, vec3 Direction,
      float *t, vec3 *normal, vec3* tpix)
  {
      return false;
  }

};
