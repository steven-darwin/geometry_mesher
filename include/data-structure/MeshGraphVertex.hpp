/**
 * @file MeshGraphVertex.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-09-28
 * @date Last Modified : 2025-09-28
 *
 * @brief [Header] Class for representing vertex on mesh's graph data structure.
 */

#ifndef MESH_GRAPH_VERTEX_HPP
#define MESH_GRAPH_VERTEX_HPP

#include <vector>
#include <memory>

#include "geometry-topology/GeometryTopologyVertex.hpp"

class MeshGraphEdge;

class MeshGraphVertex {
public:
    /** Constructor of MeshGraphVertex object
     */
    MeshGraphVertex(std::shared_ptr<GeometryTopology> geometry_topology);

    /** Destructor of MeshGraphVertex object */
    ~MeshGraphVertex();

    /** Method to add graph edge which is connected to this graph vertex */
    void addConnectedEdge(std::shared_ptr<MeshGraphEdge> mesh_graph_edge);

    /** Accessor of underlying GeometryTopology */
    std::shared_ptr<GeometryTopology> getVertex();

    /** Accessor to mesh graph edge list begin iterator */
    std::vector<std::shared_ptr<MeshGraphEdge>>::iterator getConnectedEdgeBeginIterator();

    /** Accessor to mesh graph edge list end iterator */
    std::vector<std::shared_ptr<MeshGraphEdge>>::iterator getConnectedEdgeEndIterator();

private:
    std::shared_ptr<GeometryTopology> _vertex;
    std::vector<std::shared_ptr<MeshGraphEdge>> _connectedEdge;
};

#endif