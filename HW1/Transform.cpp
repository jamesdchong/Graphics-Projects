// Transform.cpp: implementation of the Transform class.

#include "Transform.h"

// Helper rotation function.
mat3 Transform::rotate(const float degrees, const vec3& axis) {

    // Normalize axis angle
    vec3 normAxis = glm::normalize(axis);
    
    // Identity Matrix
    mat3 identity(1.0f);
    
    // Cosine, Sine
    float cosineTheta = cos(glm::radians(degrees));
    float sineTheta = sin(glm::radians(degrees));
    
    // Outer Product
    mat3 outerProduct(normAxis.x * normAxis.x, normAxis.x * normAxis.y, normAxis.x * normAxis.z, normAxis.y * normAxis.x, normAxis.y * normAxis.y, normAxis.y * normAxis.z, normAxis.z * normAxis.x, normAxis.z * normAxis.y, normAxis.z * normAxis.z);
    
    // A*
    mat3 dualMatrix(0.0f, -1.0f * normAxis.z, normAxis.y, normAxis.z, 0.0f, -1.0f * normAxis.x, -1.0f * normAxis.y, normAxis.x, 0.0f);
    dualMatrix = glm::transpose(dualMatrix);
    
    // cos * I + (1-cos) * aaT + sin * A*
    mat3 axis_angle = (cosineTheta * identity) + ((1.0f - cosineTheta) * outerProduct) + (sineTheta * dualMatrix);
    
    return axis_angle;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
    
    mat3 axis_angle = rotate(degrees, up);
    
    eye = axis_angle * eye;
    
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
    
    vec3 orth = glm::cross(eye, up);
    
    mat3 axis_angle = rotate(degrees, orth);
    
    eye = axis_angle * eye;
    
    up = axis_angle * up;

}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {

    vec3 a = vec3(0.0f); // center

    vec3 b = glm::normalize(up); // up

    vec3 w = glm::normalize(eye - a);

    vec3 u = glm::normalize(glm::cross(b, w));

    vec3 v = glm::cross(w, u);

    mat4 rot(u.x, u.y, u.z, 0.0f,  v.x, v.y, v.z, 0.0f,  w.x, w.y, w.z, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f);
    
    rot = glm::transpose(rot);

    mat4 trans(1.0f, 0.0f, 0.0f, -1.0f * eye.x,  0.0f, 1.0f, 0.0f, -1.0f * eye.y,  0.0f, 0.0f, 1.0f, -1.0f * eye.z,  0.0f, 0.0f, 0.0f, 1.0f);
    
    trans = glm::transpose(trans);

    mat4 lookAt = rot * trans;

    return lookAt;
    
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
