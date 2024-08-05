#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h"

using namespace std;
#include "variables.h"
#include "readfile.h"

void matransform(stack<mat4> &transfstack, float *values)
{
  mat4 transform = transfstack.top();
  vec4 valvec = vec4(values[0], values[1], values[2], values[3]);
  vec4 newval = transform * valvec;
  for (int i = 0; i < 4; i++)
    values[i] = newval[i];
}

void rightmultiply(const mat4 &M, stack<mat4> &transfstack)
{
  mat4 &T = transfstack.top();
  T = T * M;
}

bool readvals(stringstream &s, const int numvals, float *values)
{
  for (int i = 0; i < numvals; i++)
  {
    s >> values[i];
    if (s.fail())
    {
      cout << "Failed reading value " << i << " will skip\n";
      return false;
    }
  }
  return true;
}

string readfile(const char *filename)
{
  attenuation = glm::vec3(1, 0, 0);
  // ambient[] = {.2, .2, .2, 1};
  for (int i = 0; i < 3; i++)
  {
    ambient[i] = 0.2;
  }
  ambient[3] = 1;
  maxdepth = 5;
  string str, cmd, fileName;
  ifstream in;
  in.open(filename);
  if (in.is_open())
  {
    stack<mat4> transfstack;
    transfstack.push(mat4(1.0));

    getline(in, str);
    while (in)
    {
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#'))
      {

        stringstream s(str);
        s >> cmd;
        int i;
        float values[10];
        bool validinput;

        if (cmd == "size")
        {
          validinput = readvals(s, 2, values);
          if (validinput)
          {
            w = (int)values[0];
            h = (int)values[1];
          }
        }
        else if (cmd == "maxdepth")
        {
          validinput = readvals(s, 1, values);
          if (validinput)
          {
            maxdepth = (int)values[0];
          }
        }
        else if (cmd == "output")
        {
          s >> fileName;
          if (s.fail())
          {
            cerr << "Could not read input file";
          }
        }
        else if (cmd == "camera")
        {
          validinput = readvals(s, 10, values);
          if (validinput)
          {
            vec3 lookfrom = vec3(values[0], values[1], values[2]);
            vec3 lookat = vec3(values[3], values[4], values[5]);

            center = lookat;

            eyeinit = lookfrom;

            vec3 directionEye = lookat - lookfrom;
            upinit = glm::normalize(vec3(values[6], values[7], values[8]));
            upinit = Transform::upvector(upinit, directionEye);
            fovy = values[9];
          }
        }
        else if (cmd == "sphere" || cmd == "tri")
        {
          if (numobjects == maxobjects)
          { // No more objects
            cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
          }
          else
          {
            object *obj = &(objects[numobjects]);

            // Set the object's light properties
            for (i = 0; i < 4; i++)
            {
              (obj->ambient)[i] = ambient[i];
              (obj->diffuse)[i] = diffuse[i];
              (obj->specular)[i] = specular[i];
              (obj->emission)[i] = emission[i];
            }
            obj->shininess = shininess;

            // Set the object's transform
            obj->transform = transfstack.top();

            // Set the object's type
            if (cmd == "sphere")
            {
              validinput = readvals(s, 4, values);
              if (validinput)
              {
                obj->type = sphere;
                (obj->sphereCoords)[0] = values[0];
                (obj->sphereCoords)[1] = values[1];
                (obj->sphereCoords)[2] = values[2];
                obj->size = values[3];
              }
              else
              {
                cerr << "Something went wrong with reading sphere";
              }
            }
            else if (cmd == "tri")
            {
              validinput = readvals(s, 3, values);
              if (validinput)
              {
                obj->type = triangle;
                (obj->triangleIndices)[0] = values[0];
                (obj->triangleIndices)[1] = values[1];
                (obj->triangleIndices)[2] = values[2];
              }
              else
              {
                cerr << "Something went wrong with reading triangle";
              }
            }

            ++numobjects;
          }
        }
        else if (cmd == "maxverts")
        {
          validinput = readvals(s, 1, values);
          if (validinput)
          {
            maxverts = values[0];
          }
        }
        else if (cmd == "maxvertnorms")
        {
          validinput = readvals(s, 1, values);
          if (validinput)
          {
            maxvertnorms = values[0];
          }
        }
        else if (cmd == "vertex")
        {
          validinput = readvals(s, 3, values);
          if (validinput)
          {
            vertices[vertexIndex] = glm::vec3(values[0], values[1], values[2]);
            vertexIndex++;
          }
        }
        // else if (cmd == "vertexnormal")
        // {
        //   validinput = readvals(s, 6, values);
        //   if (validinput)
        //   {
        //     normalVertices.push_back(objects::createNormalVertex(values[0], values[1], values[2], values[3], values[4], values[5]));
        //   }
        // }
        else if (cmd == "rotate")
        {
          validinput = readvals(s, 4, values);
          if (validinput)
          {
            vec3 axis(values[0], values[1], values[2]);
            float degrees = values[3];

            mat3 rmat = Transform::rotate(degrees, axis);

            mat4 newrmat(rmat[0][0], rmat[0][1], rmat[0][2], 0.0f, rmat[1][0], rmat[1][1], rmat[1][2], 0.0f, rmat[2][0], rmat[2][1], rmat[2][2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

            rightmultiply(newrmat, transfstack);
          }
        }
        else if (cmd == "translate")
        {
          validinput = readvals(s, 3, values);

          if (validinput)
          {
            mat4 tmat = Transform::translate(values[0], values[1], values[2]);
            rightmultiply(tmat, transfstack);
          }
        }
        else if (cmd == "scale")
        {
          validinput = readvals(s, 3, values);
          if (validinput)
          {
            float sx = values[0];
            float sy = values[1];
            float sz = values[2];

            mat4 smat = Transform::scale(sx, sy, sz);

            rightmultiply(smat, transfstack);
          }
        }
        else if (cmd == "pushTransform")
        {
          transfstack.push(transfstack.top());
        }
        else if (cmd == "popTransform")
        {
          if (transfstack.size() <= 1)
          {
            cerr << "Stack has no elements.  Cannot Pop\n";
          }
          else
          {
            transfstack.pop();
          }
        }

        // Lights
        else if (cmd == "directional")
        {
          validinput = readvals(s, 6, values); // Position/color for lts.
          if (validinput)
          {
            for (int i = 0; i < 3; i++)
            {
              lightposn[(numused * 4) + i] = values[i];
              lightcolor[(numused * 4) + i] = values[i + 3];
            }
            lightposn[3 + 4 * numused] = 0;  // Based off HW2, set light position to be directional
            lightcolor[3 + 4 * numused] = 1; // alpha always 1
            ++numused;
          }
        }
        else if (cmd == "point")
        {
          validinput = readvals(s, 6, values); // Position/color for lts.
          if (validinput)
          {
            for (int i = 0; i < 3; i++)
            {
              lightposn[(numused * 4) + i] = values[i];
              lightcolor[(numused * 4) + i] = values[i + 3];
            }
            lightposn[3 + 4 * numused] = 1;  // Based off HW2, set light position to be point
            lightcolor[3 + 4 * numused] = 1; // alpha always 1
            ++numused;
          }
        }
        else if (cmd == "attenuation")
        {
          validinput = readvals(s, 3, values);
          if (validinput)
          {
            for (int i = 0; i < 3; i++)
            {
              attenuation[i] = values[i];
            }
          }
        }
        else if (cmd == "ambient")
        {
          validinput = readvals(s, 3, values); // colors
          if (validinput)
          {
            for (int i = 0; i < 3; i++)
            {
              ambient[i] = values[i];
            }
            ambient[3] = 1;
          }
        }
        else if (cmd == "diffuse")
        {
          validinput = readvals(s, 3, values);
          if (validinput)
          {
            for (int i = 0; i < 3; i++)
            {
              diffuse[i] = values[i];
            }
            diffuse[3] = 1;
          }
        }
        else if (cmd == "specular")
        {
          validinput = readvals(s, 3, values);
          if (validinput)
          {
            for (int i = 0; i < 3; i++)
            {
              specular[i] = values[i];
            }

            specular[3] = 1;
          }
        }
        else if (cmd == "shininess")
        {
          validinput = readvals(s, 1, values);
          if (validinput)
          {
            shininess = values[0];
          }
        }
        else if (cmd == "emission")
        {
          validinput = readvals(s, 3, values);
          if (validinput)
          {
            for (int i = 0; i < 3; i++)
            {
              emission[i] = values[i];
            }

            emission[3] = 1;
          }
        }
        else
        {
          cerr << "Unknown Command: " << cmd << " Skipping \n";
        }
      }
      getline(in, str);
    }

    // Set up initial position for eye, up and amount
    // As well as booleans

    eye = eyeinit;
    up = upinit;
    amount = amountinit;
    sx = sy = 1.0;  // keyboard controlled scales in x and y
    tx = ty = 0.0;  // keyboard controllled translation in x and y

    // glEnable(GL_DEPTH_TEST);
  }
  else
  {
    cerr << "Unable to Open Input Data File " << fileName << "\n";
    throw 2;
  }
  return fileName;
}
