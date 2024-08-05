#ifdef MAINPROGRAM
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN int amount;    // The amount of rotation for each arrow press
EXTERN glm::vec3 eye; // The (regularly updated) vector coordinates of the eye
EXTERN glm::vec3 up;  // The (regularly updated) vector coordinates of the up
EXTERN int maxdepth;
EXTERN glm::vec3 attenuation;
EXTERN int currDepth;
EXTERN float objSpeculars[100];

#ifdef MAINPROGRAM
glm::vec3 eyeinit(0.0, 0.0, 5.0); // Initial eye position, also for resets
glm::vec3 upinit(0.0, 1.0, 0.0);  // Initial up position, also for resets
glm::vec3 center(0.0, 0.0, 0.0);  // Center look at point
int amountinit = 5;
// int w = 500, h = 500; // width and height
int w, h;
int maxdepthinit = 5;
float fovy; // For field of view
glm::vec3 attenuationinit(1, 0, 0);
#else
EXTERN glm::vec3 eyeinit;
EXTERN glm::vec3 upinit;
EXTERN glm::vec3 center;
EXTERN int amountinit;
EXTERN int w, h;
EXTERN float fovy;
#endif

EXTERN int maxverts;
EXTERN int maxvertnorms;

EXTERN glm::vec3 vertices[1000000];
EXTERN int vertexIndex;

static enum { view,
              translate,
              scale } transop; // which operation to transform
enum shape
{
    sphere,
    triangle
};
EXTERN float sx, sy; // the scale in x and y
EXTERN float tx, ty; // the translation in x and y

// Lighting parameter array, similar to that in the fragment shader
const int numLights = 10;
EXTERN float lightposn[4 * numLights];  // Light Positions
EXTERN float lightcolor[4 * numLights]; // Light Colors
EXTERN float lightransf[4 * numLights]; // Lights transformed by modelview
EXTERN int numused;                     // How many lights are used

// Materials (read from file)
// With multiple objects, these are colors for each.
EXTERN float ambient[4];
EXTERN float diffuse[4];
EXTERN float specular[4];
EXTERN float shininess;
EXTERN float emission[4];

// For multiple objects, read from a file.
const int maxobjects = 1000000;
EXTERN int numobjects;
EXTERN struct object
{
    shape type;
    float size;
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float shininess;
    glm::vec3 sphereCoords;
    int triangleIndices[3];
    glm::mat4 transform;
} objects[maxobjects];