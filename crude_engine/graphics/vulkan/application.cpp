#include "application.hpp"

namespace crude_engine 
{

Application::Application(const char8*  pApplicationName,
                         uint32        applicationVersion,
                         const char8*  pEngineName,
                         uint32        engineVersion,
                         uint32        apiVersion)
{
  this->sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  this->pNext              = NULL;

  this->pApplicationName   = pApplicationName;
  this->applicationVersion = applicationVersion;
  this->pEngineName        = pEngineName;
  this->engineVersion      = engineVersion;
  this->apiVersion         = apiVersion;
}

}
