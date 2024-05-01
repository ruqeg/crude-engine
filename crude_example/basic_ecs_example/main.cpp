#ifdef _CRUDE_EXAMPLE_BASIC_ECS

#include <ecs/world.hpp>
#include <ecs/entity.hpp>
#include <ecs/query.hpp>

#include <iostream>

struct Force : public crude_engine::Default_Component_Container<Force>
{
  Force() = default;
  Force(int x) : x(x) {}
  int x;
};

struct Speed : public crude_engine::Default_Component_Container<Speed>
{
  Speed() = default;
  Speed(int x) : x(x) {}
  int x;
};

struct Power : public crude_engine::Default_Component_Container<Power>
{
  Power() = default;
  Power(int x) : x(x) {}
  int x;
};

struct Length : public crude_engine::Default_Component_Container<Length>
{
  Length() = default;
  Length(int x) : x(x) {}
  int x;
};

int APIENTRY wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nCmdShow)
{
  // init console
  AllocConsole();
  FILE* dummy;
  auto s = freopen_s(&dummy, "CONOUT$", "w", stdout);
  {
    crude_engine::World ecs;
    
    crude_engine::Entity entity = ecs.entity()
      .add<Force>()
      .add<Speed>();
    
    //crude_engine::World ecs;
    //
    //crude_engine::Entity entity = ecs.entity()
    //  .add<Force>()
    //  .add<Speed>()
    //  .add<Power>();
    //
    //crude_engine::Entity entity2 = ecs.entity()
    //  .add<Force>();
    //
    //crude_engine::Entity entity3 = ecs.entity()
    //  .add<Force>()
    //  .add<Speed>();
    //
    //crude_engine::Entity entity4 = ecs.entity()
    //  .add<Force>()
    //  .add<Power>();
    //
    //crude_engine::Entity entity5 = ecs.entity()
    //  .add<Force>()
    //  .add<Speed>()
    //  .add<Power>()
    //  .add<Length>();
    //
    //crude_engine::Entity entityCopy = ecs.entity()
    //  .add<Force>()
    //  .add<Speed>()
    //  .add<Power>();
    //
    //std::cout << entity.hasComponent<Force>() << " " << entity.hasComponent<Speed>() << " " << entity.hasComponent<Power>() << " " << entity.hasComponent<Length>() << std::endl;
    //std::cout << entity2.hasComponent<Force>() << " " << entity2.hasComponent<Speed>() << " " << entity2.hasComponent<Power>() << " " << entity2.hasComponent<Length>() << std::endl;
    //std::cout << entity3.hasComponent<Force>() << " " << entity3.hasComponent<Speed>() << " " << entity3.hasComponent<Power>() << " " << entity3.hasComponent<Length>() << std::endl;
    //std::cout << entity4.hasComponent<Force>() << " " << entity4.hasComponent<Speed>() << " " << entity4.hasComponent<Power>() << " " << entity4.hasComponent<Length>() << std::endl;
    //std::cout << entity5.hasComponent<Force>() << " " << entity5.hasComponent<Speed>() << " " << entity5.hasComponent<Power>() << " " << entity5.hasComponent<Length>() << std::endl;
    //
    //Force f1; f1.x = 5;
    //Force f2; f2.x = 4;
    //entity.set<Force>(f1);
    //std::cout << entity.get<Force>().x << std::endl;
    //entityCopy.set<Force>(f2);
    //std::cout << entity.get<Force>().x << " " << entityCopy.get<Force>().x << std::endl;
    //entityCopy.remove<Force>();
    //std::cout << entity.hasComponent<Force>() << " " << entityCopy.hasComponent<Force>() << std::endl;
    //
    //std::cout << entity.get<Force>().x << std::endl;
    //entity5.remove<Force>();
    
    //crude_engine::Query<Force, Speed> q(&ecs);
    //q.each([](crude_engine::Entity e, Force& f, Speed& s) {
    //  f.x += s.x;
    //  std::cout << ": {" << f.x << ", " << s.x << "}\n";
    //});
  }
  return EXIT_SUCCESS;
}

#endif