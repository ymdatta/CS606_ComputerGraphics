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

#include <cstdlib>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

class Mesh {

  int splatBool;
  int nVertices;
  int nFaces;
  float *g_vertex_buffer_data;
  float *g_splat_buffer_data;
  unsigned int *g_splat_index_data;
  unsigned int *g_face_buffer_data;
  float *g_normal_buffer_data;
  float *g_color_buffer_data;
  float *g_color_splat_buffer_data;
  int face_type;
  GLuint IBO;
  GLuint VBO;
  GLuint VAO, VCO, VNO, VSO, VSBO, VSIO, VSCO;
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
  
  int getVerticesNum();
  int getFacesNum();
  int getFaceType();

  void createVertexBuffer();
  void createIndexBuffer();
  void createColorBuffer();
  void createNormalBuffer();
  
  // Draw function
  void renderMesh(GLuint programID);

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
  void computeSplat();

  void createMVP();
  void rotations();
  void setBool() {
    if(splatBool == 0) {
      splatBool = 1;
    }
    else {
      splatBool = 0;
    }
  }
};

#endif
