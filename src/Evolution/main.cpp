#pragma warning(disable : 4201)
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Graphics3d/GlInfo.h"
#include "Graphics3d/Input/InputManager.h"
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace g3d;

void errorCallback(int error, const char * description)
{
  cout << "GLFW error (" << error << "): " << description << endl;
}

void resizeCallback(GLFWwindow * /*pWindow*/, int width, int height)
{
  cout << "resizeCallback" << endl;
  if (width > 0 && height > 0)
  {
  }
}

void drawRect()
{
  glBegin(GL_TRIANGLES);       // Each set of 3 vertices form a triangle
  glColor3f(0.0f, 0.0f, 1.0f); // Blue
  glVertex2f(0.1f, -0.6f);
  glVertex2f(0.7f, -0.6f);
  glVertex2f(0.4f, -0.1f);

  glColor3f(1.0f, 0.0f, 0.0f); // Red
  glVertex2f(0.3f, -0.4f);
  glColor3f(0.0f, 1.0f, 0.0f); // Green
  glVertex2f(0.9f, -0.4f);
  glColor3f(0.0f, 0.0f, 1.0f); // Blue
  glVertex2f(0.6f, -0.9f);
  glEnd();
}

void render(int width, int height)
{
  // GLfloat aspect = (GLfloat)width / (GLfloat)height;
  glViewport(0, 0, width, height);
  // glEnable(GL_CULL_FACE);
  glDisable(GL_CULL_FACE);
  // glFrontFace(GL_CCW);
  // glCullFace(GL_BACK);
  //

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, 0, height, -10, 10);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);

  glPointSize(20.0f);
  glBegin(GL_POINTS);
  // glVertex3f(0.0f, 0.0f, 1.0f);
  // glVertex3f(-100.5f, -100.5f, 1.0f);

  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex3f(100.0f, 100.0f, 0.0f);

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(200.0f, 100.0f, 0.0f);

  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(100.0f, 200.0f, 0.0f);
  glEnd();

  // glBegin(GL_QUADS);
  // glColor3i(255, 255, 255);
  // glVertex3i(100, 100, 1);
  // glColor3i(255, 255, 255);
  // glVertex3i(100, 200, 1);
  // glColor3i(255, 255, 255);
  // glVertex3i(200, 200, 1);
  // glColor3i(255, 255, 255);
  // glVertex3i(200, 100, 1);
  // glEnd();

  // glBegin(GL_QUADS);
  // glVertex3i(100, 100, 1);

  // glVertex3i(200, 100, 1);

  // glVertex3i(200, 200, 1);

  // glVertex3i(100, 200, 1);
  // glEnd();
}

int main()
{
  GLFWwindow * window = nullptr;
  glfwSetErrorCallback(errorCallback);

  // Initialize GLFW library.
  if (glfwInit() == false)
  {
    cout << "GLFW initialization failed." << endl;
    system("pause");
    exit(EXIT_FAILURE);
  }

  // Create a windowed mode window and its OpenGL context.
  int const width = 1400;
  int const height = 900;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
  if (!window)
  {
    cout << "GLFW couldn't create a window." << endl;
    glfwTerminate();
    system("pause");
    exit(EXIT_FAILURE);
  }
  glfwSetFramebufferSizeCallback(window, resizeCallback);
  InputManager::getInstance().init(window);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  // Initialize GLEW.
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    cout << "GLEW initialization failed." << endl;
    glfwTerminate();
    system("pause");
    exit(EXIT_FAILURE);
  }

  // Initialize my stuff.
  // if (initGL(width, height) == false)
  //{
  //  std::cout << "My initialization failed." << std::endl;
  //  glfwTerminate();
  //  system("pause");
  //  exit(EXIT_FAILURE);
  //}

  // Frames per second
  double fpsSum = 0;
  size_t fpsIndex = 0;
  vector<double> fpsVector(100, 0.0);

  // Loop until the user closes the window
  double lastTime = glfwGetTime();
  double currentTime = 0.0;
  double deltaTime = 0.0;
  while (!glfwWindowShouldClose(window))
  {
    // Poll for and process events
    glfwPollEvents();

    // update

    // render
    render(width, height);
    CHECKGLERROR();

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Reset inputs
    InputManager::getInstance().resetFrame();

    // Update time
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Frames per second
    fpsSum -= fpsVector[fpsIndex];
    fpsSum += deltaTime;
    fpsVector[fpsIndex] = deltaTime;
    ++fpsIndex;
    if (fpsIndex == fpsVector.size())
    {
      fpsIndex = 0;
    }
    // std::cout << 1.0/(fpsSum/(double)fpsVector.size()) << std::endl;
    auto const fps =
        static_cast<int>(static_cast<double>(fpsVector.size()) / fpsSum);
    glfwSetWindowTitle(window,
                       (std::string("FPS: ") + std::to_string(fps)).c_str());

    // std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);

  return 0;
}