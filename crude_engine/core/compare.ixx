module;

export module crude_engine.core.compare;

export namespace crude_engine
{

class Flexible_Less
{
public:
  template <class T1, class T2>
  inline constexpr bool operator()(const T1& lhs, const T2& rhs) const noexcept
  {
    return lhs < rhs;
  }
};

} // namespace crude_engine