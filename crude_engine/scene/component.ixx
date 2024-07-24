module;

export module crude.scene.component;

export import crude.core.std_containers_heap;

export namespace crude::scene
{

class Component
{
public:
  Component() = default;
  Component(const core::u8string& name) { m_name = name; }
  Component(Component&& other) = default;
  virtual ~Component() = default;
  const core::u8string& getName() const { return m_name; }
  virtual core::u8string getType() = 0;
private:
  core::u8string m_name;
};

}