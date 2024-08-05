// Readfile definitions

void matransform(stack<glm::mat4> &transfstack, float *values);
void rightmultiply(const glm::mat4 &M, stack<glm::mat4> &transfstack);
bool readvals(stringstream &s, const int numvals, float *values);
string readfile(const char *fileName);
