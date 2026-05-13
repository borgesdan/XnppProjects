#pragma once

#include <Xna/Framework.hpp>

namespace Platformer {
    struct RectangleExtensions {
        static Xna::Vector2 GetIntersectionDepth(Xna::Rectangle const& rectA, Xna::Rectangle const& rectB) {
            const auto halfWidthA = rectA.Width / 2.0f;
            const auto halfHeightA = rectA.Height / 2.0f;
            const auto halfWidthB = rectB.Width / 2.0f;
            const auto halfHeightB = rectB.Height / 2.0f;

            const auto centerA = Xna::Vector2(rectA.Left() + halfWidthA, rectA.Top() + halfHeightA);
            const auto centerB = Xna::Vector2(rectB.Left() + halfWidthB, rectB.Top() + halfHeightB);

            const auto distanceX = centerA.X - centerB.X;
            const auto distanceY = centerA.Y - centerB.Y;
            const auto minDistanceX = halfWidthA + halfWidthB;
            const auto minDistanceY = halfHeightA + halfHeightB;

            if (std::abs(distanceX) >= minDistanceX || std::abs(distanceY) >= minDistanceY)
                return Xna::Vector2::Zero();

            const auto depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
            const auto depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
            return Xna::Vector2(depthX, depthY);
        }

        static constexpr Xna::Vector2 GetBottomCenter(Xna::Rectangle const& rect) {
            return Xna::Vector2(rect.X + rect.Width / 2.0f, rect.Bottom());
        }
    };
}