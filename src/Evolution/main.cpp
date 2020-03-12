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

#include <GLFW/glfw3.h>
#include <gl/glew.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace g3d;

// Globals
std::unique_ptr<SceneObject> pRoot;
std::unique_ptr<SceneRenderer> pRenderer;
Camera * pCamera = nullptr;

void errorCallback(int error, const char * description)
{
  cout << "GLFW error (" << error << "): " << description << endl;
}

void resizeCallback(GLFWwindow * /*pWindow*/, int width, int height)
{
  cout << "resizeCallback" << endl;
  if (width > 0 && height > 0)
  {
    pCamera->setImageSize(width, height);
    pRenderer->setImageSize(width, height);
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
  pRenderer = std::unique_ptr<SceneRenderer>(new SceneRenderer(width, height));

  // graph
  pRoot = std::unique_ptr<SceneObject>(new SceneObject("Root"));

  // load resources from hard disk
  MeshPtr meshSphere;
  try
  {
    meshSphere = MeshManager::getInstance().load(
        "Sphere", "D:\\Eigene Daten\\Dokumente\\3D Modelle\\monkey.obj");
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
  MeshPtr meshPlane = MeshManager::getInstance().getPlane();
  SceneObject * floor = new SceneObject("Floor");
  PBRMaterialPtr pMaterial = PBRMaterialPtr(new PBRMaterial);
  floor->addComponent(new MeshRenderer(meshPlane, pMaterial));
  pRoot->addChild(floor);

  // spheres
  int numRows = 7;
  int numColumns = 7;
  float spacing = 2.5;

  for (int row = 0; row < numRows; ++row)
  {
    for (int col = 0; col < numColumns; ++col)
    {
      pMaterial = PBRMaterialPtr(new PBRMaterial);
      pMaterial->setAlbedo(1.0f, 0.0f, 0.0f);
      pMaterial->setMetallic((float)row / (float)numRows);
      pMaterial->setRoughness(
          glm::clamp((float)col / (float)numColumns, 0.05f, 1.0f));

      SceneObject * pSphere = new SceneObject("Sphere");
      pSphere->addComponent(new MeshRenderer(meshSphere, pMaterial));
      pSphere->getTransform()->setPosition(
          glm::vec3((float)(col - (numColumns / 2)) * spacing,
                    (float)(row - (numRows / 2)) * spacing, 0.0f));

      pRoot->addChild(pSphere);
    }
  }

  // camera
  SceneObject * pCameraObj = new SceneObject("Camera");
  pCamera = new Camera(width, height, 0.1f, 100.0f, 80.0f);
  pCameraObj->addComponent(pCamera);
  pCameraObj->addComponent(new CameraController);
  pCameraObj->getTransform()->lookAt(glm::vec3(0.0f, 0.0f, -5.0f),
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
  pRoot->addChild(pCameraObj);

  // lights
  SceneObject * pDirectionalLight = new SceneObject("DirectionalLight");
  pDirectionalLight->addComponent(new Light(Light::DIRECTIONAL));
  pDirectionalLight->getComponent<Light>()->setIntensity(10.0f);
  pDirectionalLight->getTransform()->rotate(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  pRoot->addChild(pDirectionalLight);

  CHECKGLERROR();
  return true;
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
    pRoot->update(deltaTime);

    // render
    pRenderer->render(pRoot.get());
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