#include "structs.h"

class objects
{
public:
    objects();
    virtual ~objects();
    static Ray createRay(glm::vec3 orientation, glm::vec3 direction, float t, glm::vec3 n, glm::vec3 pInt, bool isRecursive);
    static Intersection createIntersection(object *min_primitive, float min_t, glm::vec3 pointIntersection, Ray intersectionRay, glm::vec3 normal);
};