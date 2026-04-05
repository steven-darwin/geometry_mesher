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
    MeshCore();

    /** Destructor of MeshCore object */
    ~MeshCore();

    /** Method to set input file and input adapter obj */
    void setInputProcessor(scmp::FileExtension input_file_extension, const char* input_file_path);

    /** Method to set output file and output adapter obj */
    void setOutputProcessor(scmp::FileExtension output_file_extension, const char* output_file_path);

    /** Method to set mesh strategy */
    void run(scmp::mesh::MeshStrategy strategy);

private:
    MeshInputProcessor _inputProcessor;
    MeshOutputProcessor _outputProcessor;

    std::shared_ptr<MeshInputData> _inputData;
    std::shared_ptr<MeshAlgorithm> _algorithm;
    std::shared_ptr<MeshOutputData> _outputData;
};

#endif