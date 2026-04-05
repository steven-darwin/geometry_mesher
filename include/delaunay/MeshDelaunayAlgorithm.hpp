/**
 * @file MeshDelaunayAlgorithm.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Concrete class for Delaunay algorithm.
 */

#ifndef MESH_DELAUNAY_ALGORITHM_HPP
#define MESH_DELAUNAY_ALGORITHM_HPP

#include <memory>

#include "core/MeshAlgorithm.hpp"
#include "core/MeshInputData.hpp"
#include "core/MeshOutputData.hpp"

class MeshDelaunayAlgorithm : public MeshAlgorithm {
public:
    /** Constructor of MeshDelaunayAlgorithm object
     */
    MeshDelaunayAlgorithm(std::shared_ptr<MeshInputData> input_data, std::shared_ptr<MeshOutputData> output_data);

    /** Destructor of MeshDelaunayAlgorithm object */
    ~MeshDelaunayAlgorithm();

    /** Method to run mesh algorithm */
    void run(unsigned int step_count);

private:

};

#endif