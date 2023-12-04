#pragma once

#include <vector>
#include <string>

namespace crude_engine
{

class Requiring_Extensions
{
public:
  static const std::vector<const char*>& requiredExtensions() = delete;
};

}
