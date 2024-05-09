module crude_engine.graphics.vulkan.application;

namespace crude_engine 
{

Application::Application(const char*  pApplicationName,
                         uint32       applicationVersion,
                         const char*  pEngineName,
                         uint32       engineVersion,
                         uint32       apiVersion)
{
  this->sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  this->pNext              = nullptr;

  this->pApplicationName   = pApplicationName;
  this->applicationVersion = applicationVersion;
  this->pEngineName        = pEngineName;
  this->engineVersion      = engineVersion;
  this->apiVersion         = apiVersion;
}

}
