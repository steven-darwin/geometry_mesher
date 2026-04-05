#include <memory>
#include <iostream>
#include <unordered_set>

#include "general/Generic.hpp"

#include "core/MeshCore.hpp"

int main()
{
	MeshCore mesh_program;
	mesh_program.setInputProcessor(scmp::STEP, "../../asset/STEP/slab.STEP");
	mesh_program.setOutputProcessor(scmp::XDMF, "../staging/slab_mesh.xmf");
	mesh_program.run(scmp::mesh::TRANSFINITE_INTERPOLATION);

	return 0;
}
