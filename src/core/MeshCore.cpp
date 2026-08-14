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

#include "general/Generic.hpp"
#include "utility/ConfigReader.hpp"

#include "core/MeshCore.hpp"

#include "transfinite-interpolation/MeshTransfiniteInterpolationInputData.hpp"
#include "transfinite-interpolation/MeshTransfiniteInterpolationAlgorithm.hpp"
#include "transfinite-interpolation/MeshTransfiniteInterpolationOutputData.hpp"

#include "delaunay/MeshDelaunayInputData.hpp"
#include "delaunay/MeshDelaunayAlgorithm.hpp"
#include "delaunay/MeshDelaunayOutputData.hpp"

MeshCore::MeshCore() {
    // TBA
}

MeshCore::MeshCore(const char* runtime_config_file_path) {
    _runtimeConfigFilePath = runtime_config_file_path;
}

MeshCore::~MeshCore() {
    // TBA
}

void MeshCore::setInputProcessor() {
    _inputProcessor.setInputAdapterInfo(_runtimeConfigFilePath);
}

void MeshCore::setOutputProcessor() {
    _outputProcessor.setOutputAdapterInfo(_runtimeConfigFilePath);
}

void MeshCore::run() {
    ConfigReader config_reader;

    if (config_reader.getRuntimeConfigValue(_runtimeConfigFilePath, "mesh", "strategy") == "transfinite_interpolation") {
        _inputData = std::make_shared<MeshTransfiniteInterpolationInputData>();
        _outputData = std::make_shared<MeshTransfiniteInterpolationOutputData>();

        _inputProcessor.runInputAdapter();
        _inputProcessor.setMeshInputData(_inputData);

        _algorithm = std::make_shared<MeshTransfiniteInterpolationAlgorithm>(_inputData, _outputData);
        _algorithm->run(std::stoi(config_reader.getRuntimeConfigValue(_runtimeConfigFilePath, "mesh", "transfinite_interpolation_segment_count")));

        _outputProcessor.getMeshOutputData(_outputData);
        _outputProcessor.runOutputAdapter();
    } else {
        // TBA
    }
}