#pragma warning(disable : 4201)
#include <Graphics3d/Assets/MaterialManager.h>
#include <Graphics3d/Assets/Materials/PBRMaterial.h>
#include <Graphics3d/Assets/Mesh.h>
#include <Graphics3d/Assets/MeshManager.h>
#include <Graphics3d/Assets/Shader.h>
#include <Graphics3d/Assets/ShaderManager.h>
#include <Graphics3d/Assets/Texture.h>
#include <Graphics3d/Assets/TextureManager.h>
#include <Graphics3d/GlInfo.h>
#include <Graphics3d/Input/InputManager.h>

#include <Graphics3d/Components/Camera.h>
#include <Graphics3d/Components/CameraController.h>
#include <Graphics3d/Components/Light.h>
#include <Graphics3d/Components/MeshRenderer.h>
#include <Graphics3d/Components/Transform.h>

#include <Graphics3d/Rendering/SceneRenderer.h>
#include <Graphics3d/Scene/SceneObject.h>

#include "Individual.h"

#include <GLFW/glfw3.h>
#include <gl/glew.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <random>

using namespace std;
using namespace g3d;
using namespace evo;

// Globals
std::unique_ptr<SceneObject> root;
std::unique_ptr<SceneRenderer> renderer;
Camera * camera = nullptr;

void errorCallback(int error, const char * description)
{
  cout << "GLFW error (" << error << "): " << description << endl;
}

void resizeCallback(GLFWwindow * /*pWindow*/, int width, int height)
{
  cout << "resizeCallback" << endl;
  if (width > 0 && height > 0)
  {
    camera->setImageSize(width, height);
    renderer->setImageSize(width, height);
  }
}

bool initGL(int width, int height)
{
  glViewport(0, 0, width, height);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  // Fist init standard assets in this order
  if (ShaderManager::getInstance().initStandardShader() == false)
  {
    std::cout << "Initialization of standard shader failed." << std::endl;
    return false;
  }
  cout << "Shaders loaded." << endl;
  MeshManager::getInstance().initStandardMeshes();
  TextureManager::getInstance().initStandardTextures();
  MaterialManager::getInstance().initStandardMaterials();

  // renderer
  renderer = std::unique_ptr<SceneRenderer>(new SceneRenderer(width, height));

  // graph
  root = std::unique_ptr<SceneObject>(new SceneObject("Root"));

  // load resources from hard disk
  MeshPtr meshSphere;
  try
  {
    meshSphere = MeshManager::getInstance().load(
        "Sphere", "D:\\Eigene Daten\\Dokumente\\3D Modelle\\isosphere.obj");
  }
  catch (std::exception const & exception)
  {
    std::cout << exception.what() << std::endl;
    return false;
  }
  catch (...)
  {
    std::cout << "Unknown exception in Init()." << std::endl;
    return false;
  }

  // plane
  //MeshPtr meshPlane = MeshManager::getInstance().getPlane();
  //SceneObject * floor = new SceneObject("Floor");
  //PBRMaterialPtr pMaterial = PBRMaterialPtr(new PBRMaterial);
  //floor->addComponent(new MeshRenderer(meshPlane, pMaterial));
  //root->addChild(floor);

  // random
  float const terrainSize = 10.0f;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dis(0.0f, terrainSize);

  // apples
  SceneObject * apples = new SceneObject("Apples");
  root->addChild(apples);
  uint32_t numApples = 20;
  PBRMaterialPtr appleMat =
      MaterialManager::getInstance().create<PBRMaterial>("AppleMaterial");
  appleMat->setAlbedo(0.8f, 0.2f, 0.2f);
  for (uint32_t i = 0; i < numApples; ++i)
  {
    SceneObject * apple = new SceneObject("Apple");
    apple->getTransform()->setPosition({dis(mt), 0.0f, dis(mt)});
    apple->getTransform()->setScale({0.5f, 0.5f, 0.5f});
    apple->addComponent(new MeshRenderer(meshSphere, appleMat));
    apples->addChild(apple);
  }

  // individuals
  SceneObject * individuals = new SceneObject("Individuals");
  root->addChild(individuals);
  uint32_t const populationSize = 30;
  for (uint32_t i = 0; i < populationSize; ++i)
  {
    SceneObject * indi = new SceneObject("Indi");
    PBRMaterialPtr indiMat = PBRMaterialPtr(new PBRMaterial);
    indi->addComponent(new MeshRenderer(meshSphere, indiMat));
    indi->addComponent(new Individual());
    indi->getComponent<Individual>()->setApples(apples);
    indi->getComponent<Individual>()->setIndividuals(individuals);
    indi->getTransform()->setPosition({dis(mt), 0.0f, dis(mt)});
    indi->getTransform()->setScale({0.5f, 0.5f, 0.5f});
    individuals->addChild(indi);
  }



  // camera
  SceneObject * cameraObj = new SceneObject("Camera");
  camera = new Camera(width, height, 0.1f, 100.0f, 80.0f);
  cameraObj->addComponent(camera);
  cameraObj->addComponent(new CameraController);
  cameraObj->getTransform()->setPosition(
      {terrainSize / 2.0f, terrainSize / 2.0f, terrainSize / 2.0f});
  root->addChild(cameraObj);

  // lights

  SceneObject * pDirectionalLight1 = new SceneObject("DirectionalLight");
  pDirectionalLight1->addComponent(new Light(Light::DIRECTIONAL));
  pDirectionalLight1->getComponent<Light>()->setIntensity(1.0f);
  pDirectionalLight1->getComponent<Light>()->setColor(
      {237.0f / 255.0f, 237.0f / 255.0f, 237.0f / 255.0f});
  pDirectionalLight1->getTransform()->rotate(45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  root->addChild(pDirectionalLight1);

  //SceneObject * pDirectionalLight2 = new SceneObject("DirectionalLight");
  //pDirectionalLight2->addComponent(new Light(Light::DIRECTIONAL));
  //pDirectionalLight2->getComponent<Light>()->setIntensity(0.4f);
  //pDirectionalLight2->getComponent<Light>()->setColor(
  //    {221.0f / 255.0f, 237.0f / 255.0f, 240.0f / 255.0f});
  //pDirectionalLight2->getTransform()->rotate(180.0f+45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  //root->addChild(pDirectionalLight2);

  CHECKGLERROR();
  return true;
}

void hackyUpdates(double deltaTime)
{
  static float const terrainSize = 10.0f;
  static std::random_device rd;
  static std::mt19937 mt(rd());
  static std::uniform_real_distribution<float> dis(0.0f, terrainSize);
  static double passedTime = 0.0;
  static double newAppleTime = 7.0;

  passedTime += deltaTime;
  if (passedTime >= newAppleTime)
  {
    passedTime -= newAppleTime;
    SceneObject * apples = root->getChild("Apples");
    uint32_t numApples = 2;
    for (uint32_t i = 0; i < numApples; ++i)
    {
      SceneObject * apple = new SceneObject("Apple");
      apple->getTransform()->setPosition({dis(mt), 0.0f, dis(mt)});
      apple->getTransform()->setScale({0.5f, 0.5f, 0.5f});
      apple->addComponent(new MeshRenderer(
          MeshManager::getInstance().get("Sphere"),
          MaterialManager::getInstance().get<PBRMaterial>("AppleMaterial")));
      apples->addChild(apple);
    }
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
  int const width = 1600;
  int const height = 1000;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
  if (initGL(width, height) == false)
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
    root->update(deltaTime);
    hackyUpdates(deltaTime);
    SceneObject::updateSceneGraph();

    // render
    renderer->render(root.get());
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