#pragma warning(disable : 4201)
#include <gl/glew.h>
#include "Graphics/GlInfo.h"
#include "Input/InputManager.h"
#include "PopulationCounter.h"
#include "World.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace evo;

bool initEvo(int width, int height, World & world)
{
  Vector3f const worldSize(800.0f, 100.0f, 800.0f);
  glViewport(0, 0, width, height);
  world.createTerrain(worldSize);
  world.getCamera().setWindowSize(width, height);
  world.getCamera().setFocus({worldSize.x() / 2, 0.0f, worldSize.z() / 2});
  world.getCamera().setDistance((worldSize.x() + worldSize.z()) / 2);
  world.addRandomPeople(g_startPopulation);
  uint32_t const numApplesStart = static_cast<uint32_t>(
      static_cast<double>(g_numNewApples) * g_appleLifeTime / g_newAppleTime);
  world.addRandomApples(numApplesStart);
  return true;
}

void errorCallback(int error, const char * description)
{
  cout << "GLFW error (" << error << "): " << description << endl;
}

void resizeCallback(GLFWwindow * window, int width, int height)
{
  glfwMakeContextCurrent(window);
  glViewport(0, 0, width, height);
  cout << "resizeCallback" << endl;
  if (width > 0 && height > 0)
  {
  }
}

void runNormalMode()
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
  int const windowWidth = 800;
  int const windowHeight = 800;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  window =
      glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
  glfwSetWindowPos(window, 100, 50);
  if (!window)
  {
    cout << "GLFW couldn't create a window." << endl;
    glfwTerminate();
    system("pause");
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  InputManager::getInstance().init(window);
  glfwSwapInterval(0);
  glfwSetFramebufferSizeCallback(window, resizeCallback);

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
  World world;
  world.createTerrain({800, 100, 800});
  if (initEvo(windowWidth, windowHeight, world) == false)
  {
    std::cout << "My initialization failed." << std::endl;
    glfwTerminate();
    system("pause");
    exit(EXIT_FAILURE);
  }

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
    world.update(deltaTime);

    // render
    world.render();
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
}

void runFastMode()
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
  int const windowWidth = 1200;
  int const windowHeight = 800;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  window =
      glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
  glfwSetWindowPos(window, 100, 50);
  if (!window)
  {
    cout << "GLFW couldn't create a window." << endl;
    glfwTerminate();
    system("pause");
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  InputManager::getInstance().init(window);
  glfwSwapInterval(0);
  glfwSetFramebufferSizeCallback(window, resizeCallback);

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
  World world;
  if (initEvo(windowWidth, windowHeight, world) == false)
  {
    std::cout << "My initialization failed." << std::endl;
    glfwTerminate();
    system("pause");
    exit(EXIT_FAILURE);
  }

  double constexpr deltaTime = 1.0 / 60.0;
  while (!glfwWindowShouldClose(window))
  {
    // Poll for and process events
    glfwPollEvents();

    // update
    world.update(deltaTime);
    // Reset inputs
    InputManager::getInstance().resetFrame();
    world.update(deltaTime);
    world.update(deltaTime);
    world.update(deltaTime);
    world.update(deltaTime);
    world.update(deltaTime);
    world.update(deltaTime);
    world.update(deltaTime);

    // render
    world.render();
    CHECKGLERROR();

    // Swap front and back buffers
    glfwSwapBuffers(window);
  }
  glfwTerminate();
}

int main()
{
  runNormalMode();
  // runFastMode();

  exit(EXIT_SUCCESS);
}