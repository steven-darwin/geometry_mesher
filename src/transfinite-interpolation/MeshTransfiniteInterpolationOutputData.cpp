/**
 * @file MeshTransfiniteInterpolationOutputData.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-10-05
 *
 * @brief [Implementation] Concrete class to handle Transfinite Interpolation algorithm output data.
 */

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <optional>
#include <functional>
#include <iterator>
#include <algorithm>
#include <iostream>

#include <geometry-topology/GeometryTopology.hpp>
#include <geometry-topology/GeometryTopologyVertex.hpp>
#include <geometry-topology/GeometryTopologyEdge.hpp>
#include <geometry-topology/GeometryTopologyWire.hpp>
#include <geometry-topology/GeometryTopologyFace.hpp>
#include <geometry-topology/GeometryTopologyShell.hpp>
#include <geometry-topology/GeometryTopologyCell.hpp>
#include <geometry-topology/GeometryTopologyComposite.hpp>

#include "data-structure/MeshGraphVertex.hpp"
#include "data-structure/MeshGraphEdge.hpp"

#include "transfinite-interpolation/MeshTransfiniteInterpolationOutputData.hpp"

MeshTransfiniteInterpolationOutputData::MeshTransfiniteInterpolationOutputData() {
    // TBA
}

MeshTransfiniteInterpolationOutputData::~MeshTransfiniteInterpolationOutputData() {
    // TBA
}

std::shared_ptr<GeometryTopology> MeshTransfiniteInterpolationOutputData::transform() {
    std::unordered_map<std::shared_ptr<MeshGraphEdge>, std::pair<unsigned int, std::shared_ptr<GeometryTopologyEdge>>> edge_list;
    unsigned int counter = 0;
    for (auto edge_iter = _meshGraphEdgeList.begin(); edge_iter != _meshGraphEdgeList.end(); edge_iter++) {
        edge_list.insert(std::make_pair((*edge_iter), std::make_pair(counter, std::make_shared<GeometryTopologyEdge>(std::dynamic_pointer_cast<GeometryTopologyVertex>((*edge_iter)->getConnectedVertex(0)->getVertex()), std::dynamic_pointer_cast<GeometryTopologyVertex>((*edge_iter)->getConnectedVertex(1)->getVertex())))));
        counter++;
    }

    std::unordered_map<std::shared_ptr<GeometryTopologyFace>, std::pair<unsigned int, std::shared_ptr<MeshGraphVertex>>> face_list;
    std::unordered_set<unsigned long long> face_list_idx;

    std::vector<std::vector<std::shared_ptr<GeometryTopologyFace>>> incident_face_on_edge;
    incident_face_on_edge.resize(edge_list.size());

    for (auto edge_iter = _meshGraphEdgeList.begin(); edge_iter != _meshGraphEdgeList.end(); edge_iter++) {
        std::unordered_set<std::shared_ptr<MeshGraphEdge>> traversed_edges;
        constructFaceFromCycledGraphEdge(edge_list, face_list, incident_face_on_edge, (*edge_iter), nullptr, traversed_edges, face_list_idx);
    }

    for (auto incident_face_on_edge_iter = incident_face_on_edge.begin(); incident_face_on_edge_iter != incident_face_on_edge.end(); incident_face_on_edge_iter++) {
        for (unsigned int i = 0; i < (*incident_face_on_edge_iter).size(); i++) {
            for (unsigned int j = i + 1; j < (*incident_face_on_edge_iter).size(); j++) {
                std::shared_ptr<MeshGraphEdge> adj_face_as_graph_edge = std::make_shared<MeshGraphEdge>(face_list.at((*incident_face_on_edge_iter)[i]).second, face_list.at((*incident_face_on_edge_iter)[j]).second);
                face_list.at((*incident_face_on_edge_iter)[i]).second->addConnectedEdge(adj_face_as_graph_edge);
                face_list.at((*incident_face_on_edge_iter)[j]).second->addConnectedEdge(adj_face_as_graph_edge);
            }
        }
    }

    std::unordered_map<std::shared_ptr<GeometryTopologyCell>, unsigned int> cell_list;
    std::unordered_set<unsigned long long> cell_list_idx;

    std::unordered_map<std::shared_ptr<MeshGraphVertex>, unsigned int> face_as_graph_vertex_list;
    unsigned int counter3 = 0;
    for (auto face_iter = face_list.begin(); face_iter != face_list.end(); face_iter++) {
        face_as_graph_vertex_list.insert(std::make_pair((*face_iter).second.second, counter3));
        counter3++;
    }

    for (auto face_iter = face_as_graph_vertex_list.begin(); face_iter != face_as_graph_vertex_list.end(); face_iter++) {
        std::unordered_set<std::shared_ptr<MeshGraphVertex>> traversed_vertices;
        constructCellFromCycledGraphVertex(face_as_graph_vertex_list, cell_list, (*face_iter).first, traversed_vertices, cell_list_idx);
    }

    _composite = std::make_shared<GeometryTopologyComposite>();
    for (auto cell_list_iter = cell_list.begin(); cell_list_iter != cell_list.end(); cell_list_iter++) {
        _composite->addCell((*cell_list_iter).first);
    }

    return _composite;
}

void MeshTransfiniteInterpolationOutputData::insertMeshGraphEdge(std::shared_ptr<MeshGraphEdge> mesh_graph_edge) {
    _meshGraphEdgeList.push_back(mesh_graph_edge);
}

double MeshTransfiniteInterpolationOutputData::getCombinationCount(unsigned int set_element_count, unsigned int sample_element_count) {
    return calcFactorial(set_element_count, set_element_count - sample_element_count) / calcFactorial(sample_element_count);
}

unsigned long long MeshTransfiniteInterpolationOutputData::getCombinationID(std::vector<unsigned int> element_id_list) {
    unsigned int result = 0;

    for (unsigned int i = element_id_list.size(); i > 0; i--) {
        if (element_id_list[i - 1] >= i) {
            result += getCombinationCount(element_id_list[i - 1], i);
        }
    }

    return result;
}

double MeshTransfiniteInterpolationOutputData::calcFactorial(unsigned int num) {
    double result = num;

    if (num > 1) {
        result *= calcFactorial(num - 1);
    }
    else {
        result = 1;
    }

    return result;
}

double MeshTransfiniteInterpolationOutputData::calcFactorial(unsigned int upper_bound, unsigned int lower_bound) {
    double result = upper_bound;

    if (upper_bound > lower_bound) {
        result *= calcFactorial(upper_bound - 1, lower_bound);
    }
    else {
        result = 1;
    }

    return result;
}

void MeshTransfiniteInterpolationOutputData::constructFaceFromCycledGraphEdge(std::unordered_map<std::shared_ptr<MeshGraphEdge>, std::pair<unsigned int, std::shared_ptr<GeometryTopologyEdge>>>& edge_list, std::unordered_map<std::shared_ptr<GeometryTopologyFace>, std::pair<unsigned int, std::shared_ptr<MeshGraphVertex>>>& face_list, std::vector<std::vector<std::shared_ptr<GeometryTopologyFace>>>& incident_face_on_edge, std::shared_ptr<MeshGraphEdge> next_edge, std::shared_ptr<MeshGraphVertex> prev_vertex, std::unordered_set<std::shared_ptr<MeshGraphEdge>> traversed_edges, std::unordered_set<unsigned long long>& face_list_idx) {
    if (traversed_edges.contains(next_edge)) return;

    traversed_edges.insert(next_edge);

    if (next_edge->getConnectedVertex(0) != prev_vertex) {
        prev_vertex = next_edge->getConnectedVertex(0);
    }
    else {
        prev_vertex = next_edge->getConnectedVertex(1);
    }

    for (auto connected_edge_iter = prev_vertex->getConnectedEdgeBeginIterator(); connected_edge_iter != prev_vertex->getConnectedEdgeEndIterator(); connected_edge_iter++) {
        if ((*connected_edge_iter) != next_edge) {
            if (traversed_edges.size() == 4) {
                if (traversed_edges.contains(*connected_edge_iter)) {
                    std::vector<unsigned int> sorted_edge_idx;
                    for (auto traversed_edge_iter = traversed_edges.begin(); traversed_edge_iter != traversed_edges.end(); traversed_edge_iter++) {
                        sorted_edge_idx.push_back(edge_list.at(*traversed_edge_iter).first);
                    }
                    std::sort(sorted_edge_idx.begin(), sorted_edge_idx.end());
                    
                    if (!face_list_idx.contains(getCombinationID(sorted_edge_idx))) {
                        std::shared_ptr<GeometryTopologyWire> wire = std::make_shared<GeometryTopologyWire>();
                        std::shared_ptr<GeometryTopologyVertex> prev_geometry_topology_vertex = nullptr;
                        for (auto traversed_edge_iter = traversed_edges.begin(); traversed_edge_iter != traversed_edges.end(); traversed_edge_iter++) {
                            unsigned int origin_vertex_idx;
                            if (edge_list.at(*traversed_edge_iter).second->getVertex(0) == prev_geometry_topology_vertex) {
                                origin_vertex_idx = 0;
                                prev_geometry_topology_vertex = edge_list.at(*traversed_edge_iter).second->getVertex(1);
                            }
                            else {
                                origin_vertex_idx = 1;
                                prev_geometry_topology_vertex = edge_list.at(*traversed_edge_iter).second->getVertex(0);
                            }

                            wire->addEdge(edge_list.at(*traversed_edge_iter).second, origin_vertex_idx);
                        }
                    
                        std::shared_ptr<GeometryTopologyFace> face(new GeometryTopologyFace(wire, wire->reverseCopy()));
                        face_list.insert(std::make_pair(face, std::make_pair(getCombinationID(sorted_edge_idx), std::make_shared<MeshGraphVertex>(face))));
                        face_list_idx.insert(getCombinationID(sorted_edge_idx));

                        for (auto sorted_edge_idx_iter = sorted_edge_idx.begin(); sorted_edge_idx_iter != sorted_edge_idx.end(); sorted_edge_idx_iter++) {
                            incident_face_on_edge[*sorted_edge_idx_iter].push_back(face);
                        }
                    }

                    break;
                }
            }
            else if (traversed_edges.size() < 4) {
                constructFaceFromCycledGraphEdge(edge_list, face_list, incident_face_on_edge, (*connected_edge_iter), prev_vertex, traversed_edges, face_list_idx);
            }
            else {
                break;
            }
        }
    }
}

void MeshTransfiniteInterpolationOutputData::constructCellFromCycledGraphEdge(std::unordered_map<std::shared_ptr<MeshGraphEdge>, unsigned int>& edge_list, std::unordered_map<std::shared_ptr<GeometryTopologyCell>, unsigned int>& cell_list, std::shared_ptr<MeshGraphEdge> next_edge, std::shared_ptr<MeshGraphVertex> prev_vertex, std::unordered_set<std::shared_ptr<MeshGraphEdge>> traversed_edges, std::unordered_set<std::shared_ptr<GeometryTopologyVertex>> traversed_geometry_topology_vertex, std::unordered_set<unsigned long long>& cell_list_idx) {
    if (traversed_edges.contains(next_edge)) return;

    traversed_edges.insert(next_edge);

    if (next_edge->getConnectedVertex(0) != prev_vertex) {
        prev_vertex = next_edge->getConnectedVertex(0);
    }
    else {
        prev_vertex = next_edge->getConnectedVertex(1);
    }

    std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> vertex_on_face;
    prev_vertex->getVertex()->getDescendants(vertex_on_face, GeometryTopology::VERTEX);

    unsigned int traversed_vertex_counter = 0;
    for (auto vertex_on_face_iter = vertex_on_face.begin(); vertex_on_face_iter != vertex_on_face.end(); vertex_on_face_iter++) {
        if (!traversed_geometry_topology_vertex.contains(std::dynamic_pointer_cast<GeometryTopologyVertex>((*vertex_on_face_iter).first))) {
            traversed_geometry_topology_vertex.insert(std::dynamic_pointer_cast<GeometryTopologyVertex>((*vertex_on_face_iter).first));
        }
        else {
            traversed_vertex_counter++;
        }
    }

    for (auto connected_edge_iter = prev_vertex->getConnectedEdgeBeginIterator(); connected_edge_iter != prev_vertex->getConnectedEdgeEndIterator(); connected_edge_iter++) {
        if ((*connected_edge_iter) != next_edge) {
            if (traversed_edges.size() == 6 && traversed_geometry_topology_vertex.size() == 8 && traversed_vertex_counter == 4) {
                if (traversed_edges.contains(*connected_edge_iter)) {
                    std::vector<unsigned int> sorted_edge_idx;
                    for (auto traversed_edge_iter = traversed_edges.begin(); traversed_edge_iter != traversed_edges.end(); traversed_edge_iter++) {
                        sorted_edge_idx.push_back(edge_list.at(*traversed_edge_iter));
                    }
                    std::sort(sorted_edge_idx.begin(), sorted_edge_idx.end());

                    if (!cell_list_idx.contains(getCombinationID(sorted_edge_idx))) {
                        std::shared_ptr<GeometryTopologyShell> shell = std::make_shared<GeometryTopologyShell>();
                        std::unordered_set<std::shared_ptr<GeometryTopologyFace>> traversed_geometry_topology_faces;
                        for (auto traversed_edge_iter = traversed_edges.begin(); traversed_edge_iter != traversed_edges.end(); traversed_edge_iter++) {
                            unsigned int non_existing_face_idx;
                            if (!traversed_geometry_topology_faces.contains(std::dynamic_pointer_cast<GeometryTopologyFace>((*traversed_edge_iter)->getConnectedVertex(0)->getVertex()))) {
                                non_existing_face_idx = 0;
                            }
                            else {
                                non_existing_face_idx = 1;
                            }
                            // To Do: create logic to choose face pointer and wire_idx that will be added to the shell
                            shell->addFace(std::dynamic_pointer_cast<GeometryTopologyFace>((*traversed_edge_iter)->getConnectedVertex(non_existing_face_idx)->getVertex()), 0);
                        }

                        std::shared_ptr<GeometryTopologyCell> cell(new GeometryTopologyCell(shell, shell->reverseCopy()));
                        cell_list.insert(std::make_pair(cell, getCombinationID(sorted_edge_idx)));
                        cell_list_idx.insert(getCombinationID(sorted_edge_idx));

                        for (auto iter = sorted_edge_idx.begin(); iter != sorted_edge_idx.end(); iter++) {
                            std::cout << (*iter) << " ";
                        }
                        std::cout << "| " << getCombinationID(sorted_edge_idx) << std::endl;
                    }

                    break;
                }
            }
            else if (traversed_edges.size() < 6 && traversed_geometry_topology_vertex.size() <= 8) {
                constructCellFromCycledGraphEdge(edge_list, cell_list, (*connected_edge_iter), prev_vertex, traversed_edges, traversed_geometry_topology_vertex, cell_list_idx);
            }
            else {
                break;
            }
        }
    }
}

void MeshTransfiniteInterpolationOutputData::constructFaceFromCycledGraphVertex(std::unordered_map<std::shared_ptr<MeshGraphVertex>, unsigned int>& graph_vertex_list, std::unordered_map<std::shared_ptr<GeometryTopologyFace>, std::shared_ptr<MeshGraphVertex>>& face_list, std::shared_ptr<MeshGraphVertex> next_vertex, std::unordered_set<std::shared_ptr<MeshGraphVertex>> traversed_vertices, std::unordered_set<unsigned long long>& face_list_idx) {
    //traversed_vertices.insert(next_vertex);

    //bool is_cycled = true;
    //for (auto connected_edge_iter = next_vertex->getConnectedEdgeBeginIterator(); connected_edge_iter != next_vertex->getConnectedEdgeEndIterator(); connected_edge_iter++) {
    //    for (unsigned int i = 0; i < 2; i++) {
    //        if (!traversed_vertices.contains((*connected_edge_iter)->getConnectedVertex(i))) {
    //            next_vertex = (*connected_edge_iter)->getConnectedVertex(i);
    //            is_cycled = false;
    //            break;
    //        }
    //    }
    //    if (!is_cycled) break;
    //}

    //if (traversed_vertices.size() == 4 && is_cycled) {
    //    std::vector<unsigned int> sorted_vertex_idx;
    //    for (auto traversed_vertex_iter = traversed_vertices.begin(); traversed_vertex_iter != traversed_vertices.end(); traversed_vertex_iter++) {
    //        sorted_vertex_idx.push_back(graph_vertex_list.at(*traversed_vertex_iter));
    //    }
    //    std::sort(sorted_vertex_idx.begin(), sorted_vertex_idx.end());

    //    if (!face_list_idx.contains(getCombinationID(sorted_vertex_idx))) {
    //        std::shared_ptr<GeometryTopologyWire> wire = std::make_shared<GeometryTopologyWire>();
    //        for (auto traversed_vertex_iter = traversed_vertices.begin(); traversed_vertex_iter != traversed_vertices.end(); traversed_vertex_iter++) {
    //            // To Do: create logic to choose face pointer and wire_idx that will be added to the shell
    //            wire->addEdge::dynamic_pointer_cast<GeometryTopologyEdge>((*traversed_vertex_iter)->getVertex()), 0);
    //        }

    //        std::shared_ptr<GeometryTopologyFace> face = std::make_shared<GeometryTopologyFace>(wire, wire->reverseCopy());
    //        face_list.insert(std::make_pair(face, getCombinationID(sorted_vertex_idx)));
    //        face_list_idx.insert(getCombinationID(sorted_vertex_idx));

    //        for (auto iter = sorted_vertex_idx.begin(); iter != sorted_vertex_idx.end(); iter++) {
    //            std::cout << (*iter) << " ";
    //        }
    //        std::cout << "| " << getCombinationID(sorted_vertex_idx) << std::endl;
    //    }

    //    return;
    //}
    //else if (traversed_vertices.size() < 4 && !is_cycled) {
    //    constructFaceFromCycledGraphVertex(graph_vertex_list, face_list, next_vertex, traversed_vertices, face_list_idx);
    //}
    //else {
    //    return;
    //}
}


void MeshTransfiniteInterpolationOutputData::constructCellFromCycledGraphVertex(std::unordered_map<std::shared_ptr<MeshGraphVertex>, unsigned int>& graph_vertex_list, std::unordered_map<std::shared_ptr<GeometryTopologyCell>, unsigned int>& cell_list, std::shared_ptr<MeshGraphVertex> next_vertex, std::unordered_set<std::shared_ptr<MeshGraphVertex>> traversed_vertices, std::unordered_set<unsigned long long>& cell_list_idx) {
    traversed_vertices.insert(next_vertex);

    unsigned int complying_face_count = 0;
    if (traversed_vertices.size() < 6) {
        for (auto connected_edge_iter = next_vertex->getConnectedEdgeBeginIterator(); connected_edge_iter != next_vertex->getConnectedEdgeEndIterator(); connected_edge_iter++) {
            for (unsigned int i = 0; i < 2; i++) {
                if (!traversed_vertices.contains((*connected_edge_iter)->getConnectedVertex(i))) {
                    constructCellFromCycledGraphVertex(graph_vertex_list, cell_list, (*connected_edge_iter)->getConnectedVertex(i), traversed_vertices, cell_list_idx);
                }
            }
        }
    }
    else if (traversed_vertices.size() == 6) {
        for (auto traversed_vertex_iter = traversed_vertices.begin(); traversed_vertex_iter != traversed_vertices.end(); traversed_vertex_iter++) {
            unsigned int traversed_vertex_counter = 0;
            for (auto connected_edge_iter = (*traversed_vertex_iter)->getConnectedEdgeBeginIterator(); connected_edge_iter != (*traversed_vertex_iter)->getConnectedEdgeEndIterator(); connected_edge_iter++) {
                bool has_traversed = true;
                for (unsigned int i = 0; i < 2; i++) {
                    if (!traversed_vertices.contains((*connected_edge_iter)->getConnectedVertex(i))) {
                        has_traversed = false;
                    }
                }
                if (has_traversed) traversed_vertex_counter++;
            }
            if (traversed_vertex_counter == 4) complying_face_count++;
        }
    }
    else {
        return;
    }

    if (traversed_vertices.size() == 6 && complying_face_count == 6) {
        std::vector<unsigned int> sorted_vertex_idx;
        for (auto traversed_vertex_iter = traversed_vertices.begin(); traversed_vertex_iter != traversed_vertices.end(); traversed_vertex_iter++) {
            sorted_vertex_idx.push_back(graph_vertex_list.at(*traversed_vertex_iter));
        }
        std::sort(sorted_vertex_idx.begin(), sorted_vertex_idx.end());

        if (!cell_list_idx.contains(getCombinationID(sorted_vertex_idx))) {
            std::shared_ptr<GeometryTopologyShell> shell = std::make_shared<GeometryTopologyShell>();
            for (auto traversed_vertex_iter = traversed_vertices.begin(); traversed_vertex_iter != traversed_vertices.end(); traversed_vertex_iter++) {
                // To Do: create logic to choose face pointer and wire_idx that will be added to the shell
                shell->addFace(std::dynamic_pointer_cast<GeometryTopologyFace>((*traversed_vertex_iter)->getVertex()), 0);
            }

            std::shared_ptr<GeometryTopologyCell> cell(new GeometryTopologyCell(shell, shell->reverseCopy()));
            cell_list.insert(std::make_pair(cell, getCombinationID(sorted_vertex_idx)));
            cell_list_idx.insert(getCombinationID(sorted_vertex_idx));

            /*for (auto iter = sorted_vertex_idx.begin(); iter != sorted_vertex_idx.end(); iter++) {
                std::cout << (*iter) << " ";
            }
            std::cout << "| " << getCombinationID(sorted_vertex_idx) << std::endl;*/
        }
    }
}
