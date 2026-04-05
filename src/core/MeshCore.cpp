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

MeshCore::~MeshCore() {
    // TBA
}

void MeshCore::setInputProcessor(scmp::FileExtension input_file_extension, const char* input_file_path) {
    _inputProcessor.setInputAdapterInfo(input_file_extension, input_file_path);
}

void MeshCore::setOutputProcessor(scmp::FileExtension output_file_extension, const char* output_file_path) {
    _outputProcessor.setOutputAdapterInfo(output_file_extension, output_file_path);
}

void MeshCore::run(scmp::mesh::MeshStrategy strategy) {
    switch (strategy) {
    case scmp::mesh::TRANSFINITE_INTERPOLATION:
        _inputData = std::make_shared<MeshTransfiniteInterpolationInputData>();
        _outputData = std::make_shared<MeshTransfiniteInterpolationOutputData>();

        _inputProcessor.runInputAdapter();
        _inputProcessor.setMeshInputData(_inputData);

        _algorithm = std::make_shared<MeshTransfiniteInterpolationAlgorithm>(_inputData, _outputData);
        _algorithm->run(2);

        _outputProcessor.getMeshOutputData(_outputData);
        _outputProcessor.runOutputAdapter();

        break;
    case scmp::mesh::DELAUNAY:
        _inputData = std::make_shared<MeshDelaunayInputData>();
        _outputData = std::make_shared<MeshDelaunayOutputData>();

        _inputProcessor.runInputAdapter();
        _inputProcessor.setMeshInputData(_inputData);

        _algorithm = std::make_shared<MeshDelaunayAlgorithm>(_inputData, _outputData);
        _algorithm->run(4);

        _outputProcessor.getMeshOutputData(_outputData);
        _outputProcessor.runOutputAdapter();

        break;
    default:
        // To Do: throw unrecognized_strategy error
        break;
    }
}