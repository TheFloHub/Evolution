#include "Math/SomeTest.h"
namespace math
{
Vector3f someTestFunction(Matrix3f const & matrix)
{
  return matrix * Vector3f(1.0f, 1.0f, 1.0f);
}
} // namespace math
