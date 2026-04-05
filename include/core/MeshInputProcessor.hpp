/**
 * @file MeshInputProcessor.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Class to process mesh input.
 */

#ifndef MESH_INPUT_PROCESSOR_HPP
#define MESH_INPUT_PROCESSOR_HPP

#include <vector>
#include <memory>

#include "geometry-topology/GeometryTopologyVertex.hpp"
#include "geometry-topology/GeometryTopologyEdge.hpp"
#include "geometry-topology/GeometryTopologyWire.hpp"
#include "geometry-topology/GeometryTopologyFace.hpp"
#include "geometry-topology/GeometryTopologyShell.hpp"
#include "geometry-topology/GeometryTopologyCell.hpp"

#include "input-output/InputSTEPAdapter.hpp"

#include "general/Generic.hpp"

#include "core/MeshInputData.hpp"

class MeshInputProcessor {
public:
    /** Constructor of MeshInputProcessor object
     */
    MeshInputProcessor();

    /** Destructor of MeshInputProcessor object */
    ~MeshInputProcessor();

    /** Method to set input adapter info */
    void setInputAdapterInfo(scmp::FileExtension file_extension, const char* file_path);

    /** Method to run input adapter */
    void runInputAdapter();

    /** Method to set mesh input data */
    void setMeshInputData(std::shared_ptr<MeshInputData> input_data);

private:
    /** Attribute to store input adapter metadata */
    scmp::AdapterInfo _inputAdapterInfo;
};

#endif