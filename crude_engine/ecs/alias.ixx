module;

export module crude_engine.ecs.alias;

export import crude_engine.core.alias;

export namespace crude_engine
{

using ID            = uint64;
using ID_Generation = uint16;
using ID_Index      = uint64;

using Component_ID  = ID;
using Archetype_ID  = ID;
using Entity_ID     = ID;

}