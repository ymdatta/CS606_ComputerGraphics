#ifndef __CONTROLLER__
#define __CONTROLLER__


#include "../models/mesh.h"
#include <string>
#include <vector>
#include <stdlib.h>

class Controller {

  std::vector<Mesh> models;

  int mesh_i;  
  GLuint programID, cord_change = 0;

  glm::vec3 startAngle;
  glm::vec3 endAngle;
  std::vector<std::string> textStringVector;
  
 public:

  GLFWwindow *window;
  int key, code, action, mode;
  unsigned int tex_change = 0;

  glm::vec3 lightPos = glm::vec3(1.0f, 0, 0);
  unsigned *texBuff;

  void setProgramId(GLuint id) {
    programID = id;
  }
  void addModel(char s[]);
  void createAndBind();
  void renderModels();
  void loadTexs();
  void createTexs();

  glm::vec3 getCursorPos(GLFWwindow *window);

  void handleMouse(GLFWwindow *window, int button, int action, int mode);   
  void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
};

#endif

