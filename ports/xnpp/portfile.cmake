vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO borgesdan/xnpp
    REF v0.5.0
    SHA512 48fb5f9aa070dbc56dabc174daa5641535bac948765c9f5668eaf53f95f9d4a8411de740e48ee41ff2f1b1052c4b221008ca62e5e76edd217be405d8f1d4e50b
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