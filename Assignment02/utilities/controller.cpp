#include "controller.h"

void Controller::addModel(char s[]) {
  Mesh m_new;
  m_new.addModel(s);
  models.push_back(m_new);
}

void Controller::createAndBind() {
  for(unsigned int i = 0; i < models.size(); i++) {
    Mesh& m = models[i];
    m.normalise();
    m.createVertexBuffer();
    m.createIndexBuffer();
    m.createNormalColors();
    // First create Normal colors, then Buffer.
    m.createColorBuffer();
    m.bindOnce();
    m.initializeCube();

    // Manually setting up the models.
    if (i == 1) {
      for (int j = 0; j < 12; j++) {
	m.translateLeft();
      }
    }
    if (i == 2) {
      for (int j = 0; j < 12; j++) {
	m.translateRight();
      }      
    }    
      
  }
}

void Controller::renderModels() {

  GLuint fId = glGetUniformLocation(programID, "fId");
  glUniform1i(fId, 0);
  
  GLuint lPosId = glGetUniformLocation(programID, "lPos");
  glUniform3f(lPosId, lightPos.x, lightPos.y, lightPos.z);
  
  for(unsigned int i = 0; i < models.size(); i++) {
    Mesh& m = models[i];
    m.renderMesh(programID);
  }
  glUniform1i(fId, 1);
  glPointSize(10);
  glBindVertexArray(models[0].getVAO());
  glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);
  glPointSize(1);  
}

glm::vec3 Controller::getCursorPos(GLFWwindow *window) {

  double xpos_t = 0, ypos_t = 0;
  glfwGetCursorPos(window, &xpos_t, &ypos_t);

  xpos_t = (2 * xpos_t / 800) - 1;
  ypos_t = 1 - (2 * ypos_t / 650);

  return glm::vec3(xpos_t, ypos_t, 0);
}

void Controller::handleKeys(GLFWwindow *window, int key, int code, int action, int mode) {

  if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
    mesh_i = 0;
  }  

  if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
    mesh_i = 1;
  }

  if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
    mesh_i = 2;
  }

  if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
    mesh_i = 3;
  }

  if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
    mesh_i = 4;
  }  

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
    models[mesh_i].scaleUp();
  }

  if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
    models[mesh_i].scaleDown();
  }

  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
    models[mesh_i].translateLeft();
  }

  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
    models[mesh_i].translateRight();
  }

  if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    models[mesh_i].translateUp();
  }

  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    models[mesh_i].translateDown();
  }

  if (key == 'A' && action == GLFW_PRESS) {
    models[mesh_i].rotateRight();
  }
  if (key == 'D' && action == GLFW_PRESS) {
    models[mesh_i].rotateRight();
  }

  if (key == 'I' && action == GLFW_PRESS) {
    lightPos.y += 0.05;
    lightPos.y = glm::min(1.0f, lightPos.y);
  }
  if (key == 'J' && action == GLFW_PRESS) {
    lightPos.x -= 0.05;
    lightPos.x = glm::max(-1.0f, lightPos.x);    
  }  
  if (key == 'K' && action == GLFW_PRESS) {
    lightPos.y -= 0.05;
    lightPos.y = glm::max(-1.0f, lightPos.y);    
  }
  if (key == 'L' && action == GLFW_PRESS) {
    lightPos.x += 0.05;
    lightPos.x = glm::min(1.0f, lightPos.x);    
  }

  if (key == 'S' && action == GLFW_PRESS) {
    for(unsigned int i = 0; i < models.size(); i++) {
      models[i].setBool();
    }
  }

  if (key == 'F' && action == GLFW_PRESS) {
    poly_mode = poly_mode ? 0 : 1;
    if (poly_mode) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
  }

  // Mouse controlls
  if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    startAngle = getCursorPos(window);
    std::cout << "Called: " << std::endl;
  }

  if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
    endAngle = getCursorPos(window);
    models[mesh_i].rotateAngle(startAngle, endAngle);
  }
}

