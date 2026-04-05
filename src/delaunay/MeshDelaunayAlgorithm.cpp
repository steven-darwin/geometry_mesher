/**
 * @file MeshDelaunayAlgorithm.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Implementation] Concrete class for Delaunay algorithm.
 */

#include "delaunay/MeshDelaunayAlgorithm.hpp"

MeshDelaunayAlgorithm::MeshDelaunayAlgorithm(std::shared_ptr<MeshInputData> input_data, std::shared_ptr<MeshOutputData> output_data) {
    _inputData = input_data;
    _outputData = output_data;
}

MeshDelaunayAlgorithm::~MeshDelaunayAlgorithm() {
    // TBA
}

void MeshDelaunayAlgorithm::run(unsigned int step_count) {
    // TBA
}