/**
 * @file MeshCore.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Implementation] Mediator class for mesh core process.
 */

#include <memory>
#include <chrono>
#include <iostream>

#include "general/Generic.hpp"
#include "input-output/InputSTEPAdapter.hpp"
#include "input-output/OutputXDMFAdapter.hpp"
#include "utility/ConfigReader.hpp"

#include "core/MeshCore.hpp"
#include "report/MeshReport.hpp"

#include "transfinite-interpolation/MeshTransfiniteInterpolationInputData.hpp"
#include "transfinite-interpolation/MeshTransfiniteInterpolationAlgorithm.hpp"
#include "transfinite-interpolation/MeshTransfiniteInterpolationOutputData.hpp"

MeshCore& MeshCore::instance() {
    static MeshCore singleton;
    return singleton;
}

void MeshCore::setup() {
    if (ConfigReader::instance().getRuntimeConfigValue("mesh", "input_file_extension") == "step") {
        _inputAdapter = std::make_shared<InputSTEPAdapter>();
    }

    if (ConfigReader::instance().getRuntimeConfigValue("mesh", "output_file_extension") == "xdmf & hdf5") {
        _outputAdapter = std::make_shared<OutputXDMFAdapter>();

        OutputXDMFAdapter::ParameterMetadata computational_grid_parameter = {
            "computational_grid",
            {1, 3},
            GeometryTopology::Type::VERTEX
        };

        std::dynamic_pointer_cast<OutputXDMFAdapter>(_outputAdapter)->addSolverParameter({ computational_grid_parameter });
    }
}

void MeshCore::run() {
    if (ConfigReader::instance().getRuntimeConfigValue("mesh", "strategy") == "transfinite_interpolation") {
        std::shared_ptr<GeometryTopology> input_neutral_geometry_topology = _inputAdapter->deserialize("")[0];

        std::shared_ptr<MeshInputData> input_data = std::make_shared<MeshTransfiniteInterpolationInputData>();
        std::shared_ptr<MeshOutputData> output_data = std::make_shared<MeshTransfiniteInterpolationOutputData>();

        std::shared_ptr<MeshAlgorithm> algorithm = std::make_shared<MeshTransfiniteInterpolationAlgorithm>(
            std::stoi(ConfigReader::instance().getRuntimeConfigValue("mesh", "transfinite_interpolation_segment_count"))
        );
        algorithm->setup(input_data, output_data);

        input_data->transform(input_neutral_geometry_topology);

        if (ConfigReader::instance().getRuntimeConfigValue("mesh", "input_file_extension") == "step") {
            MeshReport::instance().addFileSuffix("in", "", "STEP");
        }

        MeshReport::instance().addTimePoint("step_in", std::chrono::system_clock::now());

        std::cout << "\r" << "Executing Meshing Algorithm (transfinite-interpolation) => 10%";

        algorithm->run();
        std::cout << "\r" << "Executing Meshing Algorithm (transfinite-interpolation) ===> 30%";

        MeshReport::instance().addTimePoint("mesh_completion", std::chrono::system_clock::now());

        std::shared_ptr<GeometryTopology> output_neutral_geometry_topology = output_data->transform();

        std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> entity_list;

        entity_list.clear();
        output_neutral_geometry_topology->getDescendants(entity_list, GeometryTopology::Type::VERTEX);
        MeshReport::instance().getWholeMeshSummary().entity_count.insert({ GeometryTopology::Type::VERTEX, entity_list.size() });

        entity_list.clear();
        output_neutral_geometry_topology->getDescendants(entity_list, GeometryTopology::Type::EDGE);
        MeshReport::instance().getWholeMeshSummary().entity_count.insert({ GeometryTopology::Type::EDGE, entity_list.size() });

        entity_list.clear();
        output_neutral_geometry_topology->getDescendants(entity_list, GeometryTopology::Type::FACE);
        MeshReport::instance().getWholeMeshSummary().entity_count.insert({ GeometryTopology::Type::FACE, entity_list.size() });

        entity_list.clear();
        output_neutral_geometry_topology->getDescendants(entity_list, GeometryTopology::Type::CELL);
        MeshReport::instance().getWholeMeshSummary().entity_count.insert({ GeometryTopology::Type::CELL, entity_list.size() });

        std::cout << "\r" << "Executing Meshing Algorithm (transfinite-interpolation) ==========> 100%" << std::endl;

        _outputAdapter->serialize(output_neutral_geometry_topology, "original.mesh");

        if (ConfigReader::instance().getRuntimeConfigValue("mesh", "input_file_extension") == "xdmf & hdf5") {
            MeshReport::instance().addFileSuffix("out", "original.mesh", "xmf");
            MeshReport::instance().addFileSuffix("out", "original.mesh", "h5");
        }

        MeshReport::instance().addTimePoint("original_mesh_out", std::chrono::system_clock::now());
    }
}

std::shared_ptr<InputOutputAdapter> MeshCore::getIOAdapter(std::string type) {
    if (type == "input") return _inputAdapter;
    if (type == "output") return _outputAdapter;
}