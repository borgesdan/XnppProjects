#pragma once

#include <Xna/Graphics.hpp>

namespace Platformer {    
    enum class TileCollision {
        Passable = 0,
        Impassable = 1,
        Platform = 2,
    };

    struct Tile
    {
        static constexpr int Width = 40;
        static constexpr int Height = 32;
        static constexpr Xna::Vector2 Size = Xna::Vector2(40, 32);

        inline Tile() = default;
        inline Tile(Xna::Texture2D const& texture, TileCollision collision)
            : Texture(texture), Collision(collision) {
        }

        Xna::Texture2D Texture{ nullptr };
        TileCollision Collision{};
    };
}