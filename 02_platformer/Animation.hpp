#pragma once

#include <Xna/Graphics.hpp>

namespace Platformer {
	class Animation {
	public:		
		Animation(Xna::Texture2D const& texture, float frameTime, bool isLooping)
			: texture(texture), frameTime(frameTime), isLooping(isLooping){ }

		inline Xna::Texture2D Texture() const { return texture; }
		constexpr float FrameTime() const { return frameTime; }
		constexpr bool IsLooping() const { return isLooping; }
		inline int FrameCount() const { return texture.Width() / FrameWidth(); }
		inline int FrameWidth() const { return texture.Height(); }
		inline int FrameHeight() const { return texture.Height(); }

	private:
		Xna::Texture2D texture{ nullptr };
		float frameTime{ 0 };
		bool isLooping{ false };
	};
}