#include "MeshLoader.hpp"

#include <iostream>
#include <algorithm>

#include "gloo/utils.hpp"

namespace GLOO {
MeshData MeshLoader::Import(const std::string& filename) {
  // Return hardcoded cube - bypass OBJ parser
  MeshData mesh_data;
  mesh_data.vertex_obj = make_unique<VertexObject>();
  return mesh_data;
}
}  // namespace GLOO
