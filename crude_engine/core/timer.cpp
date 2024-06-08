#include <chrono>
#include <thread>

module crude.core.timer;

namespace crude::core
{

bool Timer::frameElasped(core::float64& elapsed) noexcept
{
  const auto& currentFrameTime = high_resolution_clock::now();
  elapsed = std::chrono::duration_cast<duration_f64>(currentFrameTime - m_lastFrameTime).count();

  if (elapsed > m_invFrameRate)
  {
    m_lastFrameTime = currentFrameTime;
    return true;
  }

  std::this_thread::yield();
  return false;
}

void Timer::setFrameRate(core::int32 frameRate) noexcept
{
  m_frameRate = frameRate;
  m_invFrameRate = 1.0 / static_cast<core::float64>(frameRate);
}

}