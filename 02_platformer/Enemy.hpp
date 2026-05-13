#pragma once

#include <Xna/Framework.hpp>
#include <Xna/Graphics.hpp>
#include "Level.hpp"
#include "Animation.hpp"
#include "AnimationPlayer.hpp"
#include <memory>
#include <filesystem>

namespace Platformer {
	enum class FaceDirection
	{
		Left = -1,
		Right = 1,
	};

	class Enemy
	{
	public:
		inline std::shared_ptr<Platformer::Level> Level() const { return level; }
		constexpr Xna::Vector2 Position() const { return position; }
		Xna::Rectangle BoundingRectangle() const;

		Enemy(std::shared_ptr<Platformer::Level> const& level, Xna::Vector2 const& position, std::filesystem::path spriteSet);
		void LoadContent(std::filesystem::path spriteSet);
		void Update(Xna::GameTime& gameTime);		
		void Draw(Xna::GameTime& gameTime, Xna::SpriteBatch& spriteBatch);

	private:
		std::shared_ptr<Platformer::Level> level;
		Xna::Vector2 position{ 0 };
		Xna::Rectangle localBounds{};
		std::shared_ptr<Animation> runAnimation;
		std::shared_ptr<Animation> idleAnimation;
		AnimationPlayer sprite;
		FaceDirection direction{ FaceDirection::Left };
		float waitTime{ 0 };

		static constexpr float MaxWaitTime = 0.5f;
		static constexpr float MoveSpeed = 64.0f;
	};
}