#include "Player.hpp"
#include <cstdint>
#include "Level.hpp"
#include "Enemy.hpp"
#include "RectangleExtensions.hpp"

using namespace Xna;

namespace Platformer {
	Level::Level(){}

	std::shared_ptr<Platformer::Level> Player::Level() {
		return level;
	}

	Xna::Rectangle Player::BoundingRectangle() const {
		const auto left = static_cast<int>(std::round(Position().X - sprite.Origin().X)) + localBounds.X;
		const auto top =  static_cast<int>(std::round(Position().Y - sprite.Origin().Y)) + localBounds.Y;

		return Xna::Rectangle(left, top, localBounds.Width, localBounds.Height);
	}

	Player::Player(std::shared_ptr<Platformer::Level> const& level, Xna::Vector2 const& position)
		:level(level)
	{		
		LoadContent();
		Reset(position);
	}
	
	void Player::LoadContent()
	{
		idleAnimation = std::make_shared<Animation>(Level()->Content().Load<Texture2D>("Sprites/Player/Idle"), 0.1f, true);
		runAnimation = std::make_shared<Animation>(Level()->Content().Load<Texture2D>("Sprites/Player/Run"), 0.1f, true);
		jumpAnimation = std::make_shared<Animation>(Level()->Content().Load<Texture2D>("Sprites/Player/Jump"), 0.1f, false);
		celebrateAnimation = std::make_shared<Animation>(Level()->Content().Load<Texture2D>("Sprites/Player/Celebrate"), 0.1f, false);
		dieAnimation = std::make_shared<Animation>(Level()->Content().Load<Texture2D>("Sprites/Player/Die"), 0.1f, false);

		// Calculate bounds within texture size.            
		const auto width = static_cast<int>(idleAnimation->FrameWidth() * 0.4);
		const auto left = (idleAnimation->FrameWidth() - width) / 2;
		const auto height = static_cast<int>(idleAnimation->FrameWidth() * 0.8);
		const auto top = idleAnimation->FrameHeight() - height;
		localBounds = Xna::Rectangle(left, top, width, height);

		// Load sounds.            
		killedSound = Level()->Content().Load<SoundEffect>("Sounds/PlayerKilled");
		jumpSound = Level()->Content().Load<SoundEffect>("Sounds/PlayerJump");
		fallSound = Level()->Content().Load<SoundEffect>("Sounds/PlayerFall");
	}

	void Player::Reset(Vector2 const& position)
	{
		Position(position);
		Velocity(Vector2::Zero());
		isAlive = true;
		sprite.PlayAnimation(idleAnimation);
	}

	void Player::Update(
		Xna::GameTime& gameTime,
		Xna::KeyboardState& keyboardState,
		Xna::GamePadState& gamePadState,
		Xna::DisplayOrientation orientation) {
		GetInput(keyboardState, gamePadState, orientation);
		ApplyPhysics(gameTime);

		if (IsAlive() && IsOnGround())
			if (std::abs(Velocity().X) - 0.02f > 0) {
				sprite.PlayAnimation(runAnimation);
			}
			else {
				sprite.PlayAnimation(idleAnimation);
			}

		movement = 0.0f;
		isJumping = false;
	}	

	void Player::GetInput(
		Xna::KeyboardState keyboardState,
		Xna::GamePadState gamePadState,
		Xna::DisplayOrientation orientation)
	{
		movement = gamePadState.ThumbSticks().Left().X * MoveStickScale;
				
		if (std::abs(movement) < 0.5f)
			movement = 0.0f;	
				
		if (gamePadState.IsButtonDown(Buttons::DPadLeft) ||
			keyboardState.IsKeyDown(Keys::Left) ||
			keyboardState.IsKeyDown(Keys::A))
		{
			movement = -1.0f;
		}
		else if (gamePadState.IsButtonDown(Buttons::DPadRight) ||
			keyboardState.IsKeyDown(Keys::Right) ||
			keyboardState.IsKeyDown(Keys::D))
		{
			movement = 1.0f;
		}
		
		isJumping =
			gamePadState.IsButtonDown(JumpButton) ||
			keyboardState.IsKeyDown(Keys::Space) ||
			keyboardState.IsKeyDown(Keys::Up) ||
			keyboardState.IsKeyDown(Keys::W);
	}

	void Player::ApplyPhysics(GameTime& gameTime)
	{
		const auto elapsed = static_cast<float>(gameTime.ElapsedGameTime().TotalSeconds());
		const auto previousPosition = Position();
				
		velocity.X += movement * MoveAcceleration * elapsed;
		velocity.Y = MathHelper::Clamp(velocity.Y + GravityAcceleration * elapsed, -MaxFallSpeed, MaxFallSpeed);

		velocity.Y = DoJump(velocity.Y, gameTime);
				
		if (IsOnGround())
			velocity.X *= GroundDragFactor;
		else
			velocity.X *= AirDragFactor;
				
		velocity.X = MathHelper::Clamp(velocity.X, -MaxMoveSpeed, MaxMoveSpeed);
				
		Position(Position() + velocity * elapsed);
		Position(Vector2(std::round(Position().X), std::round(Position().Y)));
		
		HandleCollisions();
				
		if (Position().X == previousPosition.X)
			velocity.X = 0;

		if (Position().Y == previousPosition.Y)
			velocity.Y = 0;
	}

	float Player::DoJump(float velocityY, Xna::GameTime& gameTime)
	{
		if (isJumping)
		{			
			if ((!wasJumping && IsOnGround()) || jumpTime > 0.0f)
			{
				if (jumpTime == 0.0f)
					jumpSound.Play();

				jumpTime += (float)gameTime.ElapsedGameTime().TotalSeconds();
				sprite.PlayAnimation(jumpAnimation);
			}
			
			if (0.0f < jumpTime && jumpTime <= MaxJumpTime)
			{				
				velocityY = JumpLaunchVelocity * (1.0f - (float)std::pow(jumpTime / MaxJumpTime, JumpControlPower));
			}
			else
			{
				jumpTime = 0.0f;
			}
		}
		else
		{			
			jumpTime = 0.0f;
		}
		wasJumping = isJumping;

		return velocityY;
	}

	void Player::HandleCollisions()
	{		
		auto bounds = BoundingRectangle();
		const auto left = bounds.Left();
		const auto right = bounds.Right();
		const auto top = bounds.Top();
		const auto bottom = bounds.Bottom();

		const auto leftTile = static_cast<int>(std::floor(static_cast<float>(left) / Tile::Width));
		const auto rightTile = static_cast<int>(std::ceil((static_cast<float>(right) / Tile::Width))) - 1;
		const auto topTile = static_cast<int>(std::floor(static_cast<float>(top) / Tile::Height));
		const auto bottomTile = static_cast<int>(std::ceil((static_cast<float>(bottom) / Tile::Height))) - 1;
		
		isOnGround = false;
				
		for (int y = topTile; y <= bottomTile; ++y)
		{
			for (int x = leftTile; x <= rightTile; ++x)
			{
				const auto collision = Level()->GetCollision(x, y);
				if (collision != TileCollision::Passable)
				{					
					const auto tileBounds = Level()->GetBounds(x, y);
					const auto depth = RectangleExtensions::GetIntersectionDepth(bounds, tileBounds);
					if (depth != Vector2::Zero())
					{
						const auto absDepthX = std::abs(depth.X);
						const auto absDepthY = std::abs(depth.Y);
						
						if (absDepthY < absDepthX || collision == TileCollision::Platform)
						{							
							if (previousBottom <= tileBounds.Top())
								isOnGround = true;
														
							if (collision == TileCollision::Impassable || IsOnGround())
							{								
								Position(Vector2(Position().X, Position().Y + depth.Y));
								bounds = BoundingRectangle();
							}
						}
						else if (collision == TileCollision::Impassable)
						{							
							Position(Vector2(Position().X + depth.X, Position().Y));
							bounds = BoundingRectangle();
						}
					}
				}
			}
		}
		
		previousBottom = bounds.Bottom();
	}

	void Player::OnKilled(std::shared_ptr<Enemy>& killedBy) {
		isAlive = false;

		if (killedBy != nullptr)
			killedSound.Play();
		else
			fallSound.Play();

		sprite.PlayAnimation(dieAnimation);
	}

	void Player::Draw(GameTime& gameTime, SpriteBatch& spriteBatch)
	{
		if (Velocity().X > 0)
			flip = SpriteEffects::FlipHorizontally;
		else if (Velocity().X < 0)
			flip = SpriteEffects::None;
		
		sprite.Draw(gameTime, spriteBatch, Position(), flip);
	}
}