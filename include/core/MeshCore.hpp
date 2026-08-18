/**
 * @file MeshCore.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Mediator class for mesh core process.
 */

#ifndef MESH_CORE_HPP
#define MESH_CORE_HPP

#include <memory>

#include <general/Generic.hpp>

#include "core/MeshInputProcessor.hpp"
#include "core/MeshOutputProcessor.hpp"

#include "core/MeshInputData.hpp"
#include "core/MeshAlgorithm.hpp"
#include "core/MeshOutputData.hpp"

class MeshCore {
public:
    /** Constructor of MeshCore object
     */
    MeshCore() = default;

    /** Destructor of MeshCore object */
    ~MeshCore() = default;

    /** Method to set input file and input adapter obj */
    void setInputProcessor();

    /** Method to set output file and output adapter obj */
    void setOutputProcessor();

    /** Method to set mesh strategy */
    void run();

private:
    MeshInputProcessor _inputProcessor;
    MeshOutputProcessor _outputProcessor;

    std::shared_ptr<MeshInputData> _inputData;
    std::shared_ptr<MeshAlgorithm> _algorithm;
    std::shared_ptr<MeshOutputData> _outputData;
};

#endif