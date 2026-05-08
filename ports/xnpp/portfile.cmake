vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO borgesdan/xnpp
    REF v0.5.8
    SHA512 0ce4997ff6e52856902d75567f0f6a8bb0d419b2e39d7cbe32530d70d728082667c90c79b7bcf06679b0c454df970bba4e39135ba2e6fbf7914d82831aeced28
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