/**
 * @file MeshAlgorithm.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Abstract class for mesh algorithm.
 */

#ifndef MESH_ALGORITHM_HPP
#define MESH_ALGORITHM_HPP

#include <memory>

#include "core/MeshInputData.hpp"
#include "core/MeshOutputData.hpp"

class MeshAlgorithm {
public:
    /** Constructor of MeshAlgorithm object
     */
    MeshAlgorithm();

    /** Destructor of MeshAlgorithm object */
    ~MeshAlgorithm();

    /** Method to run mesh algorithm */
    virtual void run(unsigned int step_count) = 0;

protected:
    std::shared_ptr<MeshInputData> _inputData;
    std::shared_ptr<MeshOutputData> _outputData;
    
};

#endif