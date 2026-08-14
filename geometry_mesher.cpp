#include <memory>
#include <iostream>
#include <unordered_set>
#include <random>
#include <ctime>

#include "general/Generic.hpp"

#include "core/MeshCore.hpp"
#include "zoning/MeshZoning.hpp"

int main(int argc, char* argv[])
{
	MeshCore original_mesh_program(argv[1]);
	original_mesh_program.setInputProcessor();
	original_mesh_program.setOutputProcessor();
	original_mesh_program.run();

	MeshZoning extended_mesh_program(argv[1]);
	extended_mesh_program.setupPhase();
	extended_mesh_program.executionPhase();

	return 0;
}
