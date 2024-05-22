module;

export module crude.core.compare;

export namespace crude::core
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

} // namespace crude