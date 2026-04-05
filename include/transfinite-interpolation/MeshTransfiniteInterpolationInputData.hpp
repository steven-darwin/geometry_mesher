/**
 * @file MeshTransfiniteInterpolationInputData.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Concrete class to handle Transfinite Interpolation algorithm input data.
 */

#ifndef MESH_TRANSFINITE_INTERPOLATION_INPUT_DATA_HPP
#define MESH_TRANSFINITE_INTERPOLATION_INPUT_DATA_HPP

#include <vector>
#include <memory>

#include "geometry-topology/GeometryTopology.hpp"

#include "core/MeshInputData.hpp"

#include "data-structure/MeshGraphEdge.hpp"

class MeshTransfiniteInterpolationInputData : public MeshInputData {
public:
    /** Constructor of MeshTransfiniteInterpolationInputData object
     */
    MeshTransfiniteInterpolationInputData();

    /** Destructor of MeshTransfiniteInterpolationInputData object */
    ~MeshTransfiniteInterpolationInputData();

    /** Method to transform neutral geometry-topology data structure to mesh strategy-specific geometry-topology data structure */
    void transform(std::shared_ptr<GeometryTopology> neutral_geometry_topology);

    /** Accessor to mesh graph edge list begin iterator */ 
    std::vector<std::shared_ptr<MeshGraphEdge>>::iterator getMeshGraphEdgeListBeginIterator();

    /** Accessor to mesh graph edge list end iterator */
    std::vector<std::shared_ptr<MeshGraphEdge>>::iterator getMeshGraphEdgeListEndIterator();

private:
    std::vector<std::shared_ptr<MeshGraphEdge>> _meshGraphEdgeList;
};

#endif