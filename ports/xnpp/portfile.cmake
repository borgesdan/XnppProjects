vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO borgesdan/xnpp
    REF v0.4.3
    SHA512 dad394b96ce254a251993f4f9030c982d74128421f5bd3bb77f89c41c85f346f6dd84bda631f12c0c8af223128433cd43109e1b275caf22ef3a70725a03acdb2
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