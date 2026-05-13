#pragma once

#include "Animation.hpp"
#include "AnimationPlayer.hpp"
#include <memory>
#include <Xna/Framework.hpp>
#include <Xna/Graphics.hpp>
#include <Xna/Input.hpp>
#include <Xna/Audio.hpp>

namespace Platformer {
	class Level;
	class Enemy;

	class Player
	{
	public:
		Player(std::shared_ptr<Platformer::Level> const& level, Xna::Vector2 const& position);

		std::shared_ptr<Platformer::Level> Level();
		inline bool IsAlive() const { return isAlive; }
		Xna::Vector2 Position() const { return position; }
		void Position(Xna::Vector2 const& value) { position = value; }
		void SetPosition(Xna::Vector2 const& value) { position = value; }
		Xna::Vector2 Velocity() const { return velocity; }
		void Velocity(Xna::Vector2 const& value) { velocity = value; }
		inline bool IsOnGround() const { return isOnGround; }
		Xna::Rectangle BoundingRectangle() const;
		
		void LoadContent();		
		void Reset(Xna::Vector2 const& position);
		void Update(
			Xna::GameTime& gameTime,
			Xna::KeyboardState& keyboardState,
			Xna::GamePadState& gamePadState,
			Xna::DisplayOrientation orientation);
		void GetInput(
			Xna::KeyboardState keyboardState,
			Xna::GamePadState gamePadState,
			Xna::DisplayOrientation orientation);				
		void ApplyPhysics(Xna::GameTime& gameTime);
		float DoJump(float velocityY, Xna::GameTime& gameTime);

		void HandleCollisions();	
		void OnKilled(std::shared_ptr<Enemy>& killedBy);
		inline void OnReachedExit() { sprite.PlayAnimation(celebrateAnimation); }
		void Draw(Xna::GameTime& gameTime, Xna::SpriteBatch& spriteBatch);
	private:
		std::shared_ptr<Animation> idleAnimation;
		std::shared_ptr<Animation> runAnimation;
		std::shared_ptr<Animation> jumpAnimation;
		std::shared_ptr<Animation> celebrateAnimation;
		std::shared_ptr<Animation> dieAnimation;
		Xna::SpriteEffects flip{ Xna::SpriteEffects::None };
		AnimationPlayer sprite;
		Xna::SoundEffect killedSound{ nullptr };
		Xna::SoundEffect jumpSound{ nullptr };
		Xna::SoundEffect fallSound{ nullptr };
		std::shared_ptr<Platformer::Level> level;
		bool isAlive{ false };
		float previousBottom{ 0 };
		Xna::Vector2 position{ 0 };
		Xna::Vector2 velocity{ 0 };
		bool isOnGround{ false };
		float movement{ 0 };
		bool isJumping{ false };
		bool wasJumping{ false };
		float jumpTime{ 0 };
		Xna::Rectangle localBounds{};

		static constexpr float MoveAcceleration = 13000.0f;
		static constexpr float MaxMoveSpeed = 1750.0f;
		static constexpr float GroundDragFactor = 0.48f;
		static constexpr float AirDragFactor = 0.58f;
		static constexpr float MaxJumpTime = 0.35f;
		static constexpr float JumpLaunchVelocity = -3500.0f;
		static constexpr float GravityAcceleration = 3400.0f;
		static constexpr float MaxFallSpeed = 550.0f;
		static constexpr float JumpControlPower = 0.14f;
		static constexpr float MoveStickScale = 1.0f;
		static constexpr float AccelerometerScale = 1.5f;
		static constexpr Xna::Buttons JumpButton = Xna::Buttons::A;	
	};
}