#include "controller.h"

void Controller::addModel(char s[]) {
  Mesh m_new;
  m_new.addModel(s);
  models.push_back(m_new);
}

void Controller::loadTexs() {
  textStringVector.push_back("textures/floor.jpeg");
  //  textStringVector.push_back("textures/apple.jpg");
  textStringVector.push_back("textures/mali.jpg");  
  textStringVector.push_back("textures/world.bmp");  
  textStringVector.push_back("textures/check.bmp");
  createTexs();
}


// Get image data and create texture maps.
void Controller::createTexs() {
  unsigned int size = textStringVector.size();
  texBuff = (unsigned *)malloc(sizeof(unsigned int) * size);
  int width, height, nrChannels;
  glGenTextures(size, texBuff);
  for(unsigned int i = 0; i < size; i++) {
    glBindTexture(GL_TEXTURE_2D, texBuff[i]);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data = stbi_load(textStringVector[i].c_str(), &width, &height, &nrChannels, 0);

    if (data)
      {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
      }
    else
      {
	std::cout << "Failed to load texture" << std::endl;
      }
    glBindTexture(GL_TEXTURE_2D, 0);    
    stbi_image_free(data);
  }
}


void Controller::createAndBind() {
  for(unsigned int i = 0; i < models.size(); i++) {
    Mesh& m = models[i];
    m.normalise();
    m.initializeCube();    

      m.compSphTexCoords();
      m.compCylTexCoords();
      m.compArbTexCoords();
      
    // if ((i + 1) == (models.size())) {
    //   m.compFloorCoords();
    //   for(int i1 = 0; i1 < 100; i1++) {
    // 	m.scaleUp();	
    //   }
    //   for(int i1 = 0; i1 < 5; i1++) {
    // 	m.translateDown();
    //   }
    // }

    // Create texture coordinates.
    m.createSphTexBuffer();
    m.createCycTexBuffer();
    m.createArbTexBuffer();

    // Create buffers.
    m.createVertexBuffer();
    m.createIndexBuffer();
    m.createNormalColors();
    
    // First create Normal colors, then Buffer.
    m.createColorBuffer();
    m.bindOnce();
  }

  std::vector<int> a0, a1, a2, a3, a4;
  // child.push_back(a0);
  // a1.push_back(0);
  // child.push_back(a1);  
  // a2.push_back(0);
  // a2.push_back(1);
  // child.push_back(a2);
  // a3.push_back(0);
  // a3.push_back(1);
  // a3.push_back(2);  
  // child.push_back(a3);  
}

void Controller::renderModels() {

  GLuint fId = glGetUniformLocation(programID, "fId");
  glUniform1i(fId, 0);

  GLuint cId = glGetUniformLocation(programID, "cId");
  glUniform1i(cId, cord_change);

  // Logic for changing co-ordinates.
  // if (cord_change == 0) {
  //   std::cout << "Spherical Coordinates" << std::endl;
  // } else if (cord_change == 1) {
  //   std::cout << "Cylindrical Coordinates" << std::endl;
  // }else {
  //   std::cout << "Arbitrary Coordinates" << std::endl;
  // }
  
  GLuint lPosId = glGetUniformLocation(programID, "lPos");
  glUniform3f(lPosId, lightPos.x, lightPos.y, lightPos.z);

  glm::mat4 viewMat = (glm::lookAt(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));


  viewMat = glm::perspective(glm::radians(60.0f), (800.0f/650.0f), 1.0f, 100.0f) * viewMat;
  
  GLuint viewId = glGetUniformLocation(programID, "vMat");
  glUniformMatrix4fv(viewId, 1, GL_FALSE, &viewMat[0][0]);
  
  for(unsigned int i = 0; i < models.size(); i++) {
    Mesh& m = models[i];
    unsigned int tex_i = (i + tex_change) % models.size();
    //    models[1].rotateCircle();
    rotateVertical();
    rotateCircle();
    m.renderMesh(programID, texBuff[tex_i]);
  }
  // Logic for drawing light source.
  glUniform1i(fId, 1);
  glPointSize(10);  
  glBindVertexArray(models[0].getVAO());
  glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);
  glPointSize(1);  
}

void Controller::rotateCircle() {
  
  float g_sin = glm::sin(glm::radians(c_angle));
  float g_cos = glm::cos(glm::radians(c_angle));

  float t_sin = glm::sin(glm::radians(c_angle * 2.0f));
  float g_r = (0.7 + (0.3 * t_sin));
  float t_x = g_r * g_sin ;
  float t_z = g_r * g_cos ;

  c_angle += 1.25 * c_speed;
  c_angle = ((int)c_angle % 360);

  glm::vec3 temp_t = glm::vec3(t_x, 0, t_z);
  models[1].rotateCircle(temp_t);


  glm::vec3 vDir = glm::normalize(temp_t - followVec);
  vDir /= 100.0f;
  followVec = vDir + followVec;

  models[2].rotateCircle(followVec);
  
  glm::vec3 temp_fv = followVec;
  temp_fv.y += g_r;
  models[3].rotateCircle(temp_fv);
}

glm::vec3 Controller::getCursorPos(GLFWwindow *window) {

  double xpos_t = 0, ypos_t = 0;
  //  xpos = 0, ypos = 0, zpos = 0;
  glfwGetCursorPos(window, &xpos_t, &ypos_t);

  xpos_t = (2 * xpos_t / 800) - 1;
  ypos_t = 1 - (2 * ypos_t / 650);

  return glm::vec3(xpos_t, ypos_t, 0);
}

void Controller::rotateVertical() {
  if (rotate_t) {
    models[mesh_i].rotateLeft();
  }
}
    
  

void Controller::handleKeys(GLFWwindow *window, int key, int code, int action, int mode) {
  //    models[1].rotateCircle();
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

  // if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
  //   models[mesh_i].translateLeft();
  //   for(unsigned int i = 0; i < child[mesh_i].size(); i++) {
  //     models[child[mesh_i][i]].translateLeft();
  //   }	     
  // }

  // if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
  //   models[mesh_i].translateRight();
  //   std::cout << "Number of children: " << child[mesh_i].size() << std::endl;
  //   for(unsigned int i = 0; i < child[mesh_i].size(); i++) {
  //     models[child[mesh_i][i]].translateRight();
  //   }    
  // }

  if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    //    models[mesh_i].translateUp();
    c_speed += 0.05f;
  }

  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    //    models[mesh_i].translateDown();
    c_speed -= 0.05f;
  }

  if (key == 'A' && action == GLFW_PRESS) {
    //    models[mesh_i].rotateLeft();
    rotate_t ? (rotate_t = 0) : (rotate_t = 1);
  }
  if (key == 'D' && action == GLFW_PRESS) {
    //    models[mesh_i].rotateRight();
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
if (key == 'T' && action == GLFW_PRESS) {
  tex_change += 1;
 }

 if(key == 'M' && action == GLFW_PRESS) {
   cord_change += 1;
   cord_change %= 3;
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

void handleMouse(GLFWwindow *window, int button, int action, int mode) {
}

