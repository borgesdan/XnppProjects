#pragma once

#include <Xna/Framework.hpp>
#include <Xna/Graphics.hpp>
#include <Xna/Audio.hpp>
#include "Circle.hpp"
#include "Player.hpp"
#include "Tile.hpp"

namespace Platformer {
	class Level;

	class Gem
	{
	public:
		static constexpr int PointValue = 30;
		static constexpr Xna::Color Color = Xna::Color::Yellow();

	public:
		std::shared_ptr<Platformer::Level> Level() const;
		inline Xna::Vector2 Position() const { return basePosition + Xna::Vector2(0.0f, bounce); }
		inline Circle BoundingCircle() const { return Circle(Position(), Tile::Width / 3.0f); }

		Gem(std::shared_ptr<Platformer::Level> const& level, Xna::Vector2 const& position);

		void LoadContent();
		void Update(Xna::GameTime& gameTime);
		void OnCollected(Player& collectedBy);
		void Draw(Xna::GameTime& gameTime, Xna::SpriteBatch& spriteBatch);

	private:
		Xna::Texture2D texture{ nullptr };
		Xna::Vector2 origin{ 0 };
		Xna::SoundEffect collectedSound{ nullptr };
		Xna::Vector2 basePosition{ 0 };
		float bounce{ 0 };
		std::shared_ptr<Platformer::Level> level;
	};
}