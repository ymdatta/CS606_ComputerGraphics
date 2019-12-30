#include <bits/stdc++.h>

#include "../models/mesh.h"
#include "../utilities/controller.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define underline_on "\033[4m"
#define underline_off "\033[0m"
#include "../source/stb_image.h"
//#include <glm/gtx/transform.hpp>

//#include <common/shader.h>

using namespace std;

#define cout(a) cout << a << endl

Controller c;

const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

void handlekeys(GLFWwindow *windowt, int keyt, int codet, int actiont, int modet) {
  c.handleKeys(windowt, keyt, codet, actiont, modet);
}

void handlekeys2(GLFWwindow *windowt, int button, int action, int mods) {
  c.handleKeys(windowt, button, 0, action, mods);
}
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if(VertexShaderStream.is_open()){
    std::stringstream sstr;
    sstr << VertexShaderStream.rdbuf();
    VertexShaderCode = sstr.str();
    VertexShaderStream.close();
  }else{
    printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
    getchar();
    return 0;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if(FragmentShaderStream.is_open()){
    std::stringstream sstr;
    sstr << FragmentShaderStream.rdbuf();
    FragmentShaderCode = sstr.str();
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;


  // Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }



  // Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }



  // Link the program
  printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 0 ){
    std::vector<char> ProgramErrorMessage(InfoLogLength+1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }

	
  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);
	
  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

int main(void) {

  const GLint WIDTH = 800, HEIGHT = 650;

  // Initializing GLFW
  if (!glfwInit()) {
    cout("GLFW initialization failed.");
    glfwTerminate();
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

  GLFWwindow *mainWindow =
    glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
  if (!mainWindow) {
    cout("GLFW window creation failed.");
    glfwTerminate();
    return 1;
  }

  // Get buffer size information
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Set context for GLEW to use
  glfwMakeContextCurrent(mainWindow);

  // Allow modern extension features
  glewExperimental = GL_TRUE;

  

  if (glewInit() != GLEW_OK) {
    cout("GLEW initialization failed.");
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 1;
  }

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders( "shaders/shader.vs", "shaders/shader.fs" );  
  c.setProgramId(programID);

  glm::mat4 Model = glm::mat4(1.0f);
  

  // Our ModelViewProjection : multiplication of our 3 matrices
  //glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
  glm::mat4 MVP = Model;
	
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glViewport(0, 0, bufferWidth, bufferHeight);

  c.addModel("plyfiles/sphere.ply");
  c.addModel("plyfiles/sphere.ply");
  c.addModel("plyfiles/car.ply"); 
  c.addModel("plyfiles/beethoven.ply");
  c.createAndBind();
  c.loadTexs();

  std::cout << std::endl;
  std::cout << underline_on << "Options: " << underline_off << std::endl;


  std::cout << "Arrow up and down - To increase and decrease speed" << std::endl;
  std::cout << "Toggle selected model to spin around vertical axis" << std::endl;
  std::cout << "0, 1, 2, 3 - Select the model (if we add a model A first, then pressing 0 would select that model) " << std::endl;  


  
   glEnable(GL_DEPTH_TEST);
  
    while(!glfwWindowShouldClose(mainWindow)) {    
    
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

    c.renderModels();
    glfwSetKeyCallback(mainWindow, handlekeys);
    glfwSetMouseButtonCallback(mainWindow, handlekeys2);
		
    glfwSwapBuffers(mainWindow);
    glfwPollEvents();
    //    glfwWaitEvents();		
  }
  
  return 0;
}
