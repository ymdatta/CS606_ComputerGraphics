#include "mesh.h"

void Mesh::addModel(char s[]) {
  std::ifstream inFile(s);
  if (!inFile) {
    std::cout << "Unable to open .ply file" << std::endl;
    exit(1);
  }

  std::string s_temp;
  inFile >> s_temp;
  if (s_temp.compare("ply") != 0) {
    std::cout << "Given file isn't a .ply file" << std::endl;
    exit(1);
  }

  inFile >> s_temp >> s_temp >> s_temp;
  inFile >> s_temp >> s_temp >> nVertices;
  inFile >> s_temp >> s_temp >> s_temp;
  inFile >> s_temp >> s_temp >> s_temp;
  inFile >> s_temp >> s_temp >> s_temp;
  inFile >> s_temp >> s_temp >> nFaces;
  inFile >> s_temp >> s_temp >> s_temp >> s_temp >> s_temp;
  inFile >> s_temp;

  g_vertex_buffer_data = new float [3 * nVertices];
  g_splat_buffer_data = new float [21 * 3 * nFaces];
  g_color_buffer_data = new float [3 * nVertices];
  g_color_splat_buffer_data = new float [21 * 3 * nFaces];  


  if (s_temp.compare("end_header") != 0) {
    std::cout << "Wrong header" << std::endl;
    exit(1);
  }

  for(int i = 0; i < 3 * nVertices; i++) {
    float val_t;
    inFile >> val_t;
    g_vertex_buffer_data[i] = val_t;
    //    std::cout << g_vertex_buffer_data[i] << " ";
    if ( i % 3 == 0) {
      //      std::cout << std::endl;
    }
  }

  int ft = 0;
  inFile >> ft;

  g_face_buffer_data = new unsigned int [3 * nFaces];
  g_splat_index_data = new unsigned int [20 * 3 * nFaces];
  // Every face has a normal, a normal has 3 co-ords.
  g_normal_buffer_data = new float [3 * nFaces];

  face_type = 3;  
  if(ft != 3) {
    std::cout << "Only FaceType 3 is suppported" << std::endl;
    exit(1);
  }

  for(int i = 0; i < 3; i++) {
    unsigned int val_t;
    inFile >> val_t;
    g_face_buffer_data[i] = val_t;
  }

  int count_t = 3;
  for(int i = 1; i < nFaces; i++) {
    unsigned int t_1, t_2, t_3, t_4;
    inFile >> t_1 >> t_2 >> t_3 >> t_4;
    g_face_buffer_data[count_t] = t_2;
    count_t += 1;
    g_face_buffer_data[count_t] = t_3;
    count_t += 1;
    g_face_buffer_data[count_t] = t_4;
    count_t += 1;    
  }

}

void Mesh::computeSplat() {
  int k0 = 0, k1 = 0, k2 = 0, l = 0;
  
  for(int i = 0; i < nFaces; i++) {
    
    int face_v1 = g_face_buffer_data[3 * i];
    int face_v2 = g_face_buffer_data[(3 * i) + 1];
    int face_v3 = g_face_buffer_data[(3 * i) + 2];
    
    glm::vec3 v1(g_vertex_buffer_data[3 * face_v1],
		 g_vertex_buffer_data[(3 * face_v1) + 1],
		 g_vertex_buffer_data[(3 * face_v1) + 2]);
      
    glm::vec3 v2(g_vertex_buffer_data[3 * face_v2],
		 g_vertex_buffer_data[(3 * face_v2) + 1],
		 g_vertex_buffer_data[(3 * face_v2) + 2]);

    glm::vec3 v3(g_vertex_buffer_data[3 * face_v3],
		 g_vertex_buffer_data[(3 * face_v3) + 1],
		 g_vertex_buffer_data[(3 * face_v3) + 2]);    
    
    glm::vec3 inCentre = v1 * glm::length(v2 - v3) + v2 * glm::length(v1 - v3) + v3 * glm::length(v1 - v2);
    float s_2 = (glm::length(v2 - v3) + glm::length(v1 - v3) + glm::length(v1 - v2)) ;
    inCentre /= (s_2);

    glm::vec3 inNormal = glm::cross((v3 - v2), (v1 - v2));
    float inRadius = glm::length(inNormal) / s_2;

    //    glm::vec3 axisVec = glm::cross(inNormal, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 axisVec = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), inNormal);
    axisVec = glm::normalize(axisVec);
    
    float angleD = glm::acos(glm::dot(inNormal, glm::vec3(0.0f, 0.0f, 1.0f)) / glm::length(inNormal));

    glm::mat4 aMat = glm::scale(glm::mat4(1.0f), glm::vec3(inRadius, inRadius, inRadius));    
    aMat = glm::translate(inCentre) * glm::rotate(-angleD, axisVec) * aMat;

    inNormal = abs(glm::normalize(inNormal));

    
    g_splat_buffer_data[3 * k0] = inCentre.x;
    g_splat_buffer_data[3 * k0  + 1] = inCentre.y;
    g_splat_buffer_data[3 * k0  + 2] = inCentre.z;

    g_color_splat_buffer_data[3 * k0] = inNormal.x;
    g_color_splat_buffer_data[3 * k0  + 1] = inNormal.y;
    g_color_splat_buffer_data[3 * k0  + 2] = inNormal.z;    

    //TO : INcrement k0
    k1 = k0 + 1;
    
    float theta = 0;
    //    glm::vec4 a_temp = glm::vec4(glm::cos(theta), glm::sin(theta), 0, 1) * aMat;
    glm::vec4 a_temp = aMat * glm::vec4(glm::cos(theta), glm::sin(theta), 0, 1);


    g_splat_buffer_data[3 * k1] = a_temp.x;
    g_splat_buffer_data[3 * k1  + 1] = a_temp.y;
    g_splat_buffer_data[3 * k1  + 2] = a_temp.z;

    g_color_splat_buffer_data[3 * k1] = inNormal.x;
    g_color_splat_buffer_data[3 * k1  + 1] = inNormal.y;
    g_color_splat_buffer_data[3 * k1  + 2] = inNormal.z;        
    
    k2 = k1 + 1;
    
    for(int j = 1; j < 20; j++) {
      theta = glm::radians(360.0f) * 0.05 * (float)j;
      a_temp = aMat * glm::vec4(glm::cos(theta), glm::sin(theta), 0, 1);

      g_splat_buffer_data[3 * k2] = a_temp.x;
      g_splat_buffer_data[3 * k2  + 1] = a_temp.y;
      g_splat_buffer_data[3 * k2  + 2] = a_temp.z;

      g_color_splat_buffer_data[3 * k2] = inNormal.x;
      g_color_splat_buffer_data[3 * k2  + 1] = inNormal.y;
      g_color_splat_buffer_data[3 * k2  + 2] = inNormal.z;        

      g_splat_index_data[l++] = k0;
      g_splat_index_data[l++] = k1;
      g_splat_index_data[l++] = k2;

      k1 = k2;
      k2 += 1;
    }

    g_splat_index_data[l++] = k0;
    g_splat_index_data[l++] = (k0+20);
    g_splat_index_data[l++] = (k0+1);    

    // Should increase k0;
    k0 += 21;
  }
}

int Mesh::getVerticesNum() {
  return nVertices;
}

int Mesh::getFacesNum() {
  return nFaces;
}

int Mesh::getFaceType() {
  return face_type;
}

void Mesh::initializeCube() {
  min_x = std::numeric_limits<float>::max();
  min_y = min_x;
  min_z = min_x;

  max_x = std::numeric_limits<float>::min();
  max_y = max_x;
  max_z = max_x;

  for(int i = 0; i < nVertices * 3; i++) {
    float x = g_vertex_buffer_data[i];
    i += 1;
    float y = g_vertex_buffer_data[i];
    i += 1;
    float z = g_vertex_buffer_data[i];
    i += 1;

    if (x < min_x)
      min_x = x;
    if (x > max_x)
      max_x = x;
    if (y < min_y)
      min_y = y;
    if (y > max_y)
      max_y = y;
    if (z < min_z)
      min_z = z;
    if (z > max_z)
      max_z = z;
  }	   
}

void Mesh::normalise() {
  float max_v = 0;
  //  float max_x = -1, max_y = -1, max_z = -1;
  for(int i = 0; i < nVertices * 3; i++) {
    float t_val = abs(g_vertex_buffer_data[i]);
    if (t_val > max_v) {
      max_v = t_val;
    }
  }

  // Dividing by 4, because we want 4 models to fit the screen.
  for(int i = 0; i < nVertices * 3; i++) {
    g_vertex_buffer_data[i] = g_vertex_buffer_data[i] /  (max_v * 4);
  }

  computeSplat();
}

void Mesh::rotations() {
  rot_q = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
  rotationMatrix = glm::toMat4(rot_q);
  createMVP();
}

void Mesh::rotateLeft() {
  angle += 25.0f;
  if (angle > 360) {
    angle -= 360.f;
  }
  rotations();
}

void Mesh::rotateRight() {
  angle -= 25.0f;
  if (angle < -360) {
    angle += 360.f;
  }  
  rotations();
}


void Mesh::rotateAngle2(GLfloat angle, glm::vec3 axis){
  rot_q = glm::angleAxis(angle, axis);
  rotationMatrix = glm::toMat4(rot_q);
  createMVP();
}

void Mesh::rotateAngle(glm::vec3 startPoint, glm::vec3 endPoint) {
  glm::vec3 start, end, origin;
  
  start = glm::vec3(glm::inverse(MVP) * glm::vec4(startPoint, 0.0f));
  end = glm::vec3(glm::inverse(MVP) * glm::vec4(endPoint, 0.0f));

  glm::vec4 origin2 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);  
  origin = glm::vec3(origin2);

  glm::vec3 start_t = subVectors(startPoint, origin);
  glm::vec3 end_t = subVectors(endPoint, origin);

  glm::vec3 normal = glm::cross(start_t, end_t);
  
  float start_len = glm::length(start_t);
  float end_len = glm::length(end_t);

  float dot_prod = ((start_t.x * end_t.x) + (start_t.y * end_t.y) +
		    (start_t.z * end_t.z));
    
  GLfloat angle = glm::acos(dot_prod / (start_len*end_len));
  rotateAngle2(angle, normal);
}

void Mesh::createMVP() {
  MVP = translationMatrix * rotationMatrix * scaledMatrix;
}

void Mesh::scaleUp() {
  scale += 0.05;
  scaledMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
  createMVP();  
}

void Mesh::scaleDown() {
  scale -= 0.05;
  scaledMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
  createMVP();  
}

void Mesh::translateLeft() {
  translationMatrix = glm::translate(translationMatrix, glm::vec3(-0.05f, 0, 0));
  createMVP();
}

void Mesh::translateRight() {
  translationMatrix = glm::translate(translationMatrix, glm::vec3(0.05f, 0, 0));
  createMVP();  
}

void Mesh::translateUp() {
  translationMatrix = glm::translate(translationMatrix, glm::vec3(0, 0.05f, 0));
  createMVP();  
}

void Mesh::translateDown() {
  translationMatrix = glm::translate(translationMatrix, glm::vec3(0, -0.05f, 0));
  createMVP();  
}
  

void Mesh::createIndexBuffer() {
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	       nFaces * face_type * sizeof(unsigned int),
	       g_face_buffer_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  

  // For splat.  
  glGenBuffers(1, &VSIO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VSIO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	       nFaces * 3 * 20 * sizeof(unsigned int),
	       g_splat_index_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    
}

void Mesh::createVertexBuffer() {
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float),
	       g_vertex_buffer_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);      

  // For splat.
  glGenBuffers(1, &VSBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VSBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	       nFaces * 3 * 21 * sizeof(float),
	       g_splat_buffer_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);      
}

void Mesh::createColorBuffer() {
  glGenBuffers(1, &VCO);
  glBindBuffer(GL_ARRAY_BUFFER, VCO);
  glBufferData(GL_ARRAY_BUFFER, nVertices * 3 * sizeof(float), g_color_buffer_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // For splat.
  glGenBuffers(1, &VSCO);
  glBindBuffer(GL_ARRAY_BUFFER, VSCO);
  glBufferData(GL_ARRAY_BUFFER, nFaces * 3 * 21 *  sizeof(float), g_color_splat_buffer_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);  
}



void Mesh::bindOnce() {
  //  glUseProgram(programID);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  
  glEnableVertexAttribArray(0);  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //  For colors.
  glEnableVertexAttribArray(1);  
  glBindBuffer(GL_ARRAY_BUFFER, VCO);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


  // For splats.
  glGenVertexArrays(1, &VSO);
  glBindVertexArray(VSO);
  
  glEnableVertexAttribArray(0);  
  glBindBuffer(GL_ARRAY_BUFFER, VSBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VSIO);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //  For colors.
  glEnableVertexAttribArray(1);  
  glBindBuffer(GL_ARRAY_BUFFER, VSCO);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
}
  

void Mesh::renderMesh(GLuint programID) {
  glUseProgram(programID);
  MatrixId = glGetUniformLocation(programID, "MVP");
  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, &MVP[0][0]);

  if (splatBool!=0){
     glBindVertexArray(VAO);
     glDrawElements(GL_TRIANGLES, nFaces * 3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);     
  }
  else{
    glBindVertexArray(VSO);
    glDrawElements(GL_TRIANGLES, nFaces * 21 * 3, GL_UNSIGNED_INT, 0);    

    glBindVertexArray(0);
  }
}

glm::vec3 Mesh::subVectors(glm::vec3 b, glm::vec3 a) {
  return glm::vec3(b.x - a.x, b.y - a.y, b.z - a.z);
}

glm::vec3 Mesh::triangleNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
  return glm::cross(subVectors(b, a), subVectors(c, a));
}

void Mesh::createNormals() {
  // Count variable for normal buffer data.
  int count = 0;
  for(int i = 0; i < 3 * nFaces; i++) {
    unsigned int v1, v2 ,v3;
    //    int temp_v = 3 * i;
    // Get face vertices.
    v1 = g_face_buffer_data[i++];
    v2 = g_face_buffer_data[i++];    
    v3 = g_face_buffer_data[i];

    // Get vertex co-ords.
    glm::vec3 a, b, c, normal;
    a = glm::vec3(g_vertex_buffer_data[3 * v1],
		  g_vertex_buffer_data[(3 * v1) + 1],
		  g_vertex_buffer_data[(3 * v1) + 2]);
    b = glm::vec3(g_vertex_buffer_data[3 * v2],
		  g_vertex_buffer_data[(3 * v2) + 1],
		  g_vertex_buffer_data[(3 * v2) + 2]);
    c = glm::vec3(g_vertex_buffer_data[3 * v3],
		  g_vertex_buffer_data[(3 * v3) + 1],
		  g_vertex_buffer_data[(3 * v3) + 2]);
    // Calculate Normal of triangle.
    // Update count.
    normal = triangleNormal(a, b, c);
    g_normal_buffer_data[count++] = normal.x;
    g_normal_buffer_data[count++] = normal.y;
    g_normal_buffer_data[count++] = normal.z;
  }
}

void Mesh::createNormalColors() {

  // create Normals to Faces.
  createNormals();

  // First, go through face list, for each vertex.
  // If a face has that vertex, add it to normal.

  for(int i = 0; i < nVertices; i++) {
    glm::vec3 temp_n = glm::vec3(0, 0, 0);
    float n_count = 0;
    for(int j = 0; j < 3 * nFaces; j++) {
      int v1, v2 ,v3, v4;
      // Get face vertices.
      v4 = j;
      v1 = g_face_buffer_data[j++];
      v2 = g_face_buffer_data[j++];
      v3 = g_face_buffer_data[j];

      if ((v1 == i) || (v2 == i) || (v3 == i)) {
	n_count += 1;
	// Because Normals and face vertices sync.
	temp_n.x += g_normal_buffer_data[v4++];
	temp_n.y += g_normal_buffer_data[v4++];
	temp_n.z += g_normal_buffer_data[v4];
      }
    }
    int k = 3 * i;
    temp_n = temp_n / n_count;
    temp_n = abs(glm::normalize(temp_n));
    g_color_buffer_data[k] = temp_n.x;
    g_color_buffer_data[k+1] = temp_n.y;
    g_color_buffer_data[k+2] = temp_n.z;
  }
}
