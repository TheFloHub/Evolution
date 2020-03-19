#include "EvoSim.h"

namespace evo
{
EvoSim & EvoSim::get()
{
  static EvoSim sim;
  return sim;
}
} // namespace evo