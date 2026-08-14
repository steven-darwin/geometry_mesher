/**
 * @file MeshInputProcessor.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Implementation] Class to process mesh input.
 */

#include <memory>

#include "general/Generic.hpp"

#include "input-output/InputAdapter.hpp"
#include "input-output/InputSTEPAdapter.hpp"
#include "input-output/InputXDMFAdapter.hpp"
#include "input-output/InputHDF5Adapter.hpp"

#include "core/MeshInputProcessor.hpp"


MeshInputProcessor::MeshInputProcessor() {
    // TBA
}

MeshInputProcessor::~MeshInputProcessor() {
    // TBA
}

void MeshInputProcessor::setInputAdapterInfo(const char* runtime_config_file_path) {
    ConfigReader config_reader;

    if (config_reader.getRuntimeConfigValue(runtime_config_file_path, "mesh", "input_file_extension") == "step") {
        _inputAdapterInfo._adapterObj = std::make_shared<InputSTEPAdapter>(runtime_config_file_path);
    }
}

void MeshInputProcessor::runInputAdapter() {
    _inputAdapterInfo._neutralGeometryTopology = std::dynamic_pointer_cast<InputAdapter>(_inputAdapterInfo._adapterObj)->deserialize()[0];
}

void MeshInputProcessor::setMeshInputData(std::shared_ptr<MeshInputData> input_data) {
    input_data->transform(_inputAdapterInfo._neutralGeometryTopology);
}