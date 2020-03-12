/**
 * @author Florian Feuerstein
 * @date 02.08.2016
 *
 */

#pragma once

namespace g3d
{

class GlInfo
{

public:
  static void checkGlErrorImpl(int line, const char * file);

  static void checkFboErrorImpl(int line, const char * file);
};

} // namespace g3d

#ifdef _DEBUG
#define CHECKGLERROR() g3d::GlInfo::checkGlErrorImpl(__LINE__, __FILE__)
#define CHECKGLFBOERROR() g3d::GlInfo::checkFboErrorImpl(__LINE__, __FILE__)
#else
#define CHECKGLERROR()
#define CHECKGLFBOERROR()
#endif
