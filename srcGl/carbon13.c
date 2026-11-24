// The header this is for.
#include "carbon13.h"

// standard includes.
#include <stdio.h>
#include <stdlib.h>

// OpenGL includes.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

// Check compiled shader for errors.
static int shader_check(unsigned int shader, unsigned int type)
{
  // type can be GL_COMPILE_STATUS or GL_LINK_STATUS
  int status;
  char infoLog[512];
  glGetShaderiv(shader, type, &status);
  if (!status) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("Compilation Failed: %s\n", infoLog);
  }
  return status;
}

// Reads shader files to import.
static const char* read_shader_source(const char* filepath)
{
  FILE* file = fopen(filepath, "rb");
  if (!file) {
    printf("Failed to open shader file.\n");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = (char*)malloc(length + 1);
  if (!buffer) {
    printf("failed to allocate memory for shaderReader.\n");
    fclose(file);
    return NULL;

  }

  fread(buffer, 1, length, file);
  buffer[length] = '\0'; // Terminate the string.

  fclose(file);
  return buffer;
}

int make_shader_program(
  const char* vertPath, const char* fragPath, unsigned int* prog)
{
  // vertex shader.
  const char* vertexShaderSource = read_shader_source(vertPath);
  const char* fragmentShaderSource = read_shader_source(fragPath);

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // fragment shader.
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Check for compile time errors.
  if (!shader_check(
    fragmentShader, GL_COMPILE_STATUS) 
    || !shader_check(vertexShader, GL_COMPILE_STATUS)) {
    return 1;
  }

  // Shader Program.
  *prog = glCreateProgram();
  // Attach and link shader program.
  glAttachShader(*prog, vertexShader);
  glAttachShader(*prog, fragmentShader);
  glLinkProgram(*prog);
  // Linking error check.
  if(!shader_check(*prog, GL_LINK_STATUS)) {
    return 1;
  }

  // Delete shader objects.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return 0;
}
