/**
 * @file MeshTransfiniteInterpolationOutputData.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-10-05
 *
 * @brief [Header] Concrete class to handle Transfinite Interpolation algorithm output data.
 */

#ifndef MESH_TRANSFINITE_INTERPOLATION_OUTPUT_DATA_HPP
#define MESH_TRANSFINITE_INTERPOLATION_OUTPUT_DATA_HPP

#include <vector>
#include <memory>
#include <unordered_set>

#include "geometry-topology/GeometryTopology.hpp"
#include "geometry-topology/GeometryTopologyComposite.hpp"

#include "core/MeshOutputData.hpp"

#include "data-structure/MeshGraphEdge.hpp"

class MeshTransfiniteInterpolationOutputData : public MeshOutputData {
public:
    /** Constructor of MeshTransfiniteInterpolationOutputData object
     */
    MeshTransfiniteInterpolationOutputData();

    /** Destructor of MeshTransfiniteInterpolationOutputData object */
    ~MeshTransfiniteInterpolationOutputData();

    /** Method to transform mesh strategy-specific geometry-topology data structure to neutral geometry-topology data structure */
    std::shared_ptr<GeometryTopology> transform();

    /** Method to insert mesh graph edge */
    void insertMeshGraphEdge(std::shared_ptr<MeshGraphEdge> mesh_graph_edge);

private:
    /** Method to calculate combination of element in a set */
    double getCombinationCount(unsigned int set_element_count, unsigned int sample_element_count);

    /** Method to get unique ID for particular combination of element in a set */
    unsigned long long getCombinationID(std::vector<unsigned int> element_id_list);

    /** Method to get factorial result of a number */
    double calcFactorial(unsigned int num);

    /** Method to get factorial result of a number */
    double calcFactorial(unsigned int upper_bound, unsigned int lower_bound);

    /** Method to construct geometry-topology face from cycled graph's edge */
    void constructFaceFromCycledGraphEdge(std::unordered_map<std::shared_ptr<MeshGraphEdge>, std::pair<unsigned int, std::shared_ptr<GeometryTopologyEdge>>>& edge_list, std::unordered_map<std::shared_ptr<GeometryTopologyFace>, std::pair<unsigned int, std::shared_ptr<MeshGraphVertex>>>& face_list, std::vector<std::vector<std::shared_ptr<GeometryTopologyFace>>>& incident_face_on_edge, std::shared_ptr<MeshGraphEdge> next_edge, std::shared_ptr<MeshGraphVertex> prev_vertex, std::unordered_set<std::shared_ptr<MeshGraphEdge>> traversed_edges, std::unordered_set<unsigned long long>& face_list_idx);

    /** Method to construct geometry-topology face from cycled graph's vertex */
    void constructFaceFromCycledGraphVertex(std::unordered_map<std::shared_ptr<MeshGraphVertex>, unsigned int>& graph_vertex_list, std::unordered_map<std::shared_ptr<GeometryTopologyFace>, std::shared_ptr<MeshGraphVertex>>& face_list, std::shared_ptr<MeshGraphVertex> next_vertex, std::unordered_set<std::shared_ptr<MeshGraphVertex>> traversed_vertices, std::unordered_set<unsigned long long>& face_list_idx);

    /** Method to construct geometry-topology cell from cycled graph's edge */
    void constructCellFromCycledGraphEdge(std::unordered_map<std::shared_ptr<MeshGraphEdge>, unsigned int>& edge_list, std::unordered_map<std::shared_ptr<GeometryTopologyCell>, unsigned int>& cell_list, std::shared_ptr<MeshGraphEdge> next_edge, std::shared_ptr<MeshGraphVertex> prev_vertex, std::unordered_set<std::shared_ptr<MeshGraphEdge>> traversed_edges, std::unordered_set<std::shared_ptr<GeometryTopologyVertex>> traversed_geometry_topology_vertex, std::unordered_set<unsigned long long>& cell_list_idx);

    /** Method to construct geometry-topology cell from cycled graph's vertex */
    void constructCellFromCycledGraphVertex(std::unordered_map<std::shared_ptr<MeshGraphVertex>, unsigned int>& graph_vertex_list, std::unordered_map<std::shared_ptr<GeometryTopologyCell>, unsigned int>& cell_list, std::shared_ptr<MeshGraphVertex> next_vertex, std::unordered_set<std::shared_ptr<MeshGraphVertex>> traversed_vertices, std::unordered_set<unsigned long long>& cell_list_idx);

    /** Attribute to store the list of mesh graph edge */
    std::vector<std::shared_ptr<MeshGraphEdge>> _meshGraphEdgeList;

    /** Attribute to store the list of neutral geometry-topology cell */
    std::shared_ptr<GeometryTopologyComposite> _composite;

};

#endif