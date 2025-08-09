#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./Include/Shader/Shader.hpp"
#include "./Include/Primitives/Primitives.hpp"
#include "./Include/Memory/Buffer/Buffer.hpp"
#include "./Include/Memory/VAO/VAO.hpp"
#include "./Include/Texture/Texture.hpp"

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
  Shader shader("./Assets/Shaders/BasicShaders/vert.glsl","./Assets/Shaders/BasicShaders/frag.glsl");
  Shader skyShader("./Assets/Shaders/CubemapShaders/vert.glsl","./Assets/Shaders/CubemapShaders/frag.glsl");
  // =============================================
  
  // ============[TEXTURE SETUP]==================
  Texture maskTexture;
  maskTexture.CreateTexture(GL_TEXTURE_2D);
  maskTexture.BindTexture();

  maskTexture.SetWrapS(GL_REPEAT);
  maskTexture.SetWrapT(GL_REPEAT);
  maskTexture.SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
  maskTexture.SetMagFilter(GL_LINEAR);

  maskTexture.LoadTexture("./Assets/Textures/fur_mask.jpeg",true);
  
  maskTexture.UnbindTexture();

  maskTexture.SetSamplerValue(shader,"mask_tex",0);
  
  Texture furTexture;
  furTexture.CreateTexture(GL_TEXTURE_2D);
  furTexture.BindTexture();

  furTexture.SetWrapS(GL_REPEAT);
  furTexture.SetWrapT(GL_REPEAT);
  furTexture.SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
  furTexture.SetMagFilter(GL_LINEAR);

  furTexture.LoadTexture("./Assets/Textures/fur.jpeg",true);
  
  furTexture.UnbindTexture();

  furTexture.SetSamplerValue(shader,"fur_tex",1);

  // ----
  
  std::vector<std::string> paths = {
    "./Assets/Cubemaps/Skybox/right.jpg",
    "./Assets/Cubemaps/Skybox/left.jpg",
    "./Assets/Cubemaps/Skybox/top.jpg",
    "./Assets/Cubemaps/Skybox/bottom.jpg",
    "./Assets/Cubemaps/Skybox/front.jpg",
    "./Assets/Cubemaps/Skybox/back.jpg"
  };
  
  Texture skybox;
  skybox.CreateTexture(GL_TEXTURE_CUBE_MAP);
  skybox.BindTexture();

  skybox.SetWrapS(GL_CLAMP_TO_EDGE);
  skybox.SetWrapT(GL_CLAMP_TO_EDGE);
  skybox.SetWrapR(GL_CLAMP_TO_EDGE);
  skybox.SetMinFilter(GL_LINEAR);
  skybox.SetMagFilter(GL_LINEAR);

  skybox.LoadCubemapTexture(paths);

  skybox.UnbindTexture();

  skybox.SetSamplerValue(skyShader,"sky",0);
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

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    
    static int totalLayers = 20;
    static float furLength = 0.2f;

    // ----------------------------------------

    // -------------- Rendering ---------------
    maskTexture.AssignTextureUnit(0);
    maskTexture.BindTexture();
    
    furTexture.AssignTextureUnit(1);
    furTexture.BindTexture();

    shader.Use();
    shader.SetValue("model",model);
    shader.SetValue("view",view);
    shader.SetValue("projection",projection);
    shader.SetValue("furLength",furLength);
    shader.SetValue("totalLayers",totalLayers);

    for(int i = 0; i < totalLayers; i++){  
      shader.SetValue("layer",i);

      vao.Bind();
      glDrawArrays(GL_TRIANGLES,0,36);
      vao.Unbind();
    }
    maskTexture.UnbindTexture();
    furTexture.UnbindTexture();

    glDepthFunc(GL_LEQUAL);

    skybox.AssignTextureUnit(0);
    skybox.BindTexture();
    
    view = glm::mat4(glm::mat3(view));

    skyShader.Use();
    skyShader.SetValue("model",model);
    skyShader.SetValue("view",view);
    skyShader.SetValue("projection",projection);

    vao.Bind();
    glDrawArrays(GL_TRIANGLES,0,36);
    vao.Unbind();

    skybox.UnbindTexture();
    
    glDepthFunc(GL_LESS);
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

