cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("GeometryMesher")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add source to this project's executable.
set(PROJECT_SOURCES
    geometry_mesher.cpp
    src/core/MeshCore.cpp
    src/core/MeshInputProcessor.cpp
    src/core/MeshOutputProcessor.cpp
    src/core/MeshInputData.cpp
    src/core/MeshAlgorithm.cpp
    src/core/MeshOutputData.cpp
    src/transfinite-interpolation/MeshTransfiniteInterpolationInputData.cpp
    src/transfinite-interpolation/MeshTransfiniteInterpolationAlgorithm.cpp
    src/transfinite-interpolation/MeshTransfiniteInterpolationOutputData.cpp
    src/delaunay/MeshDelaunayInputData.cpp
    src/delaunay/MeshDelaunayAlgorithm.cpp
    src/delaunay/MeshDelaunayOutputData.cpp
    src/data-structure/MeshGraphVertex.cpp
    src/data-structure/MeshGraphEdge.cpp
    ../../src/input-output/InputOutputAdapter.cpp
    ../../src/input-output/InputAdapter.cpp
    ../../src/input-output/InputSTEPAdapter.cpp
    ../../src/input-output/InputXDMFAdapter.cpp
    ../../src/input-output/InputHDF5Adapter.cpp
    ../../src/input-output/OutputAdapter.cpp
    ../../src/input-output/OutputXDMFAdapter.cpp
    ../../src/input-output/OutputHDF5Adapter.cpp
    ../../src/geometry-topology/GeometryTopology.cpp
    ../../src/geometry-topology/GeometryTopologyComposite.cpp
    ../../src/geometry-topology/GeometryTopologyCell.cpp
    ../../src/geometry-topology/GeometryTopologyShell.cpp
    ../../src/geometry-topology/GeometryTopologyFace.cpp
    ../../src/geometry-topology/GeometryTopologyWire.cpp
    ../../src/geometry-topology/GeometryTopologyEdge.cpp
    ../../src/geometry-topology/GeometryTopologyVertex.cpp
)

add_executable (${PROJECT_NAME} ${PROJECT_SOURCES}) 

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
	    include
        ../../include
)

# add shared library
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};../../shared_lib/vcpkg_installed/x64-windows")
find_package(GTest CONFIG REQUIRED)
find_package(hdf5 CONFIG REQUIRED)
find_package(LibXml2 REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        GTest::gtest 
        GTest::gtest_main 
        GTest::gmock 
        GTest::gmock_main
        hdf5::hdf5-shared 
        hdf5::hdf5_hl-shared
        LibXml2::LibXml2
)

target_link_directories(${PROJECT_NAME}
    PRIVATE
        ../../shared_lib/xdmf-3.0.0/lib/
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
	    ../../shared_lib/xdmf-3.0.0/include/
	    ../../shared_lib/xdmf-3.0.0/include/core/
	    ../../shared_lib/xdmf-3.0.0/include/core/loki/
	    ../../shared_lib/xdmf-3.0.0/include/core/dsm/
	    ../../shared_lib/xdmf-3.0.0/include/utils/
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Xdmf
        XdmfCore
)

# add local library
target_link_directories(${PROJECT_NAME}
    PRIVATE
         lib/occt-7.9.1-vc14-64/lib
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
         lib/occt-7.9.1-vc14-64/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        TKernel
        TKMath
        TKXSBase
        TKBRep
        TKG3d
        TKDESTEP
)

# file(COPY ${PROJECT_SOURCE_DIR}/test DESTINATION ${PROJECT_BINARY_DIR})
# file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/data)