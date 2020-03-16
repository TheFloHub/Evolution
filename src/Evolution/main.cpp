#pragma warning(disable : 4201)
#include <gl/glew.h>
#include "Graphics3d/GlInfo.h"
#include "Graphics3d/Input/InputManager.h"
#include <GLFW/glfw3.h>
#include <Math/MathTypes.h>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace g3d;
using namespace math;

struct Person
{
  Vector2d m_position{0.0, 0.0};
  double m_speed{50.0};
  double m_sensingRange{80.0};
  double m_interactionRange{20.0};
  double m_size{1.0};
  double m_dirAngle{0.0};
  double m_vitality{200.0};
  double m_vitalityDecreasePerSec{10};
  double m_reproductionProbability{0.03};
  double m_reproductionTime{1.0};
  double m_reproductionPassedTime{0.0};
};

struct Apple
{
  Vector2d m_position{0.0, 0.0};
};

// gui
int const g_width = 1400;
int const g_height = 900;
int const g_yStart = 100;

// population
uint32_t const g_startPopulation = 20;
uint32_t const g_numNewApples = 16;
std::vector<Person> g_persons;
std::vector<Apple> g_apples;

// population chart
struct PopulationCounter
{
  PopulationCounter(double const timeDiscretization, size_t const bufferSize)
      : m_timeDiscretization(timeDiscretization), m_chart(bufferSize, 0)
  {
  }

  // TODO: actually scale count with time and add parts to the next count
  // this one assumes that framerate stays constant and ignores time overflow
  void add(double deltaTime, size_t count)
  {
    m_passedTime += deltaTime;
    m_totalCount += count;
    ++m_numCounts;
    if (m_passedTime >= m_timeDiscretization)
    {
      m_chart[m_index] = static_cast<uint32_t>(m_totalCount / m_numCounts);
      ++m_index;
      m_passedTime = 0.0;
      m_totalCount = 0;
      m_numCounts = 0;
      cout << m_chart[m_index - 1] << endl;
      if (m_index >= m_chart.size())
      {
        cout << "Population buffer overflow!" << endl; 
        m_index = 0;
        m_filled = true;
      }
    }
  }

  void render(int const startY, int const width, int const height)
  {
    if (m_index == 0 && !m_filled)
    {
      return;
    }

    uint32_t const maxCount =
        *std::max_element(std::begin(m_chart), std::end(m_chart));
    if (maxCount == 0)
    {
      return;
    }
    double const yScale =
        static_cast<double>(height) / static_cast<double>(maxCount);

    if (m_index == 0)
    {
        // TODO:
      return;
    }
    double const xScale = 
        static_cast<double>(width) / static_cast<double>(m_index);

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    double lastX = 0.0;
    for (int i = 0; i < m_index; ++i)
    {
      double const x = (i + 1) * xScale;
      double const y = startY + yScale * static_cast<double>(m_chart[i]);
      glVertex3d(lastX, startY, 0.0);
      glVertex3d(x, startY, 0.0);
      glVertex3d(x, y, 0.0);
      glVertex3d(lastX, y, 0.0);
      lastX = x;
    }
    glEnd();
  }

  double const m_timeDiscretization;
  double m_passedTime{0.0};
  size_t m_totalCount{0};
  size_t m_numCounts{0};
  std::vector<uint32_t> m_chart;
  size_t m_index{0};
  bool m_filled{false};
};

PopulationCounter g_populationPersons(1.0, g_width);

// random
std::random_device g_rd;
std::mt19937 g_rng(g_rd());
std::uniform_real_distribution<float> g_xdis(0.0f, static_cast<float>(g_width));
std::uniform_real_distribution<float> g_ydis(static_cast<float>(g_yStart),
                                             static_cast<float>(g_height));

bool initGL(int width, int height)
{
  glViewport(0, 0, width, height);

  // people
  for (uint32_t i = 0; i < g_startPopulation; ++i)
  {
    Person p;
    p.m_position.x() = g_xdis(g_rng);
    p.m_position.y() = g_ydis(g_rng);
    g_persons.emplace_back(p);
  }

  // apples
  for (uint32_t i = 0; i < g_numNewApples; ++i)
  {
    Apple a;
    a.m_position.x() = g_xdis(g_rng);
    a.m_position.y() = g_ydis(g_rng);
    g_apples.emplace_back(a);
  }

  return true;
}

void update(double deltaTime)
{
  static double passedAppleTime = 0.0;
  static double newAppleTime = 7.0;
  static std::uniform_real_distribution<float> angleDis(-0.3f, 0.3f);
  static std::uniform_real_distribution<double> reproDis(0.0, 1.0f);

  // population counters
  g_populationPersons.add(deltaTime, g_persons.size());
  
  // people
  for (uint32_t pi = 0; pi < g_persons.size(); ++pi)
  {
    auto & p = g_persons[pi];

    // vitality and color
    p.m_vitality -= p.m_vitalityDecreasePerSec * deltaTime;

    // death
    if (p.m_vitality <= 0.0)
    {
      g_persons.erase(std::begin(g_persons) + pi);
    }

    // movement
    p.m_dirAngle += angleDis(g_rng);
    p.m_position.x() += deltaTime * p.m_speed * std::cos(p.m_dirAngle);
    p.m_position.y() += deltaTime * p.m_speed * std::sin(p.m_dirAngle);
    p.m_position.x() =
        std::clamp(p.m_position.x(), 0.0, static_cast<double>(g_width));
    p.m_position.y() =
        std::clamp(p.m_position.y(), static_cast<double>(g_yStart),
                   static_cast<double>(g_height));

    // eat apples
    double const irsq = p.m_interactionRange * p.m_interactionRange;
    double closestAppleDistance = std::numeric_limits<double>::max();
    size_t closestAppleIndex = g_apples.size();
    for (size_t ai = 0; ai < g_apples.size(); ++ai)
    {
      double const distSq =
          (p.m_position - g_apples[ai].m_position).squaredNorm();
      if (distSq <= irsq && distSq < closestAppleDistance)
      {
        closestAppleDistance = distSq;
        closestAppleIndex = ai;
      }
    }

    if (closestAppleIndex < g_apples.size())
    {
      g_apples.erase(std::begin(g_apples) + closestAppleIndex);
      p.m_vitality += 100.0;
    }

    // reproduction
    p.m_reproductionPassedTime += deltaTime;
    if (p.m_reproductionPassedTime >= p.m_reproductionTime)
    {
      p.m_reproductionPassedTime -= p.m_reproductionTime;
      if (reproDis(g_rng) <= p.m_reproductionProbability)
      {
        Person newP = p;
        newP.m_dirAngle = 0.0;
        newP.m_vitality = 200.0;
        newP.m_reproductionPassedTime = 0.0;
        g_persons.emplace_back(newP);
      }
    }
  }
  // end people

  // new apples
  passedAppleTime += deltaTime;
  if (passedAppleTime >= newAppleTime)
  {
    passedAppleTime -= newAppleTime;
    for (uint32_t i = 0; i < g_numNewApples; ++i)
    {
      Apple a;
      a.m_position.x() = g_xdis(g_rng);
      a.m_position.y() = g_ydis(g_rng);
      g_apples.emplace_back(a);
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
  for (auto const & p : g_persons)
  {
    double const c = p.m_vitality / 255.0;
    glColor3d(1.0, c, c);
    glVertex3d(p.m_position.x(), p.m_position.y(), 0.0);
  }
  glEnd();

  // apples
  glPointSize(7.0f);
  glBegin(GL_POINTS);
  glColor3f(0.2f, 0.75f, 0.1f);
  for (auto const & a : g_apples)
  {
    glVertex3d(a.m_position.x(), a.m_position.y(), 0.0);
  }
  glEnd();

  // charts
  g_populationPersons.render(0, g_width, g_yStart);

  // chart separation
  glBegin(GL_LINES);
  glColor3f(1.0f, 0.0f, 1.0f);
  glVertex3i(0, g_yStart, 0);
  glVertex3i(g_width, g_yStart, 0);
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

  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  window = glfwCreateWindow(g_width, g_height, "Hello World", NULL, NULL);
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
  if (initGL(g_width, g_height) == false)
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
    render(g_width, g_height);
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