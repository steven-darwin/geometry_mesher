/**
 * @file MeshTransfiniteInterpolationAlgorithm.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-10-04
 *
 * @brief [Implementation] Concrete class for Transfinite Interpolation algorithm.
 */

#include <unordered_set>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <optional>
#include <iostream>

#include "geometry-topology/GeometryTopologyVertex.hpp"

#include "data-structure/MeshGraphVertex.hpp"
#include "data-structure/MeshGraphEdge.hpp"

#include "transfinite-interpolation/MeshTransfiniteInterpolationInputData.hpp"
#include "transfinite-interpolation/MeshTransfiniteInterpolationAlgorithm.hpp"
#include "transfinite-interpolation/MeshTransfiniteInterpolationOutputData.hpp"

MeshTransfiniteInterpolationAlgorithm::MeshTransfiniteInterpolationAlgorithm(std::shared_ptr<MeshInputData> input_data, std::shared_ptr<MeshOutputData> output_data) {
    _inputData = input_data;
    _outputData = output_data;
}

MeshTransfiniteInterpolationAlgorithm::~MeshTransfiniteInterpolationAlgorithm() {
    // TBA
}

void MeshTransfiniteInterpolationAlgorithm::run(unsigned int step_count) {
    // Mapped real vertex to logical vertex
    std::unordered_map<std::shared_ptr<MeshGraphVertex>, std::shared_ptr<std::array<double, 3>>> normalized_graph_vertex_mapping;

    std::shared_ptr<MeshGraphVertex> origin_vertex;
    for (auto graph_edge_iter = std::dynamic_pointer_cast<MeshTransfiniteInterpolationInputData>(_inputData)->getMeshGraphEdgeListBeginIterator(); graph_edge_iter != std::dynamic_pointer_cast<MeshTransfiniteInterpolationInputData>(_inputData)->getMeshGraphEdgeListEndIterator(); graph_edge_iter++) {
        for (unsigned int i = 0; i < 2; i++) {
            std::shared_ptr<std::array<double, 3>> normalized_coordinate(new std::array<double, 3>{0.0f, 0.0f, 0.0f});
            if (normalized_graph_vertex_mapping.size() == 0) {
                origin_vertex = (*graph_edge_iter)->getConnectedVertex(i);
                normalized_graph_vertex_mapping.insert(std::make_pair(origin_vertex, normalized_coordinate));
            } else {
                if (!normalized_graph_vertex_mapping.contains((*graph_edge_iter)->getConnectedVertex(i))) {
                    std::array<double, 3> diff_vector{ 0.0f, 0.0f, 0.0f };

                    for (int j = 0; j < 3; j++) {
                        diff_vector[j] = std::dynamic_pointer_cast<GeometryTopologyVertex>((*graph_edge_iter)->getConnectedVertex(i)->getVertex())->getCoordinate(static_cast<GeometryTopologyVertex::Coordinate>(j)) - std::dynamic_pointer_cast<GeometryTopologyVertex>(origin_vertex->getVertex())->getCoordinate(static_cast<GeometryTopologyVertex::Coordinate>(j));
                        if (diff_vector[j] != 0) {
                            (*normalized_coordinate)[j] = 1.0f;
                        }
                    }

                    normalized_graph_vertex_mapping.insert(std::make_pair((*graph_edge_iter)->getConnectedVertex(i), normalized_coordinate));
                }
            }
        }
    }

    // Discretize and map logical vertex to real vertex
    std::cout << "------------------------------------------" << std::endl;
    std::vector<std::array<double, 3>> real_vertex_coordinate_list;
    real_vertex_coordinate_list.resize(normalized_graph_vertex_mapping.size());
    for (auto normalized_vertex_iter = normalized_graph_vertex_mapping.begin(); normalized_vertex_iter != normalized_graph_vertex_mapping.end(); normalized_vertex_iter++) {
        std::shared_ptr<GeometryTopologyVertex> current_iter_real_vertex = std::dynamic_pointer_cast<GeometryTopologyVertex>((*normalized_vertex_iter).first->getVertex());
        real_vertex_coordinate_list[((*(*normalized_vertex_iter).second)[2] * 4) + ((*(*normalized_vertex_iter).second)[1] * 2) + ((*(*normalized_vertex_iter).second)[0] * 1)] = { 
            current_iter_real_vertex->getCoordinate(GeometryTopologyVertex::X), 
            current_iter_real_vertex->getCoordinate(GeometryTopologyVertex::Y), 
            current_iter_real_vertex->getCoordinate(GeometryTopologyVertex::Z) 
        };
        std::cout << (*(*normalized_vertex_iter).second)[0] << " | " << (*(*normalized_vertex_iter).second)[1] << " | " << (*(*normalized_vertex_iter).second)[2] << std::endl;
        std::cout << current_iter_real_vertex->getCoordinate(GeometryTopologyVertex::X) << " | " << current_iter_real_vertex->getCoordinate(GeometryTopologyVertex::Y) << " | " << current_iter_real_vertex->getCoordinate(GeometryTopologyVertex::Z) << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;

    double step_size = 1.0f / step_count;
    std::vector<std::shared_ptr<MeshGraphVertex>> vertex_in_prev_i_plane;
    vertex_in_prev_i_plane.resize((step_count + 1) * (step_count + 1));
    for (unsigned int i = 0; i <= step_count; i++) {
        std::vector<std::shared_ptr<MeshGraphVertex>> vertex_in_prev_j_line;
        vertex_in_prev_j_line.resize(step_count + 1);

        for (unsigned int j = 0; j <= step_count; j++) {
            std::shared_ptr<MeshGraphVertex> vertex_in_prev_k_point;

            for (unsigned int k = 0; k <= step_count; k++) {
                std::cout << i << " " << j << " " << k << std::endl;

                std::array<double, 3> current_iter_vertex_coordinate = map3DLogicalToRealCoordinate(real_vertex_coordinate_list, (i * step_size), (j * step_size), (k * step_size));
                std::shared_ptr<GeometryTopologyVertex> current_iter_geometry_topology_vertex(new GeometryTopologyVertex(current_iter_vertex_coordinate[0], current_iter_vertex_coordinate[1], current_iter_vertex_coordinate[2]));

                std::shared_ptr<MeshGraphVertex> current_iter_graph_vertex(new MeshGraphVertex(current_iter_geometry_topology_vertex));

                if (k > 0) {
                    std::shared_ptr<MeshGraphEdge> edge_to_prev_k_vertex(new MeshGraphEdge(current_iter_graph_vertex, vertex_in_prev_k_point));
                    current_iter_graph_vertex->addConnectedEdge(edge_to_prev_k_vertex);
                    vertex_in_prev_k_point->addConnectedEdge(edge_to_prev_k_vertex);
                    std::dynamic_pointer_cast<MeshTransfiniteInterpolationOutputData>(_outputData)->insertMeshGraphEdge(edge_to_prev_k_vertex);
                }

                if (j > 0) {
                    std::shared_ptr<MeshGraphEdge> edge_to_prev_j_vertex(new MeshGraphEdge(current_iter_graph_vertex, vertex_in_prev_j_line[k]));
                    current_iter_graph_vertex->addConnectedEdge(edge_to_prev_j_vertex);
                    vertex_in_prev_j_line[k]->addConnectedEdge(edge_to_prev_j_vertex);
                    std::dynamic_pointer_cast<MeshTransfiniteInterpolationOutputData>(_outputData)->insertMeshGraphEdge(edge_to_prev_j_vertex);
                }

                if (i > 0) {
                    std::shared_ptr<MeshGraphEdge> edge_to_prev_i_vertex(new MeshGraphEdge(current_iter_graph_vertex, vertex_in_prev_i_plane[(j * (step_count + 1)) + k]));
                    current_iter_graph_vertex->addConnectedEdge(edge_to_prev_i_vertex);
                    vertex_in_prev_i_plane[(j * (step_count + 1)) + k]->addConnectedEdge(edge_to_prev_i_vertex);
                    std::dynamic_pointer_cast<MeshTransfiniteInterpolationOutputData>(_outputData)->insertMeshGraphEdge(edge_to_prev_i_vertex);
                }

                vertex_in_prev_k_point = current_iter_graph_vertex;
                vertex_in_prev_j_line[k] = current_iter_graph_vertex;
                vertex_in_prev_i_plane[(j * (step_count + 1)) + k] = current_iter_graph_vertex;
            }
        }
    }
}

std::array<double, 3> MeshTransfiniteInterpolationAlgorithm::map3DLogicalToRealCoordinate(std::vector<std::array<double, 3>> real_vertex_coordinate_list, std::optional<double> x1, std::optional<double> x2, std::optional<double> x3) {
    std::array<double, 3> result;
    
    // Face interpolation
    std::vector<std::array<double, 3>> face_itpl_value;
    face_itpl_value.resize(6);

    std::array<double, 6> face_itpl_weighted_constant = { 
        (1.0f - x3.value()), 
        (1.0f - x2.value()), 
        (1.0f - x1.value()), 
        x3.value(), 
        x2.value(), 
        x1.value()
    };

    std::optional<double> empty;

    face_itpl_value[0] = map2DLogicalToRealCoordinate(
        { real_vertex_coordinate_list[0], real_vertex_coordinate_list[1], real_vertex_coordinate_list[3], real_vertex_coordinate_list[2] },
        x1, x2, empty
    );
    face_itpl_value[1] = map2DLogicalToRealCoordinate(
        { real_vertex_coordinate_list[0], real_vertex_coordinate_list[1], real_vertex_coordinate_list[5], real_vertex_coordinate_list[4] },
        x1, empty, x3
    );
    face_itpl_value[2] = map2DLogicalToRealCoordinate(
        { real_vertex_coordinate_list[0], real_vertex_coordinate_list[2], real_vertex_coordinate_list[6], real_vertex_coordinate_list[4] },
        empty, x2, x3
    );
    face_itpl_value[3] = map2DLogicalToRealCoordinate(
        { real_vertex_coordinate_list[4], real_vertex_coordinate_list[5], real_vertex_coordinate_list[7], real_vertex_coordinate_list[6] },
        x1, x2, empty
    );
    face_itpl_value[4] = map2DLogicalToRealCoordinate(
        { real_vertex_coordinate_list[2], real_vertex_coordinate_list[3], real_vertex_coordinate_list[7], real_vertex_coordinate_list[6] },
        x1, empty, x3
    );
    face_itpl_value[5] = map2DLogicalToRealCoordinate(
        { real_vertex_coordinate_list[1], real_vertex_coordinate_list[3], real_vertex_coordinate_list[7], real_vertex_coordinate_list[5] },
        empty, x2, x3
    );

    for (unsigned int i = 0; i < 6; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            face_itpl_value[i][j] *= face_itpl_weighted_constant[i];
        }
    }

    // Vertex correction
    std::vector<std::array<double, 3>> vertex_corr_val;
    vertex_corr_val.resize(8);

    std::array<double, 8> vertex_corr_weighted_constant = { 
        (1 - x1.value()) * (1 - x2.value()) * (1 - x3.value()),
        x1.value() * (1 - x2.value()) * (1 - x3.value()),
        x1.value() * x2.value() * (1 - x3.value()),
        (1 - x1.value()) * x2.value() * (1 - x3.value()),
        (1 - x1.value()) * (1 - x2.value()) * x3.value(),
        x1.value() * (1 - x2.value()) * x3.value(),
        x1.value() * x2.value() * x3.value(),
        (1 - x1.value()) * x2.value() * x3.value()
    };

    vertex_corr_val[0] = { real_vertex_coordinate_list[0][0], real_vertex_coordinate_list[0][1], real_vertex_coordinate_list[0][2] };
    vertex_corr_val[1] = { real_vertex_coordinate_list[1][0], real_vertex_coordinate_list[1][1], real_vertex_coordinate_list[1][2] };
    vertex_corr_val[2] = { real_vertex_coordinate_list[3][0], real_vertex_coordinate_list[3][1], real_vertex_coordinate_list[3][2] };
    vertex_corr_val[3] = { real_vertex_coordinate_list[2][0], real_vertex_coordinate_list[2][1], real_vertex_coordinate_list[2][2] };
    vertex_corr_val[4] = { real_vertex_coordinate_list[4][0], real_vertex_coordinate_list[4][1], real_vertex_coordinate_list[4][2] };
    vertex_corr_val[5] = { real_vertex_coordinate_list[5][0], real_vertex_coordinate_list[5][1], real_vertex_coordinate_list[5][2] };
    vertex_corr_val[6] = { real_vertex_coordinate_list[7][0], real_vertex_coordinate_list[7][1], real_vertex_coordinate_list[7][2] };
    vertex_corr_val[7] = { real_vertex_coordinate_list[6][0], real_vertex_coordinate_list[6][1], real_vertex_coordinate_list[6][2] };

    for (unsigned int i = 0; i < 8; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            vertex_corr_val[i][j] *= vertex_corr_weighted_constant[i];
        }
    }

    // Accumulation
    for (unsigned int i = 0; i < 3; i++) {
        double face_itpl_acc = std::transform_reduce(face_itpl_value.begin(), face_itpl_value.end(), 0.0, std::plus{}, [i](auto val) { return val[i]; });
        double vertex_corr_acc = std::transform_reduce(vertex_corr_val.begin(), vertex_corr_val.end(), 0.0, std::plus{}, [i](auto val) { return val[i]; });
        result[i] = (face_itpl_acc - vertex_corr_acc) / 2;

        if (std::abs(result[i]) < 0.001) result[i] = 0;
    }

    return result;
}

std::array<double, 3> MeshTransfiniteInterpolationAlgorithm::map2DLogicalToRealCoordinate(std::vector<std::array<double, 3>> real_vertex_coordinate_list, std::optional<double> x1, std::optional<double> x2, std::optional<double> x3) {
    std::array<double, 3> result;

    double y1, y2;
    if (!x1.has_value()) {
        y1 = x2.value();
        y2 = x3.value();
    }
    else if (!x2.has_value()) {
        y1 = x1.value();
        y2 = x3.value();
    }
    else if (!x3.has_value()) {
        y1 = x1.value();
        y2 = x2.value();
    }
    else {
        // do nothing
    }

    // Edge interpolation
    std::vector<std::array<double, 3>> edge_itpl_value;
    edge_itpl_value.resize(4);

    std::array<double, 4> edge_itpl_weighted_constant = {
        (1 - y2),
        y1, 
        y2, 
        (1 - y1)
    };

    std::optional<double> empty;

    edge_itpl_value[0] = map1DLogicalToRealCoordinate(
        { real_vertex_coordinate_list[0], real_vertex_coordinate_list[1] }, 
        x1.has_value() ? x1.value() : empty,
        x2.has_value() ? (x1.has_value() ? empty : x2.value()) : empty,
        empty 
    );
    edge_itpl_value[1] = map1DLogicalToRealCoordinate(
        { real_vertex_coordinate_list[1], real_vertex_coordinate_list[2] },
        empty,
        x2.has_value() ? (x3.has_value() ? empty : x2.value()) : empty,
        x3.has_value() ? x3.value() : empty
    );
    edge_itpl_value[2] = map1DLogicalToRealCoordinate(
        { real_vertex_coordinate_list[3], real_vertex_coordinate_list[2] },
        x1.has_value() ? x1.value() : empty,
        x2.has_value() ? (x1.has_value() ? empty : x2.value()) : empty,
        empty
    );
    edge_itpl_value[3] = map1DLogicalToRealCoordinate(
        { real_vertex_coordinate_list[0], real_vertex_coordinate_list[3] }, 
        empty, 
        x2.has_value() ? (x3.has_value() ? empty : x2.value()) : empty,
        x3.has_value() ? x3.value() : empty
    );

    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            edge_itpl_value[i][j] *= edge_itpl_weighted_constant[i];
        }
    }

    // Vertex correction
    std::vector<std::array<double, 3>> vertex_corr_val;
    vertex_corr_val.resize(4);

    std::array<double, 4> vertex_corr_weighted_constant = { 
        (1 - y1) * (1 - y2),
        y1 * (1 - y2),
        y1 * y2, 
        (1 - y1) * y2
    };

    vertex_corr_val[0] = { real_vertex_coordinate_list[0][0], real_vertex_coordinate_list[0][1], real_vertex_coordinate_list[0][2] };
    vertex_corr_val[1] = { real_vertex_coordinate_list[1][0], real_vertex_coordinate_list[1][1], real_vertex_coordinate_list[1][2] };
    vertex_corr_val[2] = { real_vertex_coordinate_list[2][0], real_vertex_coordinate_list[2][1], real_vertex_coordinate_list[2][2] };
    vertex_corr_val[3] = { real_vertex_coordinate_list[3][0], real_vertex_coordinate_list[3][1], real_vertex_coordinate_list[3][2] };

    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            vertex_corr_val[i][j] *= vertex_corr_weighted_constant[i];
        }
    }

    // Accumulation
    for (unsigned int i = 0; i < 3; i++) {
        double edge_itpl_acc = std::transform_reduce(edge_itpl_value.begin(), edge_itpl_value.end(), 0.0, std::plus{}, [i](auto val) { return val[i]; });
        double vertex_corr_acc = std::transform_reduce(vertex_corr_val.begin(), vertex_corr_val.end(), 0.0, std::plus{}, [i](auto val) { return val[i]; });
        result[i] = edge_itpl_acc - vertex_corr_acc;
    }

    return result;
}

std::array<double, 3> MeshTransfiniteInterpolationAlgorithm::map1DLogicalToRealCoordinate(std::vector<std::array<double, 3>> real_vertex_coordinate_list, std::optional<double> x1, std::optional<double> x2, std::optional<double> x3) {
    std::array<double, 3> result;

    // Vertex interpolation
    std::vector<std::array<double, 3>> vertex_itpl_value;
    vertex_itpl_value.resize(2);

    vertex_itpl_value[0] = {
        (x1.has_value() ? (1 - x1.value()) : 0.5) * real_vertex_coordinate_list[0][0],
        (x2.has_value() ? (1 - x2.value()) : 0.5) * real_vertex_coordinate_list[0][1],
        (x3.has_value() ? (1 - x3.value()) : 0.5) * real_vertex_coordinate_list[0][2]
    };
    vertex_itpl_value[1] = {
        (x1.has_value() ? x1.value() : 0.5) * real_vertex_coordinate_list[1][0],
        (x2.has_value() ? x2.value() : 0.5) * real_vertex_coordinate_list[1][1],
        (x3.has_value() ? x3.value() : 0.5) * real_vertex_coordinate_list[1][2]
    };

    // Accumulation
    for (unsigned int i = 0; i < 3; i++) {
        result[i] = (vertex_itpl_value[0][i] + vertex_itpl_value[1][i]);
    }

    return result;
}