module;

export module crude.core.debug;

import crude.core.alias;

export namespace crude::core
{

constexpr bool gDebug = _DEBUG;

class Debug
{
public:
  enum class Verbosity : uint8
  {
    Off, 
    Error, 
    Warning, 
    Info, 
    All
  };

  enum class Channel : uint16
  {
    General,
    Memory,
    Networking,
    Graphics,
    Collisions,
    Gameplay,
    Sound,
    FileIO,
    GUI,
    All
  };

public:

  static constexpr const char* toString(Verbosity v)
  {
    switch (v)
    {
    case Verbosity::Off:      return "Off";
    case Verbosity::Error:    return "Error";
    case Verbosity::Warning:  return "Warning";
    case Verbosity::Info:     return "Info";
    case Verbosity::All:      return "All";
    }

    return "Unknown";
  }

  static constexpr const char* toString(Channel c)
  {
    switch (c)
    {
    case Channel::General:     return "General";
    case Channel::Memory:      return "Memory";
    case Channel::Networking:  return "Networking";
    case Channel::Graphics:    return "Graphics";
    case Channel::Collisions:  return "Collisions";
    case Channel::Gameplay:    return "Gameplay";
    case Channel::Sound:       return "Sound";
    case Channel::FileIO:      return "File I/O";
    case Channel::GUI:         return "GUI";
    case Channel::All:         return "All";
    }

    return "Unknown-Channel";
  }
};

}