/**
 * @file MeshOutputProcessor.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Class to process mesh output.
 */

#ifndef MESH_OUTPUT_PROCESSOR_HPP
#define MESH_OUTPUT_PROCESSOR_HPP

#include "geometry-topology/GeometryTopology.hpp"
#include "geometry-topology/GeometryTopologyVertex.hpp"
#include "geometry-topology/GeometryTopologyEdge.hpp"
#include "geometry-topology/GeometryTopologyWire.hpp"
#include "geometry-topology/GeometryTopologyFace.hpp"
#include "geometry-topology/GeometryTopologyShell.hpp"
#include "geometry-topology/GeometryTopologyCell.hpp"

#include "input-output/OutputXDMFAdapter.hpp"
#include "input-output/OutputHDF5Adapter.hpp"

#include "general/Generic.hpp"

#include "core/MeshOutputData.hpp"

class MeshOutputProcessor {
public:
    /** Constructor of MeshOutputProcessor object
     */
    MeshOutputProcessor();

    /** Destructor of MeshOutputProcessor object */
    ~MeshOutputProcessor();

    /** Method to set output adapter info */
    void setOutputAdapterInfo(scmp::FileExtension file_extension, const char* file_path);

    /** Method to run output adapter */
    void runOutputAdapter();

    /** Method to get mesh output data */
    void getMeshOutputData(std::shared_ptr<MeshOutputData> output_data);

private:
    /** Attribute to store output adapter metadata */
    scmp::AdapterInfo _outputAdapterInfo;
};

#endif