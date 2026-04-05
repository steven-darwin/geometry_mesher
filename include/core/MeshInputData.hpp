/**
 * @file MeshInputData.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Abstract class to handle mesh algorithm input data.
 */

#ifndef MESH_INPUT_DATA_HPP
#define MESH_INPUT_DATA_HPP

#include <memory>

#include "geometry-topology/GeometryTopology.hpp"

class MeshInputData {
public:
    /** Constructor of MeshInputData object
     */
    MeshInputData();

    /** Destructor of MeshInputData object */
    ~MeshInputData();

    /** Method to transform neutral geometry-topology data structure to mesh strategy-specific geometry-topology data structure */
    virtual void transform(std::shared_ptr<GeometryTopology>) = 0;
private:

};

#endif