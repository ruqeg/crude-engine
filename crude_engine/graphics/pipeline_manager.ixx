module;

export module crude.gfx.pipeline_manager;

export import crude.core.std_containers_heap;

export namespace crude::gfx::vk
{

class Device;
class Pipeline;
class Render_Pass;

}

export namespace crude::gfx
{

class Pipeline_Manager
{
public:
  struct Pipeline_Info;
public:
  void addPipeline(const Pipeline_Info& info);
  core::shared_ptr<vk::Pipeline> getPipeline(const core::string& name);
private:
  core::unordered_map<core::string, core::shared_ptr<vk::Pipeline>> m_pipeline;
};

struct Pipeline_Manager::Pipeline_Info
{

};

void Pipeline_Manager::addPipeline(const Pipeline_Info& info)
{
}

}