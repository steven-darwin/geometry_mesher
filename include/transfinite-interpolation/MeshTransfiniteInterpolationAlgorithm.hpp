/**
 * @file MeshTranfiniteInterpolationAlgorithm.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Concrete class for Transfinite Interpolation algorithm.
 */

#ifndef MESH_TRANSFINITE_INTERPOLATION_ALGORITHM_HPP
#define MESH_TRANSFINITE_INTERPOLATION_ALGORITHM_HPP

#include <memory>
#include <vector>
#include <array>
#include <optional>

#include "core/MeshAlgorithm.hpp"
#include "core/MeshInputData.hpp"
#include "core/MeshOutputData.hpp"


class MeshTransfiniteInterpolationAlgorithm : public MeshAlgorithm {
public:
    /** Constructor of MeshTransfiniteInterpolationAlgorithm object
     */
    MeshTransfiniteInterpolationAlgorithm(std::shared_ptr<MeshInputData> input_data, std::shared_ptr<MeshOutputData> output_data);

    /** Destructor of MeshTransfiniteInterpolationAlgorithm object */
    ~MeshTransfiniteInterpolationAlgorithm();

    /** Method to run mesh algorithm */
    void run(unsigned int step_count);

private:
    /** Method to map 3D logical coordinate into real coordinate */
    std::array<double, 3> map3DLogicalToRealCoordinate(std::vector<std::array<double, 3>> real_vertex_coordinate_list, std::optional<double> x1, std::optional<double> x2, std::optional<double> x3);

    /** Method to map 2D logical coordinate into real coordinate */
    std::array<double, 3> map2DLogicalToRealCoordinate(std::vector<std::array<double, 3>> real_vertex_coordinate_list, std::optional<double> x1, std::optional<double> x2, std::optional<double> x3);

    /** Method to map 1D logical coordinate into real coordinate */
    std::array<double, 3> map1DLogicalToRealCoordinate(std::vector<std::array<double, 3>> real_vertex_coordinate_list, std::optional<double> x1, std::optional<double> x2, std::optional<double> x3);

};

#endif