#define GL_SILENCE_DEPRECATION
// #define __gl_h_
// #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "stb_image.h"
#include "camera.h"

const int WINDOW_WIDTH  = 1024;
const int WINDOW_HEIGHT = 768;

float lastMouseX = WINDOW_WIDTH  / 2.0f;
float lastMouseY = WINDOW_HEIGHT / 2.0f;
bool firstMouse  = true;

// delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float angle = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void frame_buffer_size_callback(GLFWwindow *window, int w, int h);

void mouse_callback(GLFWwindow *window, double mouse_x, double mouse_y);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

int main (int argc, char **argv)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Magic Bricks", nullptr, nullptr);

  if (window == nullptr) {
    cout << "cannot create window: " << endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "cannot load GLAD" << endl;
    glfwTerminate();
    return -1;
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

  glfwSetCursorPosCallback(window, mouse_callback);

  glfwSetScrollCallback(window, scroll_callback);

  // depth test
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // face culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // hide cursor and capture it
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  float fbovertex[] =
    {
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,

     -1.0f, -1.0f, 0.0f, 0.0f,
      1.0f, -1.0f, 1.0f, 0.0f,
     -1.0f,  1.0f, 0.0f, 1.0f,
    };

  float vertex_data[] =
    { // position, texture coords
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f,

     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

      0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

     -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
      0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,

      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
     -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,

      0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
     -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

      0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f,

      0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
      0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
      0.5f, -0.5f,  0.5f, 0.0f, 1.0f,

      0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
      0.5f, -0.5f,  0.5f, 0.0f, 1.0f,

     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
     -0.5f,  0.5f, -0.5f, 1.0f, 0.0f,

     -0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
     -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    };

  glm::vec3 cubePositions[] =
    {
     glm::vec3(0.0, 0.0, 0.0f),
     glm::vec3(0.0f, -1.0f, 0.0f),
     glm::vec3(0.0f, -2.0f, 0.0f),
     glm::vec3(-1.0f, 1.0f, 0.0f),
     glm::vec3(-1.0f, 2.0f, 0.0f),
     glm::vec3(1.0f, 1.0f, 0.0f),
     glm::vec3(1.0f, 2.0f, 0.0f),
     glm::vec3(3.5f, 0.0f, 0.0f),
     glm::vec3(3.5f, -1.0f, 0.0f),
     glm::vec3(3.5f, -2.0f, 0.0f),
     glm::vec3(3.5f, 1.0f, 0.0f),
     glm::vec3(3.5f, 2.0f, 0.0f),
     glm::vec3(6.0f, 0.0f, 0.0f),
     glm::vec3(6.0f, -1.0f, 0.0f),
     glm::vec3(6.0f, -2.0f, 0.0f),
     glm::vec3(6.0f, 1.0f, 0.0f),
     glm::vec3(6.0f, 2.0f, 0.0f),
     glm::vec3(7.0f, 0.0f, 0.0f),
     glm::vec3(8.0f, 0.0f, 0.0f),
     glm::vec3(7.0f, 2.0f, 0.0f),
     glm::vec3(8.0f, 2.0f, 0.0f),
     glm::vec3(10.0f, 0.0f, 0.0f),
     glm::vec3(10.0f, 1.0f, 0.0f),
     glm::vec3(10.0f, 2.0f, 0.0f),
     glm::vec3(10.0f, -1.0f, 0.0f),
     glm::vec3(10.0f, -2.0f, 0.0f),
     glm::vec3(11.0f, -2.0f, 0.0f),
     glm::vec3(12.0f, -2.0f, 0.0f),
     glm::vec3(12.0f, -1.0f, 0.0f),
     glm::vec3(12.0f, 0.0f, 0.0f),
     glm::vec3(12.0f, 1.0f, 0.0f),
     glm::vec3(12.0f, 2.0f, 0.0f),
    };

  // framebuffer
  unsigned int fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // texture for frame buffer's color buffer
  unsigned int texColorBuffer;
  glGenTextures(1, &texColorBuffer);
  glBindTexture(GL_TEXTURE_2D, texColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  // attach to framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

  // render buffer object
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  // check it is complete
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    cout << "ERROR::FRAMEBUFFER::NOT_COMPLETE" << endl;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // framebuffer vertex array object
  unsigned int fbovao, fbovbo;
  glGenVertexArrays(1, &fbovao);
  glBindVertexArray(fbovao);
  glGenBuffers(1, &fbovbo);
  glBindBuffer(GL_ARRAY_BUFFER, fbovbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fbovertex), fbovertex, GL_STATIC_DRAW);

  // framebuffer shader
  Shader fboShader("framebuffer.vs", "framebuffer.fs");

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // first bind a vertex array object
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // unsigned int ebo;
  // glGenBuffers(1, &ebo);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

  Shader shader("shader.vs", "shader.fs");

  // vbo is currently bind'ed to GL_ARRAY_BUFFER.
  glVertexAttribPointer(0, // in the layout
                        3, // vec3
                        GL_FLOAT, // type
                        GL_FALSE, // already normilised
                        5 * sizeof(float), // the gap between attributes
                        (void *) 0 // offset
                        );

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, // in the layout
                        2, // vec2
                        GL_FLOAT, // type
                        GL_FALSE, // already normilised
                        5 * sizeof(float), // the gap between attributes
                        (void *) (3 * sizeof(float)) // offset
                        );

  glEnableVertexAttribArray(1);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // load an image
  stbi_set_flip_vertically_on_load(true);
  int width, height, nChannels;
  unsigned char *data = stbi_load("resource/container.jpg", &width, &height, &nChannels, 0);

  if (!data)
    std::cout << "Fail to load image." << std::endl;

  // generate a texture
  unsigned int texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // give the image to texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  // free image memory
  stbi_image_free(data);

  // second image
  data = stbi_load("resource/cutecat.png", &width, &height, &nChannels, 0);

  if (!data)
    std::cout << "Fail to load image." << std::endl;

  // generate a texture
  unsigned int texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // give the image to texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  // free image memory
  stbi_image_free(data);

  // set uniform locations for textures
  shader.use();
  shader.setInt("ourTexture1", 0);
  shader.setInt("ourTexture2", 1);

  fboShader.use();
  fboShader.setInt("screenTexture", 0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // offscreen drawing
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClearColor(0.1f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(vao);

    glm::mat4 view(1.0f);
    view = camera.GetViewMatrix();
    glm::mat4 projection(1.0f), model(1.0f);
    projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1, 1, 0));
    shader.setMat4Float("view", view);
    shader.setMat4Float("model", model);
    shader.setMat4Float("projection", projection);

    for (int i = 0; i < 32; ++i) {
      glm::mat4 model(1.0f);
      model = glm::translate(model, cubePositions[i] + glm::vec3(-5.5f, 0.0f, -12.0f));
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1, 1, 0));
      shader.setMat4Float("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 3*12);
    }

    // now draw to the default framebuffer obejct
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    fboShader.use();
    glBindVertexArray(fbovao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    float currentFrame = glfwGetTime();
    deltaTime          = currentFrame - lastFrame;
    lastFrame          = currentFrame;

    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  
  return 0;
}

void frame_buffer_size_callback(GLFWwindow *window, int w, int h)
{
  glViewport(0, 0, w, h);
};

void processInput(GLFWwindow *window)
{
  float angleSpeed = 40.0f;
  
  // for quitting
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // 1=>wireframe; 2=>fill; 3=>point
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

  // for moving around
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) 
    camera.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
    camera.ProcessKeyboard(BACKWARD, deltaTime);

  // for rotating
  if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    angle += angleSpeed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    angle -= angleSpeed * deltaTime;
}

void mouse_callback(GLFWwindow *window, double mouse_x, double mouse_y)
{
  if (firstMouse) {
    lastMouseX = mouse_x;
    lastMouseY = mouse_y;
    firstMouse = false;
  }

  float xoffset = mouse_x    - lastMouseX;
  float yoffset = lastMouseY - mouse_y;

  lastMouseX = mouse_x;
  lastMouseY = mouse_y;
  camera.ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}

// ARCHIVE


// glm::vec3 cubePositions[] =
//   {
//    glm::vec3(0.0f, 0.0f, 0.0f),
//    glm::vec3(0.0f, 1.0f, 0.0f),
//    glm::vec3(0.0f, 2.0f, 0.0f),
//    glm::vec3(1.0f, 3.0f, 0.0f),
//    glm::vec3(2.0f, 3.0f, 0.0f),
//    glm::vec3(3.0f, 2.0f, 0.0f),
//    glm::vec3(0.0f, -1.0f, 0.0f),
//    glm::vec3(0.0f, -2.0f, 0.0f),
//    glm::vec3(1.0f, -3.0f, 0.0f),
//    glm::vec3(2.0f, -3.0f, 0.0f),
//    glm::vec3(3.0f, -2.0f, 0.0f),
//    glm::vec3(5.0f, 0.0f, 0.0f),
//    glm::vec3(6.0f, 0.0f, 0.0f),
//    glm::vec3(7.0f, 0.0f, 0.0f),
//    glm::vec3(6.0f, 1.0f, 0.0f),
//    glm::vec3(6.0f, -1.0f, 0.0f),
//    glm::vec3(9.0f, 0.0f, 0.0f),
//    glm::vec3(10.0f, 0.0f, 0.0f),
//    glm::vec3(11.0f, 0.0f, 0.0f),
//    glm::vec3(10.0f, 1.0f, 0.0f),
//    glm::vec3(10.0f, -1.0f, 0.0f)
// };

// transformations
// glm::mat4 model(1.0f);
// , view(1.0f), projection(1.0f)
// model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 1.0f));

// view = camera.GetViewMatrix();

// projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

// shader.use();
// shader.setMat4Float("model", model);
// shader.setMat4Float("view", view);
// shader.setMat4Float("projection", projection);
