vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO borgesdan/xnpp
    REF v0.4.0
    SHA512 8d10a2a4f471b13086a4c9825d61d8e82bf87d7fad0bc9d4906be19ae8420a2429096eaca3383d1cf493b2f67da300643b98583cea743ee66a5b161ca89f8614
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