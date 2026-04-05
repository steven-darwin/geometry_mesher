/**
 * @file MeshGraphEdge.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-09-28
 * @date Last Modified : 2025-09-28
 *
 * @brief [Implementation] Class for representing edge on mesh's graph data structure.
 */

#include <array>
#include <iostream>

#include "data-structure/MeshGraphVertex.hpp"
#include "data-structure/MeshGraphEdge.hpp"

MeshGraphEdge::MeshGraphEdge(std::shared_ptr<MeshGraphVertex> first_mesh_graph_vertex, std::shared_ptr<MeshGraphVertex> second_mesh_graph_vertex) {
    _connectedVertex[0] = first_mesh_graph_vertex;
    _connectedVertex[1] = second_mesh_graph_vertex;
}

MeshGraphEdge::~MeshGraphEdge() {
    // TBA
}

std::shared_ptr<MeshGraphVertex> MeshGraphEdge::getConnectedVertex(unsigned int vertex_idx) {
    return _connectedVertex[vertex_idx];
}