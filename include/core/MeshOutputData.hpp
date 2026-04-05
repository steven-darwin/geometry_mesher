/**
 * @file MeshOutputData.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Abstract class to handle mesh algorithm output data.
 */

#ifndef MESH_OUTPUT_DATA_HPP
#define MESH_OUTPUT_DATA_HPP

#include <memory>

#include "geometry-topology/GeometryTopology.hpp"

class MeshOutputData {
public:
    /** Constructor of MeshOutputData object
     */
    MeshOutputData();

    /** Destructor of MeshOutputData object */
    ~MeshOutputData();

    /** Method to transform mesh strategy-specific geometry-topology data structure to neutral geometry-topology data structure */
    virtual std::shared_ptr<GeometryTopology> transform() = 0;
private:
    
};

#endif