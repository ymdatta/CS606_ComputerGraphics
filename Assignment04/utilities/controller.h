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
  std::vector<std::vector <int> > child;
  
 public:

  GLFWwindow *window;
  int key, code, action, mode, rotate_t = 0;
  unsigned int tex_change = 0;

  float c_angle = 0, up_dir = 0.8f, c_speed = 1;

  glm::vec3 lightPos = glm::vec3(1.0f, 0, 0);
  glm::vec3 followVec = glm::vec3(-1.0f, 0.0f, 0.0f);
  unsigned *texBuff;

  void setProgramId(GLuint id) {
    programID = id;
  }
  void addModel(char s[]);
  void createAndBind();
  void renderModels();
  void loadTexs();
  void createTexs();
  void rotateCircle();
  void rotateVertical();

  glm::vec3 getCursorPos(GLFWwindow *window);

  void handleMouse(GLFWwindow *window, int button, int action, int mode);   
  void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
};

#endif

