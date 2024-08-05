#include <iostream>
#include <vector>
#include <stack>
#include <FreeImage.h>
#include <iostream>
#include <algorithm>

using namespace std;

#define MAINPROGRAM
#include "objects.h"
#include "readfile.h"

Ray findIntersection(Ray ray, object *primitive)
{
  float t;
  Ray currRay;

  if (primitive->type == sphere)
  {
    glm::mat4 invTransf = inverse(primitive->transform);
    glm::vec4 p0 = invTransf * glm::vec4(ray.p0, 1.0f);
    glm::vec4 p1 = invTransf * glm::vec4(ray.p1, 0.0f);

    currRay.p0 = glm::vec3(p0);
    currRay.p1 = glm::vec3(p1);

    float a = dot(currRay.p1, currRay.p1);
    float b = 2 * dot(currRay.p1, (currRay.p0 - primitive->sphereCoords));
    float c = dot(currRay.p0 - primitive->sphereCoords, currRay.p0 - primitive->sphereCoords) - (primitive->size * primitive->size);

    float discr = b * b - 4 * a * c;

    // Must be non negative
    if (discr >= 0)
    {
      float root1 = (-b + sqrt(discr)) / (2.0f * a);
      float root2 = (-b - sqrt(discr)) / (2.0f * a);

      if (root1 < 0 && root2 > 0)
      {
        t = root2;
        currRay.t = t;
        // currRay.n = n;
        // currRay.pInt = pInt;
        return currRay;
      }
      else if (root1 > 0 && root2 < 0)
      {
        t = root1;
        currRay.t = t;
        // currRay.n = n;
        // currRay.pInt = pInt;
        return currRay;
      }
      else if (root1 > 0 && root2 > 0)
      {
        t = min(root1, root2);
        currRay.t = t;
        // currRay.n = n;
        // currRay.pInt = pInt;
        return currRay;
      }
      else
      {
        // cerr << "Something went wrong with roots 1: " << root1 << " root2: " << root2 << endl;
        t = -1;
        currRay.t = t;
        return currRay;
      }
    }
  }
  else if (primitive->type == triangle)
  {
    // Vertices transformed
    glm::vec3 a = glm::vec3(primitive->transform * glm::vec4(vertices[primitive->triangleIndices[0]], 1.0f));
    glm::vec3 b = glm::vec3(primitive->transform * glm::vec4(vertices[primitive->triangleIndices[1]], 1.0f));
    glm::vec3 c = glm::vec3(primitive->transform * glm::vec4(vertices[primitive->triangleIndices[2]], 1.0f));

    // normal
    glm::vec3 n = normalize(cross(b - a, c - a));

    float t = (dot(a, n) - dot(ray.p0, n)) / dot(ray.p1, n);

    glm::vec3 p = ray.p0 + ray.p1 * t;

    glm::vec3 apNormal = cross(n, c - b) / (dot(cross(n, c - b), a - c));
    float apW = dot(-apNormal, c);

    glm::vec3 bpNormal = cross(n, a - c) / (dot(cross(n, a - c), b - a));
    float bpW = dot(-bpNormal, a);

    glm::vec3 cpNormal = cross(n, b - a) / (dot(cross(n, b - a), c - b));
    float cpW = dot(-cpNormal, b);

    float checkA = dot(apNormal, p) + apW;
    float checkB = dot(bpNormal, p) + bpW;
    float checkC = dot(cpNormal, p) + cpW;

    // Inside of triangle if passes
    if (checkA >= 0 && checkA <= 1 && checkB >= 0 && checkB <= 1 && checkC >= 0 && checkC <= 1)
    {
      currRay.p0 = ray.p0;
      currRay.p1 = ray.p1;
      currRay.t = t;
      currRay.n = n;
      currRay.pInt = p;
      return currRay;
    }
  }

  if (t < 0)
    cout << "triangle is wrong" << endl;
  currRay.t = -1;
  return currRay;
}

Intersection Intersect(Ray ray)
{
  float min_t = INFINITY;
  object *min_primitive = NULL;
  glm::vec3 pointIntersection;
  Ray intersectionRay;
  glm::vec3 normal;

  Intersection currIntersection;

  for (int i = 0; i < numobjects; i++)
  {
    object *primitive = &(objects[i]);
    Ray currRay = findIntersection(ray, primitive);

    if (currRay.t > 0 && currRay.t < min_t)
    {
      min_primitive = primitive;
      min_t = currRay.t;
      intersectionRay = currRay;

      if (min_primitive->type == sphere)
      {
        glm::mat4 invTransf = inverse(min_primitive->transform);
        glm::vec3 P = currRay.p0 + min_t * currRay.p1;
        glm::vec4 nSphere = glm::vec4(P - min_primitive->sphereCoords, 0.0f);
        normal = normalize(glm::vec3(transpose(invTransf) * nSphere));

        glm::vec4 P_homog = min_primitive->transform * glm::vec4(P, 1.0f);
        pointIntersection = glm::vec3(P_homog / P_homog[3]);
      }
      if (min_primitive->type == triangle)
      {
        normal = currRay.n;
        pointIntersection = currRay.pInt;
      }
    }
  }

  currIntersection.min_primitive = min_primitive;
  currIntersection.min_t = min_t;
  currIntersection.pointIntersection = pointIntersection;
  currIntersection.intersectionRay = intersectionRay;
  currIntersection.normal = normal;

  return currIntersection;
}

Ray RayThruPixel(float i, float j)
{
  glm::vec3 wVector = normalize(eye - center);
  glm::vec3 uVector = normalize(cross(up, wVector));
  glm::vec3 vVector = normalize(cross(wVector, uVector));

  float aspect = w / (float)h;

  float fovyRadians = glm::radians(fovy);

  Ray r;                                                                                    // Declare Ray r
  float alpha = tan(fovyRadians / 2.0f) * aspect * ((j - ((double)w / 2.0f)) / (w / 2.0f)); // Calculate alpha
  float beta = tan(fovyRadians / 2.0f) * ((((double)h / 2.0f) - i) / (h / 2.0f));           // Calculate beta
  glm::vec3 direction = normalize((alpha * uVector) + (beta * vVector) - wVector);          // Calculate ray's direction
  r.p0 = eye;                                                                               // origin of ray is the eye vector
  r.p1 = direction;                                                                         // set calculated direction

  return r;
}

float max(float a, float b)
{
  if (a >= b)
  {
    return a;
  }
  else
  {
    return b;
  }
}

glm::vec4 ComputeLight(glm::vec3 direction, glm::vec4 lightcolor, glm::vec3 normal, glm::vec3 halfvec, glm::vec4 mydiffuse, glm::vec4 myspecular, float myshininess)
{

  float nDotL = dot(normal, direction);
  // cout << "1: " << nDotL << endl;

  glm::vec4 lambert = mydiffuse * lightcolor * max(nDotL, 0.0f);

  float nDotH = dot(normal, halfvec);
  glm::vec4 phong = myspecular * lightcolor * pow(max(nDotH, 0.0f), myshininess);
  // cout << "P: " << phong[0] << phong[1] << phong[2] << phong[3] << " l: " << lambert[0] << lambert[1] << lambert[2] << lambert[3] << endl;
  glm::vec4 retval = lambert + phong;
  return retval;
}

Ray FindColor(Intersection hit, BYTE *pixels)
{
  object *currObject = hit.min_primitive;

  Ray recursiveRay;

  // Object isn't null
  if (currObject)
  {
    glm::vec4 rayFinalColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    glm::vec3 rayDirection = normalize(hit.intersectionRay.p1);
    glm::vec3 recurseDirection = normalize(rayDirection - 2.0f * dot(rayDirection, hit.normal) * hit.normal); // algorithm for reflected ray from paulbourke.net/geometry/reflected
    recursiveRay.p0 = hit.pointIntersection + (0.001f * hit.normal);
    // recursiveRay.p0 = hit.pointIntersection;
    recursiveRay.p1 = recurseDirection;

    // Query for all lights
    for (int i = 0; i < numused; i++)
    {
      glm::vec3 position = glm::vec3(lightposn[i * 4], lightposn[i * 4 + 1], lightposn[i * 4 + 2]);
      glm::vec4 Li = glm::vec4(lightcolor[i * 4], lightcolor[i * 4 + 1], lightcolor[i * 4 + 2], 1.0f);
      glm::vec3 direction;

      float V = 1.0f;
      float divisor = 1.0f;

      float lightType = lightposn[i * 4 + 3];
      // Check if directional or point light, 1 if point, 0 if directional
      if (lightType)
      {
        direction = position - hit.pointIntersection;
        float r = length(direction);

        Ray shadowRay;
        shadowRay.p0 = hit.pointIntersection + (direction * 0.001f);
        shadowRay.p1 = direction;
        Intersection visible = Intersect(shadowRay);
        direction = normalize(direction);

        // used to divide the Light intensity
        divisor = 1.0f / (attenuation[0] + attenuation[1] * r + attenuation[2] * r * r);
        if (visible.min_t < 1)
        {
          // cout << "not visible" << endl;
          V = 0.0f;
        }
      }
      else
      {
        // flip directional light @666
        direction = normalize(position);
        Ray shadowRay;
        shadowRay.p0 = hit.pointIntersection + (direction * 0.0001f);
        shadowRay.p1 = direction;
        Intersection visible = Intersect(shadowRay);
        if (visible.min_t != INFINITY)
        {
          // cout << "not visible" << endl;
          V = 0.0f;
        }
      }

      // Store object's specular within objSpeculars upon recursion
      objSpeculars[(currDepth - 1) * 3] = (currObject->specular)[0];
      objSpeculars[((currDepth - 1) * 3) + 1] = (currObject->specular)[1];
      objSpeculars[((currDepth - 1) * 3) + 2] = (currObject->specular)[2];

      glm::vec3 halfVector = normalize(direction + rayDirection);
      glm::vec4 objDiffuse = glm::vec4((currObject->diffuse)[0], (currObject->diffuse)[1], (currObject->diffuse)[2], (currObject->diffuse)[3]);
      glm::vec4 objSpecular = glm::vec4((currObject->specular)[0], (currObject->specular)[1], (currObject->specular)[2], (currObject->specular)[3]);

      // Good: Li, direction, diffuse, specular, shiny
      // normal, check pointIntersection from halfVector last

      // cout << "li is: " << Li[0] << Li[1] << Li[2] << endl;
      // cout << "normal: " << hit.normal[0] << " " << hit.normal[1] << " " << hit.normal[2] << endl;
      glm::vec4 additive = ComputeLight(direction, Li, hit.normal, halfVector, objDiffuse, objSpecular, currObject->shininess);

      // cout << "v is: " << V << " additive are: " << additive[0] << additive[1] << additive[2] << endl;
      rayFinalColor += additive * divisor * V;
    }

    float ambient_0 = (currObject->ambient)[2];
    float ambient_1 = (currObject->ambient)[1];
    float ambient_2 = (currObject->ambient)[0];

    float emission_0 = (currObject->emission)[2];
    float emission_1 = (currObject->emission)[1];
    float emission_2 = (currObject->emission)[0];

    BYTE reflectColor[3];
    reflectColor[0] = (ambient_0 + emission_0 + rayFinalColor[2]) * 255;
    reflectColor[1] = (ambient_1 + emission_1 + rayFinalColor[1]) * 255;
    reflectColor[2] = (ambient_2 + emission_2 + rayFinalColor[0]) * 255;

    if (currDepth == 1)
    {
      // Base case. No specular added
      pixels[0] = reflectColor[0];
      pixels[1] = reflectColor[1];
      pixels[2] = reflectColor[2];
    }
    else
    {
      // multiply speculars of bounces with color
      for (int i = 0; i < currDepth - 1; i++)
      {
        reflectColor[0] *= objSpeculars[i * 3 + 2];
        reflectColor[1] *= objSpeculars[i * 3 + 1];
        reflectColor[2] *= objSpeculars[i * 3];
      }

      // Add color of reflection to previous pixel (sum of IS*IK)
      pixels[0] += reflectColor[0];
      pixels[1] += reflectColor[1];
      pixels[2] += reflectColor[2];
    }
    recursiveRay.isRecursive = true;
    return recursiveRay;
  }
  else // Ray doesn't hit anything
  {
    // If first ray, color black
    if (currDepth == 1)
    {
      pixels[0] = 0.0f;
      pixels[1] = 0.0f;
      pixels[2] = 0.0f;
    }

    // Stop recursing
    recursiveRay.isRecursive = false;
    return recursiveRay;
  }
}

void Raytrace(Ray ray, BYTE *pixels)
{
  // int counter = 0;

  // Account for reflected ray depth
  currDepth++;

  Intersection hit = Intersect(ray);

  Ray recursiveRay = FindColor(hit, pixels);

  if (recursiveRay.isRecursive == true && currDepth < maxdepth)
  {
    recursiveRay.isRecursive = false;
    Raytrace(recursiveRay, pixels);
  }
  // counter += 3;
  // return pixelArray;
}

int main(int argc, char *argv[])
{

  // Check for 1 input file
  if (argc != 2)
  {
    cerr << "Need only 1 input file\n";
    exit(-1);
  }

  // Initialize FreeImage
  FreeImage_Initialise();

  string fileName = readfile(argv[1]);

  if (fileName.empty())
  {
    fileName = "output.png";
  }

  BYTE pixels[3 * w * h]; // length of pixel array = 3 colors * number of pixels

// // Raytrace, returns pointer to beginning of array of pixels
#pragma omp ordered
  for (int i = 0; i < h; i++)
  {
    cout << "iteration: " << i << endl;
    for (int j = 0; j < w; j++)
    {
      // Reset depth for each pixel
      currDepth = 0;

      float m = (float)i + 0.5f; // Center of pixel instead of corner
      float n = (float)j + 0.5f;

      Ray ray = RayThruPixel(m, n); // Get ray at each pixel

      // Raytrace @ pixel memory lo
      Raytrace(ray, &pixels[(i * w + j) * 3]);
    }
  }

  // Converts to FIBITMAP object in RGB order
  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);

  // Save image as PNG
  FreeImage_Save(FIF_PNG, img, fileName.c_str(), 0);

  // Exit cleanly
  FreeImage_DeInitialise();
}