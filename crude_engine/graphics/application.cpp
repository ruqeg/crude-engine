module crude.graphics.application;

namespace crude::graphics
{

Application::Application(const char*   pApplicationName,
                         core::uint32  applicationVersion,
                         const char*   pEngineName,
                         core::uint32  engineVersion,
                         core::uint32  apiVersion)
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
