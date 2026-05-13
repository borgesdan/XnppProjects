#pragma once

#include "Animation.hpp"
#include <memory>
#include <Xna/Framework.hpp>
#include <Xna/Graphics.hpp>

namespace Platformer {
	struct AnimationPlayer {
	public:
		inline std::shared_ptr<Platformer::Animation> Animation() const { return animation; }
		constexpr int FrameIndex() const { return frameIndex; }
		inline Xna::Vector2 Origin() const {
			return Xna::Vector2(animation->FrameWidth() / 2.0f, animation->FrameHeight());
		}

		void PlayAnimation(std::shared_ptr<Platformer::Animation> const& animation);
		
		void Draw(Xna::GameTime& gameTime, Xna::SpriteBatch& spriteBatch, Xna::Vector2 const& position, Xna::SpriteEffects const& spriteEffects);

	private:
		std::shared_ptr<Platformer::Animation> animation;
		int frameIndex{ 0 };
		float time{0};
	};
}