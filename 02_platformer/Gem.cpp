#include "Gem.hpp"
#include "Level.hpp"
#include <cmath>

namespace Platformer {
	std::shared_ptr<Platformer::Level> Gem::Level() const { return level; }

	Gem::Gem(std::shared_ptr<Platformer::Level> const& level, Xna::Vector2 const& position)
	: level(level), basePosition(position){

		LoadContent();
	}

	void Gem::LoadContent() {
		texture = Level()->Content().Load<Xna::Texture2D>("Sprites/Gem");
		origin = Xna::Vector2(texture.Width() / 2.0f, texture.Height() / 2.0f);
		collectedSound = Level()->Content().Load<Xna::SoundEffect>("Sounds/GemCollected");
	}

	void Gem::Update(Xna::GameTime& gameTime) {		
		constexpr auto BounceHeight = 0.18f;
		constexpr auto BounceRate = 3.0f;
		constexpr auto BounceSync = -0.75f;
		   
		const auto t = gameTime.TotalGameTime().TotalSeconds() * BounceRate + Position().X * BounceSync;
		bounce = std::sin(t) * BounceHeight * texture.Height();
	}

	void Gem::OnCollected(Player& collectedBy) {
		collectedSound.Play();
	}

	void Gem::Draw(Xna::GameTime& gameTime, Xna::SpriteBatch& spriteBatch)
	{		
		spriteBatch.Draw(texture, Position(), Color, std::nullopt, 0.0f, origin, 1.0f, Xna::SpriteEffects::None, 0.0f);
	}
}