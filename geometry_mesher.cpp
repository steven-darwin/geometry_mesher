#include <memory>
#include <iostream>
#include <unordered_set>

#include "general/Generic.hpp"

#include "core/MeshCore.hpp"

int main()
{
	MeshCore mesh_program;
	mesh_program.setInputProcessor(scmp::STEP, "slab");
	mesh_program.setOutputProcessor(scmp::XDMF, "slab");
	mesh_program.run(scmp::mesh::TRANSFINITE_INTERPOLATION);

	return 0;
}
