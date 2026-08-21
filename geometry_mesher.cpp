#include <memory>
#include <iostream>
#include <unordered_set>
#include <random>
#include <ctime>
#include <sstream>
#include <chrono>
#include <thread>

#include "general/Generic.hpp"
#include "utility/ConfigReader.hpp"

#include "core/MeshCore.hpp"
#include "zoning/MeshZoning.hpp"
#include "report/MeshReport.hpp"

int main(int argc, char* argv[])
{
	ConfigReader::instance().setRuntimeConfigFilePath(argv[1]);

	MeshReport::instance().addTimePoint("start_execution", std::chrono::system_clock::now());

	MeshCore original_mesh_program;
	original_mesh_program.setInputProcessor();
	original_mesh_program.setOutputProcessor();
	original_mesh_program.run();

	MeshZoning extended_mesh_program;
	extended_mesh_program.setupPhase();
	extended_mesh_program.executionPhase();
	
	MeshReport::instance().addTimePoint("end_execution", std::chrono::system_clock::now());

	MeshReport::instance().exportData();

	return 0;
}
