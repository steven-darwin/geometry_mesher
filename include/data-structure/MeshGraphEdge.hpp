/**
 * @file MeshGraphEdge.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-09-28
 * @date Last Modified : 2025-09-28
 *
 * @brief [Header] Class for representing edge on mesh's graph data structure.
 */

#ifndef MESH_GRAPH_EDGE_HPP
#define MESH_GRAPH_EDGE_HPP

#include <array>
#include <memory>

class MeshGraphVertex;

class MeshGraphEdge {
public:
    /** Constructor of MeshGraphEdge object
     */
    MeshGraphEdge(std::shared_ptr<MeshGraphVertex> first_mesh_graph_vertex, std::shared_ptr<MeshGraphVertex> second_mesh_graph_vertex);

    /** Destructor of MeshGraphEdge object */
    ~MeshGraphEdge();

    /** Accessor to the list of connected graph vertex */
    std::shared_ptr<MeshGraphVertex> getConnectedVertex(unsigned int vertex_idx);

private:
    std::array<std::shared_ptr<MeshGraphVertex>, 2> _connectedVertex;
};

#endif