/**
 * @file MeshCore.hpp
 * @author Steven Darwin
 * @version 0.0.1
 * @date Created : 2025-07-28
 * @date Last Modified : 2025-09-27
 *
 * @brief [Header] Mediator class for mesh core process.
 */

#ifndef MESH_CORE_HPP
#define MESH_CORE_HPP

#include <memory>

#include <general/Generic.hpp>
#include <input-output/InputAdapter.hpp>
#include <input-output/OutputAdapter.hpp>

#include "core/MeshInputData.hpp"
#include "core/MeshAlgorithm.hpp"
#include "core/MeshOutputData.hpp"

class MeshCore {
public:
    static MeshCore& instance();

    MeshCore(const MeshCore&) = delete;
    MeshCore& operator=(const MeshCore&) = delete;

    /** Method to set up i/o adapter objects */
    void setup();

    /** Method to set mesh strategy */
    void run();

    std::shared_ptr<InputOutputAdapter> getIOAdapter(std::string type);

private:
    MeshCore() = default;

    std::shared_ptr<InputAdapter> _inputAdapter;
    std::shared_ptr<OutputAdapter> _outputAdapter;
};

#endif