#ifndef __MESH__
#define __MESH__

#define QUAD 4
#define TRI 3

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <bits/stdc++.h>

//#include <stdio.h>

#include <cstdlib>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../source/stb_image.h"

class Mesh {

  int splatBool;
  int nVertices;
  int nFaces;
  float *g_vertex_buffer_data;
  unsigned int *g_face_buffer_data;
  float *g_normal_buffer_data;
  float *g_color_buffer_data;
  float *g_text_cord_spher;
  float *g_text_cord_cyl;
  float *g_text_cord_arb;
  int face_type;
  GLuint IBO;
  GLuint VBO;
  GLuint VAO, VCO, VNO, VSTO, VCTO, VATO;
  glm::mat4 MVP = glm::mat4(1.0f);
  glm::mat4 rotationMatrix = glm::mat4(1.0f);
  glm::mat4 scaledMatrix = glm::mat4(1.0f);
  glm::mat4 translationMatrix = glm::mat4(1.0f);
  GLuint MatrixId;
  float scale = 0.55;
  float angle = 0.0f;
  glm::quat rot_q = glm::mat4(1.0f);

  float max_x, max_y, max_z = 0;
  float min_x, min_y, min_z = 0;
  
public:

  glm::vec3 subVectors(glm::vec3 a, glm::vec3 b);
  glm::vec3 triangleNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
  void createNormals();
  void createNormalColors();
  //  Mesh();

  //Given a ply file, add it to mesh.
  void addModel(char s[]);
  GLuint getVAO() { return VAO; }

  // Draw function
  void render();
  
  int getVerticesNum();
  int getFacesNum();
  int getFaceType();

  void createVertexBuffer();
  void createIndexBuffer();
  void createColorBuffer();
  void createNormalBuffer();

  void createSphTexBuffer();
  void createCycTexBuffer();
  void createArbTexBuffer();

  void renderMesh(GLuint programID, GLuint texID);

  void bindOnce();
  void normalise();

  void translateLeft();
  void translateRight();
  void translateUp();
  void translateDown();

  void rotateLeft();
  void rotateRight();
  void rotateAngle2(GLfloat angle, glm::vec3 axis);
  void rotateAngle(glm::vec3 startAngle, glm::vec3 endAngle);
  
  void scaleUp();
  void scaleDown();

  void initializeCube();

  void compSphTexCoords();
  void compCylTexCoords();
  void compArbTexCoords();

  void compFloorCoords();


  void createMVP();
  void rotations();
  void print() {
    for(int i = 0; i < 3 * nVertices; i++) {
      std::cout << g_vertex_buffer_data[i] << " ";
    }
    for(int i = 0; i < 3 * nFaces; i++) {
      std::cout << g_face_buffer_data[i] << " ";
    }
  }
      
  void printVertices() {
    for(int i = 0; i < 4; i++) {
      std::cout << "Here" << std::endl;
      std::cout << g_vertex_buffer_data[i] << std::endl;
    }
  }
  void printFaces() {
    std::cout << "numFaces: " << nFaces << std::endl;
    for(int i = 0; i < 5; i++) {
      std::cout << g_face_buffer_data[i] << " ";
    }
    std::cout << std::endl;
  }
};

#endif
