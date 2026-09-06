# Geometry Mesher

## Overview

Geometry Mesher is the first module in Scientific Computing - Simulation Pipeline that will be executed when the pipeline runs. This module is designed to encapsulate all mesh-related logic into a standalone package, so it can be executed independently from the other modules. As a meshing module, it has an ability to consume CAD files, which are then processed into an internal data structure within the module. In the subsequent step, a meshing algorithm will be applied to the the processed CAD data until the resulting mesh is generated. Lastly, as the final step, the resulting mesh is going to be serialized into files, which then can also be passed to downstream modules for further processing. 

## Implemented Strategies

- [Transfinite Interpolation](https://adjoining-sing-847.notion.site/Transfinite-Interpolation-3ae4793fb41a8043bce5eaa43bbcdd9e)

    Transfinite Interpolation is a discretization technique that can be used to generate a structured mesh. In summary, the physical coordinate of the geometry to be discretized are mapped to a logical coordinates system, whose values are normalized from 0 to 1 on each coordinate axis. The geometry is then discretized within this logical domain, and the resulting points are mapped back to the physical coordinate system, finalizing the meshing process.

## Features

- [Zone Creation](https://adjoining-sing-847.notion.site/Zone-Creation-3b04793fb41a80bb953fc63e80423f19)

    When creating a mesh, there are often specific areas with a higher interest. To facilitate the identification of these areas, this module provides a zone creation feature that allows users to define regions of the mesh that are of interest and may require further configuration in downstream processes. Until today, there are three configurations that can be applied to the created zones: initial conditions, boundary conditions, and visualization settings. 

- [Mesh Summary Export](https://adjoining-sing-847.notion.site/Execution-Report-3ae4793fb41a80b8a893d3c1485269d6)

    Since mesh data can be highly complex, analyzing the raw data to obtain a summary of the resulting mesh can be a complicated process. To provide an easy-to-digest summary of the mesh, this module autmatically performs the summarization process and exports the resulting information to the execution report. In the report, there will information such as the number of each geometry-topology entity (vertex, edge, face, and cell), along with additional information pertained to the selected meshing strategy.

## Limitations

- Currently, the mesher only accepts geometry models in the shape of a cube.
- This module only accepts STEP files as input. However, the architecture of this module has been designed to support additional input file formats in the future.
- As for now, this module always stores the resulting mesh in XDMF/HDF5 file format. However, similar to the input, the designed module's architecture allows the output capabilities to be extended to support other file formats in the future.

## Building the Module

- Dependencies

    - hdf5 (c++ library, managed by shared lib vcpkg)
    - gtest (c++ library, managed by shared lib vcpkg)
    - libxml2 (c++ library, managed by shared lib vcpkg)
    - nlohmann-json (c++ library, managed by shared lib vcpkg)
    - xdmf 3.0.0 (c++ library, built manually using cmake in shared lib)
    - occt 7.9.1 (c++ library, built manually using cmake in local lib)

- How to Build?

    a. Have cmake and ninja build system
    b. Add CMakeUserPresets.json if it is needed
    c. Add VCPKG_ROOT as env variable and set it to the root of your vcpkg directory
    d. Generate the project using cmake
    e. Build the executable using ninja

## Technical Notes

- [Requirements](https://adjoining-sing-847.notion.site/Requirements-2324793fb41a8062bffdc728d775d1df)
- [Architecture and Workflow](https://adjoining-sing-847.notion.site/Architecture-and-Workflow-3ae4793fb41a803ab4bddff689595043)
- Future Plan
    - [Delaunay Triangulation](https://adjoining-sing-847.notion.site/Delaunay-Triangulation-3ae4793fb41a80fc832efbf20b17470a)
