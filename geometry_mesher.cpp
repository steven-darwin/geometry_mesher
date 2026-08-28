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
#include "zoning/MeshZone.hpp"
#include "report/MeshReport.hpp"

int main(int argc, char* argv[])
{
	ConfigReader::instance().setRuntimeConfigFilePath(argv[1]);

	MeshReport::instance().addTimePoint("start_execution", std::chrono::system_clock::now());

	MeshCore::instance().setup();
	MeshCore::instance().run();

	MeshZone::instance().setup();
	MeshZone::instance().run();
	
	MeshReport::instance().addTimePoint("end_execution", std::chrono::system_clock::now());

	MeshReport::instance().exportData();

	return 0;
}
