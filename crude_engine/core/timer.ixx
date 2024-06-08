module;

#include <chrono>

export module crude.core.timer;

export import crude.core.alias;

export namespace crude::core
{

class Timer
{
private:
  using duration_f64          = std::chrono::duration<core::float64>;
  using high_resolution_clock = std::chrono::high_resolution_clock;
  using time_point            = std::chrono::steady_clock::time_point;
public:
  Timer() noexcept : m_lastFrameTime(high_resolution_clock::now()) {}
  Timer(core::int32 frameRate) noexcept : Timer() { setFrameRate(frameRate); }
  core::int32 getFrameRate() const noexcept { return m_frameRate; }
  bool frameElasped(core::float64& elapsed) noexcept;
  void setFrameRate(core::int32 frameRate) noexcept;
private:
  time_point     m_lastFrameTime;
  core::int32    m_frameRate;
  core::float64  m_invFrameRate;
};

}