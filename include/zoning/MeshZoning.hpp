/**
 * @file MeshZoning.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2026-08-03
 * @date Last Modified : 2026-08-10
 *
 * @brief [Header] Mediator class for mesh zoning process.
 */

#ifndef MESH_ZONING_HPP
#define MESH_ZONING_HPP

#include <tuple>
#include <vector>
#include <optional>
#include <string>

#include "geometry-topology/GeometryTopology.hpp"

class MeshZoning {
public:
    enum ZoneOption { POINT, LINE, PLANE, MAX };

    /** Constructor of MeshZoning object
     */
    MeshZoning() = default;

    /** Destructor of MeshZoning object */
    ~MeshZoning() = default;

    void addZoneMetadata();

    void setupPhase();
    void executionPhase();

private:
    const char* _runtimeConfigFilePath;
    std::unordered_map<std::string, std::array<std::optional<unsigned int>, 3>> _zoneMetadataList;

};

#endif