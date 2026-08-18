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
#include <chrono>

#include "general/Generic.hpp"

#include "input-output/InputAdapter.hpp"
#include "input-output/InputSTEPAdapter.hpp"
#include "input-output/InputXDMFAdapter.hpp"
#include "input-output/InputHDF5Adapter.hpp"

#include "core/MeshInputProcessor.hpp"
#include "report/MeshReport.hpp"

void MeshInputProcessor::setInputAdapterInfo() {
    if (ConfigReader::instance().getRuntimeConfigValue("mesh", "input_file_extension") == "step") {
        _inputAdapterInfo._adapterObj = std::make_shared<InputSTEPAdapter>();
    }
}

void MeshInputProcessor::runInputAdapter() {
    _inputAdapterInfo._neutralGeometryTopology = std::dynamic_pointer_cast<InputAdapter>(_inputAdapterInfo._adapterObj)->deserialize()[0];
    MeshReport::instance().addFileSuffix("in", "", "STEP");
    MeshReport::instance().addTimePoint("step_in", std::chrono::system_clock::now());
}

void MeshInputProcessor::setMeshInputData(std::shared_ptr<MeshInputData> input_data) {
    input_data->transform(_inputAdapterInfo._neutralGeometryTopology);
}