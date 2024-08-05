#include "objects.h"

Ray objects::createRay(glm::vec3 origin, glm::vec3 direction, float t, glm::vec3 n, glm::vec3 pInt, bool isRecursive)
{
    Ray r;

    r.p0 = origin;
    r.p1 = direction;
    r.t = t;
    r.n = n;
    r.pInt = pInt;
    r.isRecursive = false;

    return r;
}

Intersection objects::createIntersection(object *min_primitive, float min_t, glm::vec3 pointIntersection, Ray intersectionRay, glm::vec3 normal)
{
    Intersection intersection;

    intersection.min_primitive = min_primitive;
    intersection.min_t = min_t;
    intersection.pointIntersection = pointIntersection;
    intersection.intersectionRay = intersectionRay;
    intersection.normal = normal;

    return intersection;
}