/**
 * @file MeshReport.cpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2026-08-17
 * @date Last Modified : 2026-08-17
 *
 * @brief [Implementation] Mediator class for mesh report creation.
 */

#include <fstream>

#include "utility/ConfigReader.hpp"
#include "report/MeshReport.hpp"

MeshReport& MeshReport::instance() {
    static MeshReport singleton;
    return singleton;
}

MeshReport::WholeMeshSummary& MeshReport::getWholeMeshSummary() {
    return _wholeMeshSummaryData;
}

void MeshReport::setWholeMeshSummary(WholeMeshSummary data) {
    _wholeMeshSummaryData = data;
}

MeshReport::ZoneSummaryItem& MeshReport::getZoneSummaryItem(std::string zone_name) {
    return _zoneSummaryData.at(zone_name);
}
void MeshReport::setZoneSummaryItem(std::string zone_name, ZoneSummaryItem data) {
    _zoneSummaryData.insert({ zone_name, data });
}

void MeshReport::addTimePoint(std::string label, std::chrono::time_point<std::chrono::system_clock> timestamp) {
    _timePointList.push_back({ label, timestamp });
}

void MeshReport::addFileSuffix(std::string type, std::string file_suffix, std::string extension) {
    _fileSuffixList.push_back({ type, file_suffix, extension });
}

void MeshReport::exportData() {
    std::ofstream report_file(
        ConfigReader::instance().getRuntimeConfigValue("scmp", "staging_directory_path") + "/" + ConfigReader::instance().getRuntimeConfigValue("scmp", "file_name_prefix") + "_report.txt",
        std::ios::app
    );

    std::ostringstream mesh_data_in_str;

    // Header
    mesh_data_in_str << paddingCenteredString("", "=", 8) << "\n";
    mesh_data_in_str << paddingCenteredString("MESHER", " ", 8) << "\n";
    mesh_data_in_str << paddingCenteredString("", "=", 8) << "\n";
    mesh_data_in_str << "\n";

    // Configuration
    mesh_data_in_str << "> Configuration" << "\n";
    mesh_data_in_str << "\n";
    mesh_data_in_str << "strategy: " << ConfigReader::instance().getRuntimeConfigValue("mesh", "strategy") << "\n";
    mesh_data_in_str << "segment_count: " << ConfigReader::instance().getRuntimeConfigValue("mesh", "transfinite_interpolation_segment_count") << "\n";
    mesh_data_in_str << "\n";

    // Milestone
    mesh_data_in_str << "> Milestone" << "\n";
    mesh_data_in_str << "\n";

    for (auto iter = _timePointList.begin(); iter != _timePointList.end(); iter++) {
        mesh_data_in_str << std::get<0>(*iter) << " -> ";

        const std::time_t temp_time = std::chrono::system_clock::to_time_t(std::get<1>(*iter));
        mesh_data_in_str << std::put_time(std::gmtime(&temp_time), "%F %T");
        mesh_data_in_str << "." << std::chrono::duration_cast<std::chrono::milliseconds>(std::get<1>(*iter).time_since_epoch()).count() % 1000 << "\n";
    }

    mesh_data_in_str << "\n";

    // Whole Mesh
    mesh_data_in_str << "> Whole Mesh" << "\n";
    mesh_data_in_str << "\n";

    mesh_data_in_str << "logical -> real" << "\n";
    for (auto iter = _wholeMeshSummaryData.coordinate_mapping.begin(); iter != _wholeMeshSummaryData.coordinate_mapping.end(); iter++) {
        mesh_data_in_str << std::get<0>(*iter)[0] << " " << std::get<0>(*iter)[1] << " " << std::get<0>(*iter)[2];
        mesh_data_in_str << " -> ";
        mesh_data_in_str << std::get<1>(*iter)[0] << " " << std::get<1>(*iter)[1] << " " << std::get<1>(*iter)[2];
        mesh_data_in_str << "\n";
    }

    mesh_data_in_str << "\n";
    mesh_data_in_str << "vertex_count | edge_count | face_count | cell_count" << "\n";
    std::array<unsigned int, 4> entity_count_summary;
    for (auto iter = _wholeMeshSummaryData.entity_count.begin(); iter != _wholeMeshSummaryData.entity_count.end(); iter++) {
        switch ((*iter).first) {
        case GeometryTopology::Type::VERTEX:
            entity_count_summary[0] = (*iter).second;
            break;
        case GeometryTopology::Type::EDGE:
            entity_count_summary[1] = (*iter).second;
            break;
        case GeometryTopology::Type::FACE:
            entity_count_summary[2] = (*iter).second;
            break;
        case GeometryTopology::Type::CELL:
            entity_count_summary[3] = (*iter).second;
            break;
        }
    }
    mesh_data_in_str << entity_count_summary[0] << " | " << entity_count_summary[1] << " | " << entity_count_summary[2] << " | " << entity_count_summary[3] << "\n";

    mesh_data_in_str << "\n";

    // Zone
    mesh_data_in_str << "> Zone" << "\n";
    mesh_data_in_str << "\n";

    mesh_data_in_str << "name | type | vertex_count | edge_count | face_count" << "\n";
    for (auto iter = _zoneSummaryData.begin(); iter != _zoneSummaryData.end(); iter++) {
        mesh_data_in_str << (*iter).first << " | " << (*iter).second.type << " | ";

        std::array<unsigned int, 3> entity_count_summary = { 0, 0, 0 };
        for (auto entity_count_iter = (*iter).second.entity_count.begin(); entity_count_iter != (*iter).second.entity_count.end(); entity_count_iter++) {
            switch ((*entity_count_iter).first) {
            case GeometryTopology::Type::VERTEX:
                entity_count_summary[0] = (*entity_count_iter).second;
                break;
            case GeometryTopology::Type::EDGE:
                entity_count_summary[1] = (*entity_count_iter).second;
                break;
            case GeometryTopology::Type::FACE:
                entity_count_summary[2] = (*entity_count_iter).second;
                break;
            }
        }
        mesh_data_in_str << entity_count_summary[0] << " | " << entity_count_summary[1] << " | " << entity_count_summary[2] << "\n";
    }

    mesh_data_in_str << "\n";

    // I/O Files
    mesh_data_in_str << "> I/O Files" << "\n";
    mesh_data_in_str << "\n";

    for (auto iter = _fileSuffixList.begin(); iter != _fileSuffixList.end(); iter++) {
        mesh_data_in_str << std::get<0>(*iter) << " | ";
        mesh_data_in_str << ConfigReader::instance().getRuntimeConfigValue("scmp", "file_name_prefix");
        if (std::get<1>(*iter) != "") mesh_data_in_str << "." + std::get<1>(*iter);
        mesh_data_in_str << "." + std::get<2>(*iter) << "\n";
    }

    mesh_data_in_str << "\n";

    report_file << mesh_data_in_str.str();
    report_file.close();
}

std::string MeshReport::paddingCenteredString(std::string text, std::string padding_char, unsigned int line_width) {
    if (text.length() >= line_width) {
        return text;
    }
    else {
        while (text.length() < line_width) {
            text = padding_char + text + padding_char;
        }

        return text;
    }
}