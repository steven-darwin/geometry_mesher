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

#include "general/Generic.hpp"
#include "utility/ConfigReader.hpp"

#include "input-output/OutputAdapter.hpp"
#include "input-output/OutputXDMFAdapter.hpp"
#include "input-output/OutputHDF5Adapter.hpp"

#include "core/MeshOutputProcessor.hpp"

MeshOutputProcessor::MeshOutputProcessor() {
    // TBA
}

MeshOutputProcessor::~MeshOutputProcessor() {
    // TBA
}

void MeshOutputProcessor::setOutputAdapterInfo(const char* runtime_config_file_path) {
    ConfigReader config_reader;

    if (config_reader.getRuntimeConfigValue(runtime_config_file_path, "mesh", "output_file_extension") == "xdmf & hdf5") {
        _outputAdapterInfo._adapterObj = std::make_shared<OutputXDMFAdapter>(runtime_config_file_path, "original.mesh");
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
}

void MeshOutputProcessor::getMeshOutputData(std::shared_ptr<MeshOutputData> output_data) {
    _outputAdapterInfo._neutralGeometryTopology = output_data->transform();
}