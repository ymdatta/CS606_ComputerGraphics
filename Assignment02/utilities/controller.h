#ifndef __CONTROLLER__
#define __CONTROLLER__


#include "../models/mesh.h"
#include <vector>

class Controller {

  std::vector<Mesh> models;

  int mesh_i;
  int poly_mode = 0;
  GLuint programID;

  glm::vec3 startAngle;
  glm::vec3 endAngle;
  
 public:

  GLFWwindow *window;
  int key, code, action, mode;

  glm::vec3 lightPos = glm::vec3(1.0f, 0, 0);

  void setProgramId(GLuint id) {
    programID = id;
  }
  void addModel(char s[]);
  
  void createAndBind();
  void renderModels();

  glm::vec3 getCursorPos(GLFWwindow *window);
  void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
};

#endif

