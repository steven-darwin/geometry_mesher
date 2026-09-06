/**
 * @file MeshReport.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2026-08-17
 * @date Last Modified : 2026-08-17
 *
 * @brief [Header] Mediator class for mesh report creation.
 */

#ifndef MESH_REPORT_HPP
#define MESH_REPORT_HPP

#include <string>
#include <unordered_map>
#include <array>
#include <vector>
#include <tuple>
#include <chrono>

#include "geometry-topology/GeometryTopology.hpp"
#include "zoning/MeshZone.hpp"

class MeshReport {
public:
    struct WholeMeshSummary {
        std::vector<std::tuple<std::array<double, 3>, std::array<double, 3>>> coordinate_mapping;
        std::unordered_map<GeometryTopology::Type, unsigned int> entity_count;
    };

    struct ZoneSummaryItem {
        std::string type;
        std::unordered_map<GeometryTopology::Type, unsigned int> entity_count;
    };

    static MeshReport& instance();

    MeshReport(const MeshReport&) = delete;
    MeshReport& operator=(const MeshReport&) = delete;

    WholeMeshSummary& getWholeMeshSummary();

    ZoneSummaryItem& getZoneSummaryItem(std::string zone_name);
    void setZoneSummaryItem(std::string zone_name, ZoneSummaryItem data);

    void addTimePoint(std::string label, std::chrono::time_point<std::chrono::system_clock> timestamp);
    void addFileSuffix(std::string type, std::string file_suffix, std::string extension);

    void exportData();

private:
    /** Constructor of MeshReport object */
    MeshReport() = default;

    std::unordered_map<std::string, ZoneSummaryItem> _zoneSummaryData;
    WholeMeshSummary _wholeMeshSummaryData;
    std::vector<std::tuple<std::string, std::chrono::time_point<std::chrono::system_clock>>> _timePointList;
    std::vector<std::tuple<std::string, std::string, std::string>> _fileSuffixList;

    std::string paddingString(std::string text, std::string padding_char, unsigned int line_width, std::string align_direction);
};

#endif