#ifndef __CONSTANTS__
#define __CONSTANTS__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Colors
const GLfloat orange[] = {0.97, 0.55, 0.06, 1.0};
const GLfloat lightgrey[] = {0.9, 0.9, 0.9, 1.0};
const GLfloat grey[] = {0.81, 0.81, 0.81, 1.0};
const GLfloat darkgrey[] = {0.3, 0.3, 0.3, 1.0};
const GLfloat black[] = {0.2, 0.2, 0.2, 1.0};
const GLfloat red[] = {1, 0, 0, 1.0};

// For normalizing to NDC
const GLfloat tileSize = 0.12;

// Drawing styles
const GLint STYLE1 = 1;
const GLint STYLE2 = 2;

#endif
