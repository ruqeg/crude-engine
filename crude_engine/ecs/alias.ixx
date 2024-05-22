module;

export module crude.ecs.alias;

export import crude.core.alias;

export namespace crude::ecs
{

using ID            = core::uint64;
using ID_Generation = core::uint16;
using ID_Index      = core::uint64;

using Component_ID  = ID;
using Archetype_ID  = ID;
using Entity_ID     = ID;

}