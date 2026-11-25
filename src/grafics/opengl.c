#define UNUSED __attribute__((unused))

// OpenGL related includes.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Imported from the std library.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Imported for math.
#include <cglm/cglm.h>

// Custom OpenGL relevant header.
#include "carbon13.h"

// For loading images/textures.
#include "stb_image.h"

// FUNCTIONS
// GLFW initialization func.
static void framebuffer_size_callback(
  UNUSED GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// GLFW window creation func.
static GLFWwindow* init(void)
{
  if (!glfwInit()) {
    printf("GLFW initialization failed.\n");
    return NULL;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // OS specific.
  #if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  GLFWwindow* window = glfwCreateWindow(1280, 720, "Learn OpenGL", NULL, NULL);
  if (!window) {
    printf("GLFW window creation failed.\n");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD.\n");
    return NULL;
  }

  const char *version = (const char *)glGetString(GL_VERSION);
  printf("OpenGL version: %s\n", version);

  return window;
}

// Process input from keys
static void process_input(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

// VARIABLES
float vertices[] = {
   0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
   0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
};

unsigned int indeces[] = {
  0, 1, 3, //first triangle
  1, 2, 3 //second triangle
};

float texCoords[] = {
  0.0f, 0.0f,
  1.0f, 0.0f,
  0.5f, 1.0f
};

int run_window()
{

  GLFWwindow* window = init();
  if (!window) {
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // SHADERS.
  const char* vertexShaderPath = "src/shader.vert";
  const char* fragmentShaderPath = "src/shader.frag";
  unsigned int shaderProgram;
  make_shader_program(vertexShaderPath, fragmentShaderPath, &shaderProgram);

  // OBJECTS.
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  unsigned int EBO;
  glGenBuffers(1, &EBO);

  // VERTEX BUFFER & ARRAYS
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // ELEMENT BUFFER
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);


  // Position attribute.
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // Define location of data.
  glVertexAttribPointer(
    2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Textures.
  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // Set options.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
  // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glTexParameteri(
  //   GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  // glTexParameteri(
  //   GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Flip loaded images.
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char* data = stbi_load(
    "src/container.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB, 
      width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    printf("Failed to load texture.");
  }
  stbi_image_free(data);

  // Texture2.
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  data = stbi_load(
    "src/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, 
      width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    printf("Failed to load texture.");
  }
  stbi_image_free(data);

  // Use Shader Program for tetures.
  glUseProgram(shaderProgram);
  glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
  glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

  // RENDER LOOP.
  while (!glfwWindowShouldClose(window)) {
    // Input.
    process_input(window);

    // Rendering commands.
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Update uniform color.
    //float timeValue = glfwGetTime();
    //float greenValue = sin(timeValue) / 2.0f + 0.5f;
    //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

    // Draw that bad boy (promotove in current shader)
    //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // Texture stuff.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Draw from array.
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    //
    // Draw from element.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);

    // Check and call events and swap the buffers.
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}

// Finish "Transformations/In practice" in LearnOpenGL"
