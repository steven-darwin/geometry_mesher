/**
 * @file MeshOutputProcessor.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Implementation] Class to process mesh output.
 */

#include <memory>
#include <unordered_map>

#include "general/Generic.hpp"
#include "utility/ConfigReader.hpp"

#include "input-output/OutputAdapter.hpp"
#include "input-output/OutputXDMFAdapter.hpp"
#include "input-output/OutputHDF5Adapter.hpp"

#include "core/MeshOutputProcessor.hpp"
#include "report/MeshReport.hpp"

void MeshOutputProcessor::setOutputAdapterInfo() {
    if (ConfigReader::instance().getRuntimeConfigValue("mesh", "output_file_extension") == "xdmf & hdf5") {
        _outputAdapterInfo._adapterObj = std::make_shared<OutputXDMFAdapter>("original.mesh");
        OutputXDMFAdapter::ParameterMetadata computational_grid_parameter = {
            "computational_grid",
            {1, 3},
            GeometryTopology::Type::VERTEX
        };
        std::dynamic_pointer_cast<OutputXDMFAdapter>(_outputAdapterInfo._adapterObj)->addSolverParameter({ computational_grid_parameter });
    }
}

void MeshOutputProcessor::runOutputAdapter() {
    std::dynamic_pointer_cast<OutputAdapter>(_outputAdapterInfo._adapterObj)->serialize(_outputAdapterInfo._neutralGeometryTopology);
    MeshReport::instance().addFileSuffix("out", "original.mesh", "xmf");
    MeshReport::instance().addFileSuffix("out", "original.mesh", "h5");
    MeshReport::instance().addTimePoint("original_mesh_out", std::chrono::system_clock::now());
}

void MeshOutputProcessor::getMeshOutputData(std::shared_ptr<MeshOutputData> output_data) {
    _outputAdapterInfo._neutralGeometryTopology = output_data->transform();

    std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> entity_list;

    entity_list.clear();
    _outputAdapterInfo._neutralGeometryTopology->getDescendants(entity_list, GeometryTopology::Type::VERTEX);
    MeshReport::instance().getWholeMeshSummary().entity_count.insert({ GeometryTopology::Type::VERTEX, entity_list.size() });

    entity_list.clear();
    _outputAdapterInfo._neutralGeometryTopology->getDescendants(entity_list, GeometryTopology::Type::EDGE);
    MeshReport::instance().getWholeMeshSummary().entity_count.insert({ GeometryTopology::Type::EDGE, entity_list.size() });

    entity_list.clear();
    _outputAdapterInfo._neutralGeometryTopology->getDescendants(entity_list, GeometryTopology::Type::FACE);
    MeshReport::instance().getWholeMeshSummary().entity_count.insert({ GeometryTopology::Type::FACE, entity_list.size() });

    entity_list.clear();
    _outputAdapterInfo._neutralGeometryTopology->getDescendants(entity_list, GeometryTopology::Type:: CELL);
    MeshReport::instance().getWholeMeshSummary().entity_count.insert({ GeometryTopology::Type:: CELL, entity_list.size() });

}