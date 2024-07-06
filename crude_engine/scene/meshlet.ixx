#pragma once

export module crude.scene.meshlet;

export import crude.core.alias;

export namespace crude::scene
{

struct Meshlet
{
  core::uint32 vertexCount;
  core::uint32 vertexOffset;
  core::uint32 primitiveCount;
  core::uint32 primitiveOffest;
};

}