/**
 * @file MeshGraphVertex.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-09-28
 * @date Last Modified : 2025-09-28
 *
 * @brief [Implementation] Class for representing vertex on mesh's graph data structure.
 */

#include <memory>
#include <iostream>

#include "data-structure/MeshGraphVertex.hpp"
#include "data-structure/MeshGraphEdge.hpp"

MeshGraphVertex::MeshGraphVertex(std::shared_ptr<GeometryTopology> geometry_topology) {
    _vertex = geometry_topology;
}

MeshGraphVertex::~MeshGraphVertex() {
    // TBA
}

void MeshGraphVertex::addConnectedEdge(std::shared_ptr<MeshGraphEdge> mesh_graph_edge) {
    _connectedEdge.push_back(mesh_graph_edge);
}

std::shared_ptr<GeometryTopology> MeshGraphVertex::getVertex() {
    return _vertex;
}

std::vector<std::shared_ptr<MeshGraphEdge>>::iterator MeshGraphVertex::getConnectedEdgeBeginIterator() {
    return _connectedEdge.begin();
}

std::vector<std::shared_ptr<MeshGraphEdge>>::iterator MeshGraphVertex::getConnectedEdgeEndIterator() {
    return _connectedEdge.end();
}