/**
 * @author Florian Feuerstein
 * @date 02.08.2016
 *
 */

#pragma once

namespace G3d
{

class GlInfo
{

public:
  static void checkGlErrorImpl(int line, const char * file);

  static void checkFboErrorImpl(int line, const char * file);
};

} // namespace G3d

#ifdef _DEBUG
#define CHECKGLERROR() G3d::GlInfo::checkGlErrorImpl(__LINE__, __FILE__)
#define CHECKGLFBOERROR() G3d::GlInfo::checkFboErrorImpl(__LINE__, __FILE__)
#else
#define CHECKGLERROR()
#define CHECKGLFBOERROR()
#endif
