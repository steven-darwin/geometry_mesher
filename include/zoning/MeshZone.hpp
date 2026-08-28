/**
 * @file MeshZone.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2026-08-03
 * @date Last Modified : 2026-08-10
 *
 * @brief [Header] Mediator class for mesh zoning process.
 */

#ifndef MESH_ZONE_HPP
#define MESH_ZONE_HPP

#include <tuple>
#include <vector>
#include <optional>
#include <string>

#include "geometry-topology/GeometryTopology.hpp"

class MeshZone {
public:
    enum ZoneOption { POINT, LINE, PLANE, MAX };

    static MeshZone& instance();

    MeshZone(const MeshZone&) = delete;
    MeshZone& operator=(const MeshZone&) = delete;

    void addZoneMetadata();

    void setup();
    void run();

private:
    MeshZone() = default;

    std::unordered_map<std::string, std::array<std::optional<unsigned int>, 3>> _zoneMetadataList;

};

#endif