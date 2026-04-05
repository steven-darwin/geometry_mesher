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

void MeshOutputProcessor::setOutputAdapterInfo(scmp::FileExtension file_extension, const char* file_path) {
    _outputAdapterInfo._fileExtension = file_extension;
    _outputAdapterInfo._filePath = file_path;

    switch (file_extension) {
    case scmp::XDMF:
        _outputAdapterInfo._adapterObj = std::make_shared<OutputXDMFAdapter>(file_path);
        break;
    case scmp::HDF5:
        _outputAdapterInfo._adapterObj = std::make_shared<OutputHDF5Adapter>(file_path);
        break;
    default:
        // To Do: throw unrecogized file_extension error
        break;
    }
}

void MeshOutputProcessor::runOutputAdapter() {
    std::dynamic_pointer_cast<OutputAdapter>(_outputAdapterInfo._adapterObj)->serialize(_outputAdapterInfo._neutralGeometryTopology);

}

void MeshOutputProcessor::getMeshOutputData(std::shared_ptr<MeshOutputData> output_data) {
    _outputAdapterInfo._neutralGeometryTopology = output_data->transform();
}