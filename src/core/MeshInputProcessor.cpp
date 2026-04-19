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

void MeshInputProcessor::setInputAdapterInfo(scmp::FileExtension file_extension, std::string file_name) {
    _inputAdapterInfo._fileExtension = file_extension;
    _inputAdapterInfo._filePath = file_name;

    switch (file_extension) {
    case scmp::STEP:
        _inputAdapterInfo._adapterObj = std::make_shared<InputSTEPAdapter>(file_name);
        break;
    case scmp::XDMF:
        _inputAdapterInfo._adapterObj = std::make_shared<InputXDMFAdapter>(file_name, file_name);
        break;
    case scmp::HDF5:
        _inputAdapterInfo._adapterObj = std::make_shared<InputHDF5Adapter>(file_name);
        break;
    default:
        // To Do: throw unrecogized file_extension error
        break;
    }
}

void MeshInputProcessor::runInputAdapter() {
    _inputAdapterInfo._neutralGeometryTopology = std::dynamic_pointer_cast<InputAdapter>(_inputAdapterInfo._adapterObj)->deserialize();
}

void MeshInputProcessor::setMeshInputData(std::shared_ptr<MeshInputData> input_data) {
    input_data->transform(_inputAdapterInfo._neutralGeometryTopology);
}