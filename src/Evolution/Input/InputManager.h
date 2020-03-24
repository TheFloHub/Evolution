#pragma once
#include "Math/MathTypes.h"
#include <vector>

struct GLFWwindow;

namespace evo
{
enum MouseInput
{
  MOUSE_BUTTON_1 = 0,
  MOUSE_BUTTON_2 = 1,
  MOUSE_BUTTON_3 = 2,
};

enum KeyboardInput
{
  KEY_SPACE = 32,
  KEY_0 = 48,
  KEY_1 = 49,
  KEY_2 = 50,
  KEY_3 = 51,
  KEY_4 = 52,
  KEY_5 = 53,
  KEY_6 = 54,
  KEY_7 = 55,
  KEY_8 = 56,
  KEY_9 = 57,
  KEY_A = 65,
  KEY_B = 66,
  KEY_C = 67,
  KEY_D = 68,
  KEY_E = 69,
  KEY_F = 70,
  KEY_G = 71,
  KEY_H = 72,
  KEY_I = 73,
  KEY_J = 74,
  KEY_K = 75,
  KEY_L = 76,
  KEY_M = 77,
  KEY_N = 78,
  KEY_O = 79,
  KEY_P = 80,
  KEY_Q = 81,
  KEY_R = 82,
  KEY_S = 83,
  KEY_T = 84,
  KEY_U = 85,
  KEY_V = 86,
  KEY_W = 87,
  KEY_X = 88,
  KEY_Y = 89,
  KEY_Z = 90,
  KEY_ESCAPE = 256,
  KEY_ENTER = 257,
  KEY_TAB = 258,
  KEY_BACKSPACE = 259,
  KEY_INSERT = 260,
  KEY_DELETE = 261,
  KEY_RIGHT = 262,
  KEY_LEFT = 263,
  KEY_DOWN = 264,
  KEY_UP = 265,
  KEY_PAGE_UP = 266,
  KEY_PAGE_DOWN = 267,
  KEY_HOME = 268,
  KEY_END = 269,
  KEY_CAPS_LOCK = 280,
  KEY_SCROLL_LOCK = 281,
  KEY_NUM_LOCK = 282,
  KEY_PRINT_SCREEN = 283,
  KEY_PAUSE = 284,
  KEY_F1 = 290,
  KEY_F2 = 291,
  KEY_F3 = 292,
  KEY_F4 = 293,
  KEY_F5 = 294,
  KEY_F6 = 295,
  KEY_F7 = 296,
  KEY_F8 = 297,
  KEY_F9 = 298,
  KEY_F10 = 299,
  KEY_F11 = 300,
  KEY_F12 = 301,
  KEY_KP_DECIMAL = 330,
  KEY_KP_DIVIDE = 331,
  KEY_KP_MULTIPLY = 332,
  KEY_KP_SUBTRACT = 333,
  KEY_KP_ADD = 334,
  KEY_KP_ENTER = 335,
  KEY_KP_EQUAL = 336,
  KEY_LEFT_SHIFT = 340,
  KEY_LEFT_CONTROL = 341,
  KEY_LEFT_ALT = 342,
  KEY_LEFT_SUPER = 343,
  KEY_RIGHT_SHIFT = 344,
  KEY_RIGHT_CONTROL = 345,
  KEY_RIGHT_ALT = 346,
  KEY_RIGHT_SUPER = 347
};

class InputManager
{
public:
  static InputManager & getInstance();

  ~InputManager();

  void init(GLFWwindow * pWindow);

  void resetFrame();

  bool getKey(KeyboardInput name) const;

  bool getKeyDown(KeyboardInput name) const;

  bool getKeyUp(KeyboardInput name) const;

  bool getMouseButton(MouseInput name) const;

  bool getMouseButtonDown(MouseInput name) const;

  bool getMouseButtonUp(MouseInput name) const;

  double getMouseDeltaX() const;

  double getMouseDeltaY() const;

  double getMouseWheelDeltaX() const;

  double getMouseWheelDeltaY() const;

  Vector2d getMousePosition() const;

private:
  InputManager();

  InputManager(InputManager const &) = delete;

  InputManager & operator=(InputManager const &) = delete;

  static const size_t MAX_KEY_ID;

  static const size_t MAX_MOUSE_BUTTON_ID;

  static void keyCallback(GLFWwindow * pWindow, int key, int scancode,
                          int action, int mods);

  static void mouseButtonCallback(GLFWwindow * pWindow, int button, int action,
                                  int mods);

  static void cursorPositionCallback(GLFWwindow * window, double xpos,
                                     double ypos);

  static void scrollCallback(GLFWwindow * window, double xoffset,
                             double yoffset);

  // TODO:
  //static void cursorEnterCallback(GLFWwindow * window, int entered);


  std::vector<unsigned char> mKey;

  std::vector<unsigned char> mKeyDown;

  std::vector<unsigned char> mKeyUp;

  std::vector<unsigned char> mMouseButton;

  std::vector<unsigned char> mMouseButtonDown;

  std::vector<unsigned char> mMouseButtonUp;

  double mLastX;

  double mLastY;

  double mDiffX;

  double mDiffY;

  double m_mouseWheelDeltaX{0.0};
  double m_mouseWheelDeltaY{0.0};
};

} // namespace evo
