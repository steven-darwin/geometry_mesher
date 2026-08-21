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

#include "general/Generic.hpp"
#include "utility/ConfigReader.hpp"

#include "core/MeshCore.hpp"
#include "report/MeshReport.hpp"

#include "transfinite-interpolation/MeshTransfiniteInterpolationInputData.hpp"
#include "transfinite-interpolation/MeshTransfiniteInterpolationAlgorithm.hpp"
#include "transfinite-interpolation/MeshTransfiniteInterpolationOutputData.hpp"

#include "delaunay/MeshDelaunayInputData.hpp"
#include "delaunay/MeshDelaunayAlgorithm.hpp"
#include "delaunay/MeshDelaunayOutputData.hpp"

void MeshCore::setInputProcessor() {
    _inputProcessor.setInputAdapterInfo();
}

void MeshCore::setOutputProcessor() {
    _outputProcessor.setOutputAdapterInfo();
}

void MeshCore::run() {
    if (ConfigReader::instance().getRuntimeConfigValue("mesh", "strategy") == "transfinite_interpolation") {
        _inputData = std::make_shared<MeshTransfiniteInterpolationInputData>();
        _outputData = std::make_shared<MeshTransfiniteInterpolationOutputData>();

        _inputProcessor.runInputAdapter();

        _inputProcessor.setMeshInputData(_inputData);
        std::cout << "\r" << "Executing Meshing Algorithm (transfinite-interpolation) => 10%";

        _algorithm = std::make_shared<MeshTransfiniteInterpolationAlgorithm>(_inputData, _outputData);
        _algorithm->run(std::stoi(ConfigReader::instance().getRuntimeConfigValue("mesh", "transfinite_interpolation_segment_count")));
        std::cout << "\r" << "Executing Meshing Algorithm (transfinite-interpolation) ===> 30%";

        MeshReport::instance().addTimePoint("mesh_completion", std::chrono::system_clock::now());

        _outputProcessor.getMeshOutputData(_outputData);
        std::cout << "\r" << "Executing Meshing Algorithm (transfinite-interpolation) ==========> 100%" << std::endl;

        _outputProcessor.runOutputAdapter();
    } else {
        // TBA
    }
}