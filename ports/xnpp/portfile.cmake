vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO borgesdan/xnpp
    REF v0.5.1
    SHA512 e210ef8e18bf77a032229703aabdb95262335d2d734f8e579300c0682f84c2ea17d4fb577249a3f30c77990e10a1ce1927e9dd138daa3523e2323d901ada062c
)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DXNPP_BUILD_LIB=ON
        -DXNPP_BUILD_CONTENTPIPELINE=OFF
        -DXNPP_BUILD_GAME_TEST=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    CONFIG_PATH lib/cmake/xnpp
)

vcpkg_install_copyright(
    FILE_LIST "${SOURCE_PATH}/LICENSE.md"
)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
