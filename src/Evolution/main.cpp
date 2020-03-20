#pragma warning(disable : 4201)
#include <gl/glew.h>
#include "Graphics3d/GlInfo.h"
#include "Graphics3d/Input/InputManager.h"
#include <GLFW/glfw3.h>

#include "EvoSim.h"
#include "PopulationCounter.h"


#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace g3d;
using namespace evo;


// evo
EvoSim & evoSim = EvoSim::get();

// apple
uint32_t g_numNewApples = 50;


// global distributions
std::uniform_real_distribution<double> g_reproDis(0.0, 1.0f);

// population chart
PopulationCounter g_populationPersons(1.0, evoSim.m_width);


bool initGL(int width, int height)
{
  glViewport(0, 0, width, height);

  // people
  for (uint32_t i = 0; i < g_startPopulation; ++i)
  {
    Person p;
    p.m_position.x() = evoSim.m_xdis(evoSim.m_rng);
    p.m_position.y() = evoSim.m_ydis(evoSim.m_rng);
    evoSim.m_persons.emplace_back(p);
  }

  // apples
  uint32_t const numApplesStart = static_cast<uint32_t>(
      static_cast<double>(g_numNewApples) * g_appleLifeTime / g_newAppleTime);
  for (uint32_t i = 0; i < numApplesStart; ++i)
  {
    Apple a;
    a.m_position.x() = evoSim.m_xdis(evoSim.m_rng);
    a.m_position.y() = evoSim.m_ydis(evoSim.m_rng);
    evoSim.m_apples.emplace_back(a);
  }

  return true;
}

void update(double deltaTime)
{
  static double passedAppleTime = 0.0;

  if (InputManager::getInstance().getKeyDown(KEY_DOWN))
  {
    --g_numNewApples;
    cout << g_numNewApples << endl;
  }

  // population counters
  g_populationPersons.add(deltaTime, evoSim.m_persons.size());

  // TODO: think about order, deltaTime is from last frame
  // people
  std::vector<size_t> died;
  std::vector<Person> born;
  for (size_t pi = 0; pi < evoSim.m_persons.size(); ++pi)
  {
    auto & p = evoSim.m_persons[pi];

    // movement
    p.m_movementTrait->move(p, deltaTime);

    // find closest apple
    double closestAppleDistance = std::numeric_limits<double>::max();
    size_t closestAppleIndex = evoSim.m_apples.size();
    for (size_t ai = 0; ai < evoSim.m_apples.size(); ++ai)
    {
      double const distSq =
          (p.m_position - evoSim.m_apples[ai].m_position).squaredNorm();
      if (distSq < closestAppleDistance)
      {
        closestAppleDistance = distSq;
        closestAppleIndex = ai;
      }
    }

    // eat apple
    double const irsq = p.m_interactionRange * p.m_interactionRange;
    if (p.isHungry() && closestAppleIndex < evoSim.m_apples.size() &&
        closestAppleDistance < irsq)
    {
      auto const apple = std::begin(evoSim.m_apples) + closestAppleIndex;
      p.m_energy += apple->m_energy;
      p.m_energy = std::min(p.m_maxEnergy, p.m_energy);
      evoSim.m_apples.erase(apple);
    }

    // reproduction
    p.m_reproductionPassedTime += deltaTime;
    if (p.m_reproductionPassedTime >= p.m_reproductionTime)
    {
      p.m_reproductionPassedTime -= p.m_reproductionTime;
      if (g_reproDis(evoSim.m_rng) <= p.m_reproductionProbability)
      {
        Person newP = p;
        newP.m_movementTrait->setDefault();
        newP.m_energy = newP.m_maxEnergy;
        newP.m_reproductionPassedTime = 0.0;

        // speed mutation
         //std::uniform_real_distribution<double> speedMutationDis(
         //   0.9 * p.m_speed, 1.1 * p.m_speed);
         //newP.m_speed = speedMutationDis(evoSim.m_rng);

        // sensing range mutation
         std::uniform_real_distribution<double> sensingMutationDis(
            0.9 * p.m_sensingRange, 1.1 * p.m_sensingRange);
        newP.m_sensingRange = sensingMutationDis(evoSim.m_rng);

        born.emplace_back(newP);
      }
    }

    // energy
    p.m_energy -= (p.m_speed /** p.m_engergySpeedLossFactor*/ +
                   p.m_energyFixedLossPerSec + p.m_sensingRange) *
        deltaTime;

    // death
    if (p.m_energy <= 0.0)
    {
      died.emplace_back(pi);
    }
  }

  for (auto riter = std::rbegin(died); riter != std::rend(died); ++riter)
  {
    evoSim.m_persons.erase(std::begin(evoSim.m_persons) + (*riter));
  }
  for (auto & b : born)
  {
    evoSim.m_persons.emplace_back(b);
  }

  // end people

  // apple decay
  auto appleIter = std::begin(evoSim.m_apples);
  while (appleIter != std::end(evoSim.m_apples))
  {
    appleIter->m_passedTime += deltaTime;
    if (appleIter->m_passedTime >= appleIter->m_lifeTime)
    {
      appleIter = evoSim.m_apples.erase(appleIter);
    }
    else
    {
      ++appleIter;
    }
  }

  // new apples
  passedAppleTime += deltaTime;
  if (passedAppleTime >= g_newAppleTime)
  {
    passedAppleTime -= g_newAppleTime;
    for (uint32_t i = 0; i < g_numNewApples; ++i)
    {
      Apple a;
      a.m_position.x() = evoSim.m_xdis(evoSim.m_rng);
      a.m_position.y() = evoSim.m_ydis(evoSim.m_rng);
      evoSim.m_apples.emplace_back(a);
    }
  }
}

void render(int width, int height)
{
  glViewport(0, 0, width, height);
  glDisable(GL_CULL_FACE);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, 0, height, -10, 10);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // persons
  glPointSize(14.0f);
  glBegin(GL_POINTS);
  for (auto const & p : evoSim.m_persons)
  {
    //double const c = p.m_energy / p.m_maxEnergy;
    //double const c = p.m_speed / 40.0;
    double const c = p.m_sensingRange / 120.0;
    glColor3d(1.0, c, c);
    glVertex3d(p.m_position.x(), p.m_position.y(), 0.0);
  }
  glEnd();

  // apples
  glPointSize(7.0f);
  glBegin(GL_POINTS);
  glColor3f(0.2f, 0.75f, 0.1f);
  for (auto const & a : evoSim.m_apples)
  {
    glVertex3d(a.m_position.x(), a.m_position.y(), 0.0);
  }
  glEnd();

  // charts
  g_populationPersons.render(0, evoSim.m_width, evoSim.m_yStart);

  // chart separation
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 1.0f);
  glVertex3i(0, evoSim.m_yStart, 0);
  glVertex3i(evoSim.m_width, evoSim.m_yStart, 0);
  glEnd();
}

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

void runFastMode() {
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

  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  window = glfwCreateWindow(evoSim.m_width, evoSim.m_height, "Hello World",
                            NULL, NULL);
  glfwSetWindowPos(window, 600, 100);
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
  if (initGL(evoSim.m_width, evoSim.m_height) == false)
  {
    std::cout << "My initialization failed." << std::endl;
    glfwTerminate();
    system("pause");
    exit(EXIT_FAILURE);
  }


  double const fixedDeltaTime = 1.0 / 60.0;
  while (!glfwWindowShouldClose(window))
  {
    // Poll for and process events
    glfwPollEvents();

    // update
    update(fixedDeltaTime);
    // Reset inputs
    InputManager::getInstance().resetFrame();
    update(fixedDeltaTime);
    update(fixedDeltaTime);
    update(fixedDeltaTime);
    update(fixedDeltaTime);
    update(fixedDeltaTime);
    update(fixedDeltaTime);
    update(fixedDeltaTime);

    // render
    render(evoSim.m_width, evoSim.m_height);
    CHECKGLERROR();

    // Swap front and back buffers
    glfwSwapBuffers(window);

  }

  glfwTerminate();
}

void runNormalMode() {
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

  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  window = glfwCreateWindow(evoSim.m_width, evoSim.m_height, "Hello World",
                            NULL, NULL);
  glfwSetWindowPos(window, 600, 100);
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
  if (initGL(evoSim.m_width, evoSim.m_height) == false)
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
    update(deltaTime);

    // render
    render(evoSim.m_width, evoSim.m_height);
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
}

int main()
{
  runFastMode();

  exit(EXIT_SUCCESS);
}