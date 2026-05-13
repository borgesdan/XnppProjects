#include "Level.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "RectangleExtensions.hpp"

using namespace Xna;
using namespace CSharp;

namespace Platformer {
	std::shared_ptr<Level> Level::Create(Xna::GameServiceContainer const& serviceProvider, std::ifstream& fileStream, int levelIndex) {
		auto level = std::shared_ptr<Level>(new Level());
		level->Initialize(serviceProvider, fileStream, levelIndex);

		return level;
	}

	void Level::Initialize(Xna::GameServiceContainer const& serviceProvider, std::ifstream& fileStream, int levelIndex)
	{
		content = ContentManager(serviceProvider, "Content");
		timeRemaining = TimeSpan::FromMinutes(2.0);

		LoadTiles(fileStream);

		layers.resize(3);

		for (size_t i = 0; i < layers.size(); ++i)
		{
			int segmentIndex = levelIndex;
			layers[i] = Content().Load<Texture2D>("Backgrounds/Layer" + std::to_string(i) + "_" + std::to_string(segmentIndex));
		}

		exitReachedSound = Content().Load<SoundEffect>("Sounds/ExitReached");
	}

	void Level::LoadTiles(std::ifstream& fileStream)
	{
		std::vector<std::string> lines;
		std::string line;
		std::getline(fileStream, line);
		size_t width = line.size();		

		while (fileStream.good()) {
			if (line.size() != width)
				throw std::runtime_error("The length of line is different from all preceeding lines.");

			lines.push_back(line);
			line.clear();
			std::getline(fileStream, line);
		}

		tiles = std::vector<std::vector<Tile>>(width, std::vector<Tile>(lines.size()));

		// Loop over every tile position,
		for (size_t y = 0; y < lines.size(); ++y) {
			for (size_t x = 0; x < width; ++x) {

				auto tileType = lines[y][x];

				tiles[x][y] = LoadTile(tileType, x, y);
			}
		}

		if (Player() == nullptr)
			throw std::runtime_error("A level must have a starting point.");
		if (exit == InvalidPosition)
			throw std::runtime_error("A level must have an exit.");

	}

	Tile Level::LoadTile(char tileType, int x, int y)
	{
		switch (tileType)
		{
			// Blank space
		case '.':
			return Tile(nullptr, TileCollision::Passable);

			// Exit
		case 'X':
			return LoadExitTile(x, y);

			// Gem
		case 'G':
			return LoadGemTile(x, y);

			// Floating platform
		case '-':
			return LoadTile("Platform", TileCollision::Platform);

			// Various enemies
		case 'A':
			return LoadEnemyTile(x, y, "MonsterA");
		case 'B':
			return LoadEnemyTile(x, y, "MonsterB");
		case 'C':
			return LoadEnemyTile(x, y, "MonsterC");
		case 'D':
			return LoadEnemyTile(x, y, "MonsterD");

			// Platform block
		case '~':
			return LoadVarietyTile("BlockB", 2, TileCollision::Platform);

			// Passable block
		case ':':
			return LoadVarietyTile("BlockB", 2, TileCollision::Passable);

			// Player 1 start point
		case '1':
			return LoadStartTile(x, y);

			// Impassable block
		case '#':
			return LoadVarietyTile("BlockA", 7, TileCollision::Impassable);

			// Unknown tile type character
		default:
			throw std::runtime_error(std::format("Unsupported tile type character '{0}' at position {1}, {2}.", std::to_string(tileType), std::to_string(x), std::to_string(y)));
		}
	}

	Tile Level::LoadTile(std::filesystem::path const& name, TileCollision collision)
	{
		return Tile(Content().Load<Texture2D>("Tiles/" / name), collision);
	}

	Tile Level::LoadVarietyTile(std::filesystem::path const& baseName, int variationCount, TileCollision collision)
	{
		//TODO
		auto dist = std::uniform_int_distribution<>(0, variationCount - 1);
		auto index = dist(randomEngine);
		return LoadTile(baseName.string() + std::to_string(index), collision);
	}

	Tile Level::LoadStartTile(int x, int y)
	{
		if (Player() != nullptr)
			throw std::runtime_error("A level may only have one starting point.");

		start = RectangleExtensions::GetBottomCenter(GetBounds(x, y));
		player = std::make_shared<Platformer::Player>(shared_from_this(), start);

		return Tile(nullptr, TileCollision::Passable);
	}

	Tile Level::LoadExitTile(int x, int y)
	{
		if (exit != InvalidPosition)
			throw std::runtime_error("A level may only have one exit.");

		exit = GetBounds(x, y).Center();

		return LoadTile("Exit", TileCollision::Passable);
	}

	Tile Level::LoadEnemyTile(int x, int y, std::filesystem::path const& spriteSet)
	{
		const auto position = RectangleExtensions::GetBottomCenter(GetBounds(x, y));
		enemies.push_back(std::make_shared<Enemy>(shared_from_this(), position, spriteSet));

		return Tile(nullptr, TileCollision::Passable);
	}

	Tile Level::LoadGemTile(int x, int y)
	{
		const auto position = GetBounds(x, y).Center();
		gems.push_back(std::make_shared<Gem>(shared_from_this(), Vector2(position.X, position.Y)));

		return Tile(nullptr, TileCollision::Passable);
	}

	TileCollision Level::GetCollision(int x, int y)
	{
		if (x < 0 || x >= Width())
			return TileCollision::Impassable;

		if (y < 0 || y >= Height())
			return TileCollision::Passable;

		return tiles[x][y].Collision;
	}

	int Level::Width() const {
		return tiles.size();
	};

	int Level::Height() const {
		return tiles[0].size();
	};

	void Level::Update(
		Xna::GameTime& gameTime,
		Xna::KeyboardState& keyboardState,
		Xna::GamePadState& gamePadState,
		DisplayOrientation orientation)
	{
		if (!Player()->IsAlive() || TimeRemaining() == TimeSpan::Zero())
		{
			Player()->ApplyPhysics(gameTime);
		}
		else if (ReachedExit())
		{
			auto seconds = static_cast<int64_t>(std::round(gameTime.ElapsedGameTime().TotalSeconds() * 100.0f));
			seconds = std::min(seconds, static_cast<int64_t>(std::ceil(TimeRemaining().TotalSeconds())));
			timeRemaining = timeRemaining - CSharp::TimeSpan::FromSeconds(seconds);
			score += seconds * PointsPerSecond;
		}
		else
		{
			timeRemaining = timeRemaining - gameTime.ElapsedGameTime();
			Player()->Update(gameTime, keyboardState, gamePadState, orientation);
			UpdateGems(gameTime);

			if (Player()->BoundingRectangle().Top() >= Height() * Tile::Height)
				OnPlayerKilled(nullptr);

			UpdateEnemies(gameTime);

			if (Player()->IsAlive() &&
				Player()->IsOnGround() &&
				Player()->BoundingRectangle().Contains(exit))
			{
				OnExitReached();
			}
		}

		if (timeRemaining < CSharp::TimeSpan::Zero())
			timeRemaining = CSharp::TimeSpan::Zero();
	}

	void Level::UpdateGems(Xna::GameTime& gameTime)
	{
		for (size_t i = 0; i < gems.size(); ++i)
		{
			auto& gem = gems[i];

			gem->Update(gameTime);

			if (gem->BoundingCircle().Intersects(Player()->BoundingRectangle()))
			{
				auto gem1 = gem;
				gems.erase(gems.begin() + i--);
				OnGemCollected(*gem1, *Player());
			}
		}
	}

	void Level::UpdateEnemies(Xna::GameTime& gameTime)
	{
		for (auto& enemy : enemies)
		{
			enemy->Update(gameTime);

			if (enemy->BoundingRectangle().Intersects(Player()->BoundingRectangle()))
			{
				OnPlayerKilled(enemy);
			}
		}
	}

	void Level::OnGemCollected(Gem& gem, Platformer::Player& collectedBy)
	{
		score += Gem::PointValue;

		gem.OnCollected(collectedBy);
	}

	void Level::OnPlayerKilled(std::shared_ptr<Enemy> killedBy)
	{
		Player()->OnKilled(killedBy);
	}

	void Level::OnExitReached()
	{
		Player()->OnReachedExit();
		exitReachedSound.Play();
		reachedExit = true;
	}

	void Level::StartNewLife()
	{
		Player()->Reset(start);
	}

	void Level::Draw(Xna::GameTime& gameTime, Xna::SpriteBatch& spriteBatch)
	{
		for (size_t i = 0; i <= EntityLayer; ++i)
			spriteBatch.Draw(layers[i], Vector2::Zero(), Color::White());

		DrawTiles(spriteBatch);

		for(auto& gem : gems)
			gem->Draw(gameTime, spriteBatch);

		Player()->Draw(gameTime, spriteBatch);

		for(auto& enemy : enemies)
			enemy->Draw(gameTime, spriteBatch);

		for (size_t i = EntityLayer + 1; i < layers.size(); ++i)
			spriteBatch.Draw(layers[i], Vector2::Zero(), Color::White());
	}

	void Level::DrawTiles(Xna::SpriteBatch& spriteBatch)
	{
		for (size_t y = 0; y < Height(); ++y)
		{
			for (size_t x = 0; x < Width(); ++x)
			{				
				auto& texture = tiles[x][y].Texture;
				if (texture != nullptr)
				{
					auto position = Vector2(x, y) * Tile::Size;
					spriteBatch.Draw(texture, position, Color::White());
				}
			}
		}
	}
}