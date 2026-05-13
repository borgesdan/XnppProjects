add_executable (PlatformerGame  
    "Game1.cpp" 
    "AnimationPlayer.cpp"
    "Level.cpp"
    "Gem.cpp"
    "Enemy.cpp"
    "Player.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PlatformerGame PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
find_package(xnpp CONFIG REQUIRED)
target_link_libraries(PlatformerGame PRIVATE xnpp::xnpp)

xnpp_deploy_shaders(PlatformerGame)
xnpp_build_content(PlatformerGame)