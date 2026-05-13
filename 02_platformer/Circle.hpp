#pragma once

#include <Xna/Framework.hpp>

namespace Platformer {
	struct Circle {
		Xna::Vector2 Center;
		float Radius;

		constexpr Circle(Xna::Vector2 const& position, float radius)
			: Center(position), Radius(radius){ }

		constexpr bool Intersects(Xna::Rectangle const& rectangle)
		{
			const auto x = Xna::MathHelper::Clamp(static_cast<int32_t>(Center.X), rectangle.Left(), rectangle.Right());
			const auto y = Xna::MathHelper::Clamp(static_cast<int32_t>(Center.Y), rectangle.Top(), rectangle.Bottom());

			const auto v = Xna::Vector2(x,y);

			const auto direction = Center - v;
			const auto distanceSquared = direction.LengthSquared();

			return ((distanceSquared > 0) && (distanceSquared < Radius * Radius));
		}
	};	
}