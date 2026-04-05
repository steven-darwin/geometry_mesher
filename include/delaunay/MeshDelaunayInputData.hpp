/**
 * @file MeshDelaunayInputData.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Concrete class to handle Delaunay algorithm input data.
 */

#ifndef MESH_DELAUNAY_INPUT_DATA_HPP
#define MESH_DELAUNAY_INPUT_DATA_HPP

#include "core/MeshInputData.hpp"

class MeshDelaunayInputData : public MeshInputData {
public:
    /** Constructor of MeshDelaunayInputData object
     */
    MeshDelaunayInputData();

    /** Destructor of MeshDelaunayInputData object */
    ~MeshDelaunayInputData();

    /** Method to transform neutral geometry-topology data structure to mesh strategy-specific geometry-topology data structure */
    void transform(std::shared_ptr<GeometryTopology> neutral_geometry_topology);

private:

};

#endif