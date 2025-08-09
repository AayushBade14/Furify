#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./Include/Shader/Shader.hpp"
#include "./Include/Primitives/Primitives.hpp"
#include "./Include/Memory/Buffer/Buffer.hpp"
#include "./Include/Memory/VAO/VAO.hpp"

#define WIDTH 1920.0f
#define HEIGHT 1013.0f
#define TITLE "Furify"

// ================[CAMERA-PROPERTIES]============================
glm::vec3 cameraPos = {0.0f,0.0f,3.0f};
glm::vec3 cameraFront = {0.0f,0.0f,-1.0f};
glm::vec3 cameraUp = {0.0f,1.0f,0.0f};

float cameraSpeed = 5.0f;
float fov = 45.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = WIDTH/2.0f;
float lastY = HEIGHT/2.0f;

bool firstMouse = true;
float dpi = 0.1f;

float dt = 0.0f;
float lastFrame = 0.0f;
// ==============================================================

// ==================[CALLBACKS]=================================
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// ===============================================================

// ===================[METHODS]===================================
void ProcessInput(GLFWwindow* window){
  if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
    glfwSetWindowShouldClose(window,true);

  if(glfwGetKey(window,GLFW_KEY_T)==GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  if(glfwGetKey(window,GLFW_KEY_Y)==GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront * dt;
  if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront * dt;
  if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS)
    cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront,cameraUp)) * dt;
  if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)
    cameraPos  += cameraSpeed * glm::normalize(glm::cross(cameraFront,cameraUp)) * dt;
}
// ===============================================================

int main(void){
  
  // ==============[INITIALIZATION]=================
  if(glfwInit()<0){
    std::cerr<<"ERROR: GLFW_INIT!"<<std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIDTH,
                                        HEIGHT,
                                        TITLE,
                                        NULL,
                                        NULL);
  if(!window){
    glfwTerminate();
    std::cerr<<"ERROR: Creating Window!"<<std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cerr<<"ERROR: GLAD_INIT!"<<std::endl;
    return -1;
  }

  // ==============================================

  // ============[SHADER SETUP]===================
  Shader shader("./Assets/Shaders/vert.glsl","./Assets/Shaders/frag.glsl");
  // =============================================

  // ============[BUFFER SETUP]===================
  VAO vao;
  vao.Bind();

  Buffer vbo;
  vbo.CreateBuffer(GL_ARRAY_BUFFER);
  vbo.BindBuffer();
  vbo.AllocateAndFill(cubeVertices.size()*sizeof(float),cubeVertices.data(),GL_STATIC_DRAW);
  
  vao.SetAttribPointer(0,3,8,0);
  vao.SetAttribPointer(1,3,8,3);
  vao.SetAttribPointer(2,2,8,6);

  vao.Unbind();
  // =============================================


  // ============[SETTING CALLBACKS]==============
  glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
  glfwSetCursorPosCallback(window,mouse_callback);
  glfwSetScrollCallback(window,scroll_callback);
  // ==============================================
  
  // ===================[OGL BUFFERS]===============
  glEnable(GL_DEPTH_TEST);
  // ===============================================
  
  // =================[RENDER-LOOP]=================
  
  while(!glfwWindowShouldClose(window)){
    // ----Calculating dt and processing inputs----
    float currentFrame = (float)glfwGetTime();
    dt = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    ProcessInput(window);
    // -----------------------------------------
    
    // ----------- Clearing Buffers -----------
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ----------------------------------------
    
    // ------------- SETUP --------------------
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(fov),WIDTH/HEIGHT,0.1f,1000.0f);

    shader.Use();
    shader.SetValue("model",model);
    shader.SetValue("view",view);
    shader.SetValue("projection",projection);
    // ----------------------------------------

    // -------------- Rendering ---------------
    vao.Bind();
    glDrawArrays(GL_TRIANGLES,0,36);
    vao.Unbind();
    // ----------------------------------------
    
    // ------------- Swap buffers -------------
    glfwPollEvents();
    glfwSwapBuffers(window);
    // ----------------------------------------
  }

  // ===============================================
  
  // ================[CLEANUP]======================
  glfwTerminate();
  glfwDestroyWindow(window);
  // ===============================================
  
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0,0,width,height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
  float Xpos = (float)xpos;
  float Ypos = (float)ypos;

  if(firstMouse){
    lastX = Xpos;
    lastY = Ypos;
    firstMouse = false;
  }

  float xOffset = Xpos - lastX;
  float yOffset = lastY - Ypos;

  xOffset *= dpi;
  yOffset *= dpi;

  lastX = Xpos;
  lastY = Ypos;

  yaw += xOffset;
  pitch += yOffset;

  if(pitch > 89.0f)
    pitch = 89.0f;
  if(pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
  direction.y = glm::sin(glm::radians(pitch));
  direction.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));

  cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset){
  fov -= (float)yOffset;
  if(fov > 45.0f)
    fov = 45.0f;
  if(fov < 1.0f)
    fov = 1.0f;
}

