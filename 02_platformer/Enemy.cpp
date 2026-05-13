#include "Enemy.hpp"
#include <cstdint>

using namespace Xna;

namespace Platformer {
	Xna::Rectangle Enemy::BoundingRectangle() const
	{
		auto left = std::round(Position().X - sprite.Origin().X) + localBounds.X;
		auto top = std::round(Position().Y - sprite.Origin().Y) + localBounds.Y;

		return Xna::Rectangle(left, top, localBounds.Width, localBounds.Height);
	}

	Enemy::Enemy(std::shared_ptr<Platformer::Level> const& level, Xna::Vector2 const& position, std::filesystem::path spriteSet)
		:level(level), position(position)
	{
		LoadContent(spriteSet);
	}

	void Enemy::LoadContent(std::filesystem::path spriteSet) {
		spriteSet = "Sprites/" / spriteSet;
		runAnimation = std::make_shared<Animation>(Level()->Content().Load<Texture2D>(spriteSet / "Run"), 0.1f, true);
		idleAnimation = std::make_shared<Animation>(Level()->Content().Load<Texture2D>(spriteSet / "Idle"), 0.15f, true);
		sprite.PlayAnimation(idleAnimation);

		const auto width = static_cast<int>((idleAnimation->FrameWidth() * 0.35));
		const auto left = (idleAnimation->FrameWidth() - width) / 2;
		const auto height = static_cast<int>((idleAnimation->FrameWidth() * 0.7));
		const auto top = idleAnimation->FrameHeight() - height;
		localBounds = Xna::Rectangle(left, top, width, height);
	}

	void Enemy::Update(Xna::GameTime& gameTime)
	{
		const auto elapsed = gameTime.ElapsedGameTime().TotalSeconds();
		
		const auto posX = Position().X + localBounds.Width / 2 * (int)direction;
		const auto tileX = static_cast<int>(std::floor(posX / Tile::Width)) - (int)direction;
		const auto tileY = static_cast<int>(std::floor(Position().Y / Tile::Height));

		if (waitTime > 0)
		{
			waitTime = std::max(0.0f, waitTime - static_cast<float>(gameTime.ElapsedGameTime().TotalSeconds()));
			if (waitTime <= 0.0f) {
				direction = (FaceDirection)(-(int)direction);
			}
		}
		else
		{			
			if (Level()->GetCollision(tileX + (int)direction, tileY - 1) == TileCollision::Impassable ||
				Level()->GetCollision(tileX + (int)direction, tileY) == TileCollision::Passable)
			{
				waitTime = MaxWaitTime;
			}
			else
			{				
				const auto velocity = Vector2((int)direction * MoveSpeed * elapsed, 0.0f);
				position = position + velocity;
			}
		}
	}

	void Enemy::Draw(GameTime& gameTime, SpriteBatch& spriteBatch) {		
		if (!Level()->Player()->IsAlive() ||
			Level()->ReachedExit() ||
			Level()->TimeRemaining() == CSharp::TimeSpan::Zero() ||
			waitTime > 0)
		{
			sprite.PlayAnimation(idleAnimation);
		}
		else
		{
			sprite.PlayAnimation(runAnimation);
		}
		
		const auto flip = (int)direction > 0 ? SpriteEffects::FlipHorizontally: SpriteEffects::None;
		sprite.Draw(gameTime, spriteBatch, Position(), flip);
	}
}