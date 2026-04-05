/**
 * @file MeshTransfiniteInterpolationInputData.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Implementation] Concrete class to handle Transfinite Interpolation algorithm input data.
 */

#include <unordered_map>
#include <memory>
#include <array>

#include "transfinite-interpolation/MeshTransfiniteInterpolationInputData.hpp"

#include "data-structure/MeshGraphVertex.hpp"
#include "data-structure/MeshGraphEdge.hpp"

MeshTransfiniteInterpolationInputData::MeshTransfiniteInterpolationInputData() {
    // TBA
}

MeshTransfiniteInterpolationInputData::~MeshTransfiniteInterpolationInputData() {
    // TBA
}

void MeshTransfiniteInterpolationInputData::transform(std::shared_ptr<GeometryTopology> neutral_geometry_topology) {
    std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> edge_list;
    neutral_geometry_topology->getDescendants(edge_list, GeometryTopology::EDGE);

    std::unordered_map<std::shared_ptr<GeometryTopology>, std::shared_ptr<MeshGraphVertex>> mesh_graph_vertex_mapping;
    for (auto edge_iter = edge_list.begin(); edge_iter != edge_list.end(); edge_iter++) {
        std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> vertex_list;
        ((*edge_iter).first)->getDescendants(vertex_list, GeometryTopology::VERTEX);

        std::array<std::shared_ptr<MeshGraphVertex>, 2> vertex_in_edge;
        for (auto vertex_iter = vertex_list.begin(); vertex_iter != vertex_list.end(); vertex_iter++) {
            if (!mesh_graph_vertex_mapping.contains((*vertex_iter).first)) {
                mesh_graph_vertex_mapping.insert(std::make_pair((*vertex_iter).first, new MeshGraphVertex((*vertex_iter).first)));
            }

            vertex_in_edge[(*vertex_iter).second] = mesh_graph_vertex_mapping.at((*vertex_iter).first);
        }

        _meshGraphEdgeList.push_back(std::make_shared<MeshGraphEdge>(vertex_in_edge[0], vertex_in_edge[1]));
        for (unsigned int i = 0; i < 2; i++) {
            vertex_in_edge[i]->addConnectedEdge(_meshGraphEdgeList.back());
        }
    }
}

std::vector<std::shared_ptr<MeshGraphEdge>>::iterator MeshTransfiniteInterpolationInputData::getMeshGraphEdgeListBeginIterator() {
    return _meshGraphEdgeList.begin();
}

std::vector<std::shared_ptr<MeshGraphEdge>>::iterator MeshTransfiniteInterpolationInputData::getMeshGraphEdgeListEndIterator() {
    return _meshGraphEdgeList.end();
}