/**
 * @file MeshZoning.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2026-08-03
 * @date Last Modified : 2026-08-03
 *
 * @brief [Implementation] Mediator class for mesh zoning process.
 */

#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "geometry-topology/GeometryTopologyVertex.hpp"
#include "geometry-topology/GeometryTopologyWire.hpp"
#include "geometry-topology/GeometryTopologyShell.hpp"
#include "input-output/OutputXDMFAdapter.hpp"
#include "input-output/InputHDF5Adapter.hpp"
#include "utility/ConfigReader.hpp"

#include "zoning/MeshZoning.hpp"
#include "report/MeshReport.hpp"

void MeshZoning::setupPhase() {
    bool setup_zone = true;

    while (setup_zone) {
        std::string is_zone_being_added;

        std::cout << "Adding Zone? (Y/N)" << std::endl;
        std::cin >> is_zone_being_added;
        if (is_zone_being_added == "Y") {
            addZoneMetadata();
        }
        else if (is_zone_being_added == "N") {
            break;
        }
        else {
            continue;
        }
    }
}

void MeshZoning::executionPhase() {
    MeshReport::instance().addTimePoint("zoning_begin", std::chrono::system_clock::now());

    std::string mesh_strategy = ConfigReader::instance().getRuntimeConfigValue("mesh", "strategy");
    std::vector<std::shared_ptr<GeometryTopology>> neutral_geometry_topology_list;

    std::string existing_hdf5_file_path =
        ConfigReader::instance().getRuntimeConfigValue("scmp", "staging_directory_path") +
        "/" +
        ConfigReader::instance().getRuntimeConfigValue("scmp", "file_name_prefix") +
        "." +
        "original.mesh" +
        ".h5";

    char* hdf5_buffer = new char[existing_hdf5_file_path.length() + 1];
    std::memcpy(hdf5_buffer, existing_hdf5_file_path.c_str(), existing_hdf5_file_path.length());
    hdf5_buffer[existing_hdf5_file_path.length()] = '\0';

    OutputXDMFAdapter internal_output_xdmf_adapter("extended.mesh");
    OutputXDMFAdapter::ParameterMetadata computational_grid_parameter = {
            "computational_grid",
            {1, 3},
            GeometryTopology::Type::VERTEX
    };
    internal_output_xdmf_adapter.addSolverParameter({ computational_grid_parameter });
    
    json zone_full_json;
    std::vector<json> zone_data;

    if (mesh_strategy == "transfinite_interpolation") {
        const std::vector<std::string> zone_option = { "plane", "line", "point" };
        InputHDF5Adapter internal_input_hdf5_adapter("original.mesh");
        std::shared_ptr<GeometryTopology> original_mesh = internal_input_hdf5_adapter.deserialize()[0];

        json whole_mesh_zone_item;
        whole_mesh_zone_item["name"] = "whole_mesh";
        whole_mesh_zone_item["entity_id"] = original_mesh->getID();
        whole_mesh_zone_item["type"] = "whole";
        zone_data.push_back(whole_mesh_zone_item);

        for (auto zone_metadata_iter = _zoneMetadataList.begin(); zone_metadata_iter != _zoneMetadataList.end(); zone_metadata_iter++) {
            unsigned int element_idx_counter = 0;
            for (auto element_idx_iter = 0; element_idx_iter < 3; element_idx_iter++) {
                if ((*zone_metadata_iter).second[element_idx_iter].has_value()) {
                    element_idx_counter++;
                }
            }

            hid_t file = H5Fopen(hdf5_buffer, H5F_ACC_RDONLY, H5P_DEFAULT);
            hid_t solver_group = H5Gopen(file, "solver", H5P_DEFAULT);

            struct parameter_buffer_struct {
                std::array<uint8_t, 16> entity_id;
                int element_index;
                double value;
            };
            std::vector<parameter_buffer_struct> parameter_buffer;

            hsize_t uuid_dim[2] = { 1, 16 };
            hid_t uuid_datatype = H5Tarray_create2(H5Tcopy(H5T_NATIVE_B8), 2, uuid_dim);
            hid_t parameter_datatype = H5Tcreate(H5T_COMPOUND, sizeof(parameter_buffer_struct));
            H5Tinsert(parameter_datatype, "entity_id", HOFFSET(parameter_buffer_struct, entity_id), uuid_datatype);
            H5Tinsert(parameter_datatype, "element_index", HOFFSET(parameter_buffer_struct, element_index), H5T_NATIVE_INT);
            H5Tinsert(parameter_datatype, "value", HOFFSET(parameter_buffer_struct, value), H5T_NATIVE_DOUBLE);
            hid_t memory_datatype = H5Tcopy(parameter_datatype);

            hid_t parameter_dataset = H5Dopen(solver_group, "computational_grid", H5P_DEFAULT);
            hid_t parameter_dataspace = H5Dget_space(parameter_dataset);

            hsize_t parameter_dataspace_dim[1];
            H5Sget_simple_extent_dims(parameter_dataspace, parameter_dataspace_dim, nullptr);
            parameter_buffer.resize(parameter_dataspace_dim[0]);

            const hsize_t parameter_start[1] = { 0 };
            const hsize_t parameter_stride[1] = { 1 };
            const hsize_t parameter_count[1] = { parameter_dataspace_dim[0] };
            const hsize_t parameter_block[1] = { 1 };
            H5Sselect_hyperslab(parameter_dataspace, H5S_SELECT_SET, parameter_start, parameter_stride, parameter_count, parameter_block);

            const hsize_t memory_dataspace_dim[1] = { parameter_buffer.size() };
            hid_t memory_dataspace = H5Screate_simple(1, memory_dataspace_dim, nullptr);

            const hsize_t memory_start[1] = { 0 };
            const hsize_t memory_stride[1] = { 1 };
            const hsize_t memory_count[1] = { parameter_dataspace_dim[0] };
            const hsize_t memory_block[1] = { 1 };
            H5Sselect_hyperslab(memory_dataspace, H5S_SELECT_SET, memory_start, memory_stride, memory_count, memory_block);

            if (H5Dread(parameter_dataset, memory_datatype, memory_dataspace, parameter_dataspace, H5P_DEFAULT, parameter_buffer.data()) < 0) {
                std::cout << "Parameter (" << "computational_grid" << ") Dataset fetching is failed." << std::endl;
            }
            else {
                std::cout << "Parameter (" << "computational_grid" << ") Dataset fetching is success." << std::endl;
            }

            std::unordered_map<std::array<uint8_t, 16>, std::array<double, 3>, InputHDF5Adapter::UUIDHash> computational_grid_data;
            for (auto parameter_iter = parameter_buffer.begin(); parameter_iter != parameter_buffer.end(); parameter_iter++) {
                std::array<double, 3> dummy_array;

                if (!computational_grid_data.contains(parameter_iter->entity_id)) {
                    computational_grid_data.insert({ parameter_iter->entity_id, dummy_array });
                } 

                computational_grid_data.at(parameter_iter->entity_id)[parameter_iter->element_index] = parameter_iter->value;
            }

            std::unordered_set<std::array<uint8_t, 16>, InputHDF5Adapter::UUIDHash> vertex_in_the_zone;

            for (auto computational_grid_iter = computational_grid_data.begin(); computational_grid_iter != computational_grid_data.end(); computational_grid_iter++) {
                unsigned int has_value_counter = 0;
                unsigned int equal_value_counter = 0;

                for (auto element_idx_iter = 0; element_idx_iter < 3; element_idx_iter++) {
                    if ((*zone_metadata_iter).second[element_idx_iter].has_value()) {
                        has_value_counter++;

                        if (std::abs(((*zone_metadata_iter).second[element_idx_iter].value() / std::stod(ConfigReader::instance().getRuntimeConfigValue("mesh", "transfinite_interpolation_segment_count"))) - (*computational_grid_iter).second[element_idx_iter]) < 1e-6) {
                            equal_value_counter++;
                        }
                    }
                }

                if (has_value_counter == equal_value_counter) {
                    vertex_in_the_zone.insert((*computational_grid_iter).first);
                }
            }

            if (zone_option[element_idx_counter - 1] == "plane") {
                std::shared_ptr<GeometryTopologyShell> plane_zone = std::make_shared<GeometryTopologyShell>();

                std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> face_list;
                original_mesh->getDescendants(face_list, GeometryTopology::Type::FACE);

                for (auto face_iter = face_list.begin(); face_iter != face_list.end(); face_iter++) {
                    std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> vertex_list;
                    (*face_iter).first->getDescendants(vertex_list, GeometryTopology::Type::VERTEX);

                    unsigned int included_vertex_counter = 0;
                    for (auto vertex_iter = vertex_list.begin(); vertex_iter != vertex_list.end(); vertex_iter++) {
                        if (vertex_in_the_zone.contains((*vertex_iter).first->getID())) {
                            included_vertex_counter++;
                        }
                    }

                    if (included_vertex_counter == vertex_list.size()) {
                        plane_zone->addFace(std::dynamic_pointer_cast<GeometryTopologyFace>((*face_iter).first), 0);
                    }
                }

                neutral_geometry_topology_list.push_back(plane_zone);
            }
            else if (zone_option[element_idx_counter - 1] == "line") {
                std::shared_ptr<GeometryTopologyWire> line_zone = std::make_shared<GeometryTopologyWire>();

                std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> edge_list;
                original_mesh->getDescendants(edge_list, GeometryTopology::Type::EDGE);

                for (auto edge_iter = edge_list.begin(); edge_iter != edge_list.end(); edge_iter++) {
                    std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> vertex_list;
                    (*edge_iter).first->getDescendants(vertex_list, GeometryTopology::Type::VERTEX);

                    unsigned int included_vertex_counter = 0;
                    for (auto vertex_iter = vertex_list.begin(); vertex_iter != vertex_list.end(); vertex_iter++) {
                        if (vertex_in_the_zone.contains((*vertex_iter).first->getID())) {
                            included_vertex_counter++;
                        }
                    }

                    if (included_vertex_counter == vertex_list.size()) {
                        line_zone->addEdge(std::dynamic_pointer_cast<GeometryTopologyEdge>((*edge_iter).first), 0);
                    }
                }

                neutral_geometry_topology_list.push_back(line_zone);
            }
            else if (zone_option[element_idx_counter - 1] == "point") {
                std::shared_ptr<GeometryTopologyVertex> point_zone;

                std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> vertex_list;
                original_mesh->getDescendants(vertex_list, GeometryTopology::Type::FACE);

                for (auto vertex_iter = vertex_list.begin(); vertex_iter != vertex_list.end(); vertex_iter++) {
                    if (vertex_in_the_zone.contains((*vertex_iter).first->getID())) {
                        point_zone = std::dynamic_pointer_cast<GeometryTopologyVertex>((*vertex_iter).first);
                    }
                }

                neutral_geometry_topology_list.push_back(point_zone);
            }
            else {
                // do nothing
            }

            json zone_item;

            MeshReport::ZoneSummaryItem zone_summary;

            zone_item["name"] = (*zone_metadata_iter).first;

            zone_item["entity_id"] = neutral_geometry_topology_list[neutral_geometry_topology_list.size() - 1]->getID();

            std::unordered_map<std::shared_ptr<GeometryTopology>, unsigned int> entity_list;

            switch (neutral_geometry_topology_list[neutral_geometry_topology_list.size() - 1]->getType()) {
            case GeometryTopology::Type::SHELL:
                entity_list.clear();
                neutral_geometry_topology_list[neutral_geometry_topology_list.size() - 1]->getDescendants(entity_list, GeometryTopology::Type::FACE);
                zone_summary.entity_count.insert({ GeometryTopology::Type::FACE, entity_list.size() });
            case GeometryTopology::Type::WIRE:
                entity_list.clear();
                neutral_geometry_topology_list[neutral_geometry_topology_list.size() - 1]->getDescendants(entity_list, GeometryTopology::Type::EDGE);
                zone_summary.entity_count.insert({ GeometryTopology::Type::EDGE, entity_list.size() });
            case GeometryTopology::Type::VERTEX:
                entity_list.clear();
                neutral_geometry_topology_list[neutral_geometry_topology_list.size() - 1]->getDescendants(entity_list, GeometryTopology::Type::VERTEX);
                zone_summary.entity_count.insert({ GeometryTopology::Type::VERTEX, entity_list.size() });
            }

            switch (neutral_geometry_topology_list[neutral_geometry_topology_list.size() - 1]->getType()) {
            case GeometryTopology::Type::SHELL:
                zone_item["type"] = zone_summary.type = "plane";
                break;
            case GeometryTopology::Type::WIRE:
                zone_item["type"] = zone_summary.type = "line";
                break;
            case GeometryTopology::Type::VERTEX:
                zone_item["type"] = zone_summary.type = "point";
                break;
            }

            MeshReport::instance().setZoneSummaryItem((*zone_metadata_iter).first, zone_summary);

            std::vector<json> zone_coordinate_list;

            for (auto element_idx_iter = 0; element_idx_iter < 3; element_idx_iter++) {
                json zone_coordinate;

                if ((*zone_metadata_iter).second[element_idx_iter].has_value()) {
                    switch (element_idx_iter) {
                    case 0:
                        zone_coordinate["axis"] = "X";
                        break;
                    case 1:
                        zone_coordinate["axis"] = "Y";
                        break;
                    case 2:
                        zone_coordinate["axis"] = "Z";
                        break;
                    }

                    zone_coordinate["logical_value"] = (*zone_metadata_iter).second[element_idx_iter].value() / std::stod(ConfigReader::instance().getRuntimeConfigValue("mesh", "transfinite_interpolation_segment_count"));
                    
                    zone_coordinate_list.push_back(zone_coordinate);
                }
            }

            zone_item["coordinate"] = zone_coordinate_list;

            zone_data.push_back(zone_item);
        }
    }
    
    internal_output_xdmf_adapter.appendZoneCreationData(neutral_geometry_topology_list, hdf5_buffer);
    MeshReport::instance().addFileSuffix("out", "extended.mesh", "xmf");
    MeshReport::instance().addFileSuffix("out", "extended.mesh", "xmf");

    MeshReport::instance().addTimePoint("extended_mesh_out", std::chrono::system_clock::now());

    zone_full_json["zone"] = zone_data;

    std::string zone_json_file_path =
        ConfigReader::instance().getRuntimeConfigValue("scmp", "staging_directory_path") +
        "/" +
        ConfigReader::instance().getRuntimeConfigValue("scmp", "file_name_prefix") +
        "_" +
        "zone.json";

    char* zone_buffer = new char[zone_json_file_path.length() + 1];
    std::memcpy(zone_buffer, zone_json_file_path.c_str(), zone_json_file_path.length());
    zone_buffer[zone_json_file_path.length()] = '\0';

    std::ofstream zone_json_out(zone_buffer);
    zone_json_out << std::setw(4) << zone_full_json << std::endl;

    MeshReport::instance().addTimePoint("zoning_finish", std::chrono::system_clock::now());
}

void MeshZoning::addZoneMetadata() {
    std::string mesh_strategy = ConfigReader::instance().getRuntimeConfigValue("mesh", "strategy");

    if (mesh_strategy == "transfinite_interpolation") {
        const std::vector<std::string> zone_option_str = { "plane", "line", "point" };

        std::string selected_zone_option;
        std::string zone_name;

        std::cout << "-----------------------------------------------" << std::endl;

        std::cout << "Please select the zone option! (type the number)" << std::endl;
        for (unsigned int iter = 0; iter < MeshZoning::ZoneOption::MAX; iter++) {
            std::cout << (iter + 1) << ". " << zone_option_str[iter] << std::endl;
        }
        std::cout << "selected zone : ";
        std::cin >> selected_zone_option;

        std::cout << "-----------------------------------------------" << std::endl;

        std::cout << "Please input the zone name!" << std::endl;
        std::cout << "zone name : ";
        std::cin >> zone_name;

        std::cout << "-----------------------------------------------" << std::endl;
        
        std::array<std::optional<unsigned int>, 3> selected_element_idx_arr;

        std::cout << zone_option_str[std::stoi(selected_zone_option) - 1] << " zone setup: " << std::endl;
        for (unsigned int iter = 0; iter < std::stoi(selected_zone_option); iter++) {
            std::string selected_coordinate_axis;
            std::string selected_element_idx;

            std::cout << "--- coordinate " << (iter + 1) << " ---" << std::endl;
            std::cout << "coordinate axis (X/Y/Z): ";
            std::cin >> selected_coordinate_axis;
            std::cout << "element_index (0-" << ConfigReader::instance().getRuntimeConfigValue("mesh", "transfinite_interpolation_segment_count") << "): ";
            std::cin >> selected_element_idx;

            if (selected_coordinate_axis == "X") {
                selected_element_idx_arr[0] = std::stoi(selected_element_idx);
            }
            else if (selected_coordinate_axis == "Y") {
                selected_element_idx_arr[1] = std::stoi(selected_element_idx);
            }
            else if (selected_coordinate_axis == "Z") {
                selected_element_idx_arr[2] = std::stoi(selected_element_idx);
            }
            else {
                iter--;
            }
        }

        _zoneMetadataList.insert(
            {
                zone_name,
                selected_element_idx_arr
            }
        );

        std::cout << "-----------------------------------------------" << std::endl;
    }
}