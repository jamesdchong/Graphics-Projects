// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

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

    up = normalize(axis_angle * up);

}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {

    vec3 a = center;

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

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    float theta = glm::radians(fovy) / 2.0f;
    float d = 1.0f / tanf(theta);
    float n = zNear;
    float f = zFar;
    
    float A = -1.0f * ((f+n)/(f-n));
    float B = -1.0f * ((2.0f*f*n) / (f-n));
    
    mat4 persp(d/aspect,0.0f,0.0f,0.0f, 0.0f,d,0.0f,0.0f, 0.0f,0.0f,A,B, 0.0f,0.0f,-1.0f,0.0f);
    persp = glm::transpose(persp);
    
    return persp;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 scaleMat(sx,0.0f,0.0f,0.0f,  0.0f,sy,0.0f,0.0f,  0.0f,0.0f,sz,0.0f,  0.0f,0.0f,0.0f,1.0f);
    
    scaleMat = glm::transpose(scaleMat);
    
    return scaleMat;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{

    mat4 transMat(1.0f,0.0f,0.0f,tx, 0.0f,1.0f,0.0f,ty, 0.0f,0.0f,1.0f,tz, 0.0f,0.0f,0.0f,1.0f);
    
    transMat = glm::transpose(transMat);
    
    return transMat;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
