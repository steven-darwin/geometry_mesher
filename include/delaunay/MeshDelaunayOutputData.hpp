/**
 * @file MeshDelaunayOutputData.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Concrete class to handle Delaunay algorithm output data.
 */

#ifndef MESH_DELAUNAY_OUTPUT_DATA_HPP
#define MESH_DELAUNAY_OUTPUT_DATA_HPP

#include "core/MeshOutputData.hpp"

class MeshDelaunayOutputData : public MeshOutputData {
public:
    /** Constructor of MeshDelaunayOutputData object
     */
    MeshDelaunayOutputData();

    /** Destructor of MeshDelaunayOutputData object */
    ~MeshDelaunayOutputData();

    /** Method to transform mesh strategy-specific geometry-topology data structure to neutral geometry-topology data structure */
    std::shared_ptr<GeometryTopology> transform();

private:

};

#endif