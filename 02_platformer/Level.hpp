#pragma once

#include <Xna/Framework.hpp>
#include <Xna/Graphics.hpp>
#include <Xna/Audio.hpp>
#include <Xna/Content.hpp>
#include <Xna/Input.hpp>
#include <Xna/CSharp/TimeSpan.hpp>
#include <fstream>
#include <filesystem>
#include <vector>
#include <random>
#include "Tile.hpp"
#include "Player.hpp"
#include "Gem.hpp"
#include "Enemy.hpp"

namespace Platformer {
	class Level : public std::enable_shared_from_this<Level> {
	public:
		inline std::shared_ptr<Platformer::Player> Player() { return player; }
		inline int Score() const { return score; }
		inline bool ReachedExit() const { return reachedExit; }
		inline Xna::CSharp::TimeSpan TimeRemaining() const { return timeRemaining; }
		inline Xna::ContentManager Content() const { return content; }

		static std::shared_ptr<Level> Create(Xna::GameServiceContainer const& serviceProvider, std::ifstream& fileStream, int levelIndex);

		TileCollision GetCollision(int x, int y);
		inline Xna::Rectangle GetBounds(int x, int y) const { return Xna::Rectangle(x * Tile::Width, y * Tile::Height, Tile::Width, Tile::Height); }
		int Width() const;
		int Height() const;
		void Update(
			Xna::GameTime& gameTime,
			Xna::KeyboardState& keyboardState,
			Xna::GamePadState& gamePadState,
			Xna::DisplayOrientation orientation);
		void StartNewLife();
		void Draw(Xna::GameTime& gameTime, Xna::SpriteBatch& spriteBatch);

	private:
		Level();		
		void Initialize(Xna::GameServiceContainer const& serviceProvider, std::ifstream& fileStream, int levelIndex);
		void LoadTiles(std::ifstream& fileStream);
		Tile LoadTile(char tileType, int x, int y);
		Tile LoadTile(std::filesystem::path const& name, TileCollision collision);
		Tile LoadVarietyTile(std::filesystem::path const& baseName, int variationCount, TileCollision collision);
		Tile LoadStartTile(int x, int y);
		Tile LoadExitTile(int x, int y);
		Tile LoadEnemyTile(int x, int y, std::filesystem::path const& spriteSet);
		Tile LoadGemTile(int x, int y);
		void UpdateGems(Xna::GameTime& gameTime);
		void UpdateEnemies(Xna::GameTime& gameTime);
		void OnGemCollected(Gem& gem, Platformer::Player& collectedBy);
		void OnPlayerKilled(std::shared_ptr<Enemy> killedBy);
		void OnExitReached();
		void DrawTiles(Xna::SpriteBatch& spriteBatch);
		
	private:
		static constexpr size_t EntityLayer = 2;
		static constexpr Xna::Point InvalidPosition = Xna::Point(-1, -1);
		static constexpr int randomSeed = 354668;
		static constexpr int PointsPerSecond = 5;

		std::random_device randomdDevice;
		std::mt19937 randomEngine;
		std::uniform_int_distribution<> randomDistr;

		std::vector<std::vector<Tile>> tiles;
		std::vector<Xna::Texture2D> layers;
		std::shared_ptr<Platformer::Player> player;
		std::vector<std::shared_ptr<Gem>> gems;
		std::vector< std::shared_ptr<Enemy>> enemies;
		Xna::Vector2 start{};
		Xna::Point exit{ InvalidPosition };
		Xna::ContentManager content{ nullptr };
		Xna::CSharp::TimeSpan timeRemaining{};
		bool reachedExit{ false };
		int score{ 0 };
		Xna::SoundEffect exitReachedSound = nullptr;
	};
}