vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO borgesdan/xnpp
    REF v0.5.9
    SHA512 5f52348786282e9267a6533abb3efee515364437d3ba240d405013d1629e0a8f7b990b11b5c7bfa6004289099ae2683813ced74a9ea0013a66d1cafc0d25ded2
)

set(BUILD_CONTENTPIPELINE OFF)

if("content-pipeline" IN_LIST FEATURES)
    set(BUILD_CONTENTPIPELINE ON)
endif()

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DXNPP_BUILD_LIB=ON
        -DXNPP_BUILD_CONTENT_PIPELINE=${BUILD_CONTENTPIPELINE}
        -DXNPP_BUILD_CONTENT_CLI=${BUILD_CONTENTPIPELINE}
        -DXNPP_BUILD_GAME_TEST=OFF        
)

vcpkg_cmake_install()

if(BUILD_CONTENTPIPELINE)
    vcpkg_copy_tools(
        TOOL_NAMES xnpp-content-cli
        AUTO_CLEAN
    )
endif()

vcpkg_cmake_config_fixup(
    CONFIG_PATH lib/cmake/xnpp
)

vcpkg_install_copyright(
    FILE_LIST "${SOURCE_PATH}/LICENSE.md"
)

file(REMOVE_RECURSE 
    "${CURRENT_PACKAGES_DIR}/debug/include"
    "${CURRENT_PACKAGES_DIR}/debug/share"
)