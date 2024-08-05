#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "variables.h"

struct Ray
{
    glm::vec3 p0;
    glm::vec3 p1;
    float t;
    glm::vec3 n;
    glm::vec3 pInt;
    bool isRecursive;
};

struct Camera
{
    glm::vec3 eye, center, up, a, b, w, u, v;
    int fovy, fovx;
};

struct Intersection
{
    object *min_primitive;
    float min_t;
    glm::vec3 pointIntersection;
    Ray intersectionRay;
    glm::vec3 normal;
};
