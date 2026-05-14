// LocalProject.cpp : Defines the entry point for the application.
//

#include <Xna/Framework.hpp>
#include <Xna/Graphics.hpp>
#include <Xna/Media.hpp>
#include <Xna/Audio.hpp>
#include <Xna/Input.hpp>

#include "Level.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Gem.hpp"

using namespace std;
using namespace Xna;
using namespace Xna::CSharp;

namespace Platformer {
	class Game1 : public Game {
		using base = Game;
	public:
		Game1() : Game() {
			graphics = Xna::GraphicsDeviceManager(*this);
			Content().RootDirectory("Content");
		}

		void Initialize() override {
			base::Initialize();
		}

		void LoadContent() override {
			spriteBatch = SpriteBatch(GraphicsDevice());

			hudFont = Content().Load<SpriteFont>("Fonts/Hud");
			winOverlay = Content().Load<Texture2D>("Overlays/you_win");
			loseOverlay = Content().Load<Texture2D>("Overlays/you_lose");
			diedOverlay = Content().Load<Texture2D>("Overlays/you_died");

			MediaPlayer::IsRepeating(true);
			MediaPlayer::Play(Content().Load<Song>("Sounds/Music"));

			LoadNextLevel();

			base::LoadContent();
		}

		void Update(Xna::GameTime& gameTime) override {

			HandleInput();			
			level->Update(gameTime, keyboardState, gamePadState, Window()->CurrentOrientation());

			base::Update(gameTime);
		}

		void Draw(Xna::GameTime& gameTime) override {
			graphics.GraphicsDevice()->Clear(Xna::Color::CornflowerBlue());

			spriteBatch.Begin();
			level->Draw(gameTime, spriteBatch);
			DrawHud();

			spriteBatch.End();

			base::Draw(gameTime);
		}
	
	private:
		void HandleInput();
		void LoadNextLevel();
		void ReloadCurrentLevel();
		void DrawHud();
		void DrawShadowedString(SpriteFont const& font, std::string const& value, Vector2 const& position, Color const& color);

	private:
		GraphicsDeviceManager graphics = nullptr;
		SpriteBatch spriteBatch = nullptr;
		SpriteFont hudFont = nullptr;
		Texture2D winOverlay = nullptr;
		Texture2D loseOverlay = nullptr;
		Texture2D diedOverlay = nullptr;
		int levelIndex{ -1 };
		std::shared_ptr<Level> level;
		bool wasContinuePressed{false};

		static constexpr TimeSpan WarningTime = TimeSpan::FromSeconds(30.0);

		GamePadState gamePadState{};
		KeyboardState keyboardState{};		

		static constexpr int numberOfLevels = 3;
	};

	void Game1::DrawShadowedString(SpriteFont const& font, std::string const& value, Vector2 const& position, Color const& color) {
		spriteBatch.DrawString(font, value, position + Vector2(1.0f, 1.0f), Color::Black());
		spriteBatch.DrawString(font, value, position, color);
	}

	void Game1::DrawHud() {
		const auto viewport = GraphicsDevice().Viewport();

		const auto titleSafeArea = viewport.Bounds(); //TitleSafeArea
		const auto hudLocation = Vector2(titleSafeArea.X, titleSafeArea.Y);
		const auto center = Vector2(titleSafeArea.X + titleSafeArea.Width / 2.0f,
			titleSafeArea.Y + titleSafeArea.Height / 2.0f);
		
		const std::string timeString = "TIME: " + std::to_string(level->TimeRemaining().Minutes()) + ":" + std::to_string(level->TimeRemaining().Seconds());
		Color timeColor{};
		
		if (level->TimeRemaining() > WarningTime ||	level->ReachedExit() ||	static_cast<int>(level->TimeRemaining().TotalSeconds()) % 2 == 0) {
			timeColor = Color::Yellow();
		}
		else {
			timeColor = Color::Red();
		}

		DrawShadowedString(hudFont, timeString, hudLocation, timeColor);
		
		const auto timeHeight = hudFont.MeasureString(timeString).Y;
		DrawShadowedString(hudFont, "SCORE: " + std::to_string(level->Score()), hudLocation + Vector2(0.0f, timeHeight * 1.2f), Color::Yellow());
		
		Texture2D status = nullptr;
		if (level->TimeRemaining() == TimeSpan::Zero())
		{
			if (level->ReachedExit())
			{
				status = winOverlay;
			}
			else
			{
				status = loseOverlay;
			}
		}
		else if (!level->Player()->IsAlive())
		{
			status = diedOverlay;
		}

		if (status != nullptr)
		{
			const auto statusSize = Vector2(status.Width(), status.Height());
			spriteBatch.Draw(status, center - statusSize / 2, Color::White());
		}
	}

	void Game1::ReloadCurrentLevel() {
		--levelIndex;
		LoadNextLevel();
	}

	void Game1::LoadNextLevel() {
		levelIndex = (levelIndex + 1) % numberOfLevels;
				
		if (level != nullptr)
			level = nullptr; //Dispose()
		
		std::string levelPath = std::format("Content/Levels/{0}.txt", std::to_string(levelIndex));
		auto fileStream = ifstream(levelPath);	

		if (fileStream.bad())
			throw std::runtime_error("Cannot open level path.");

		level = Level::Create(Services(), fileStream, levelIndex);
		fileStream.close();
	}

	void Game1::HandleInput() {		
		keyboardState = Keyboard::GetState();
		gamePadState = GamePad::GetState(PlayerIndex::One);
				
		if (gamePadState.Buttons().Back() == ButtonState::Pressed)
			Exit();

		const auto continuePressed =
			keyboardState.IsKeyDown(Keys::Space) ||
			gamePadState.IsButtonDown(Buttons::A);
		
		if (!wasContinuePressed && continuePressed)
		{
			if (!level->Player()->IsAlive())
			{
				level->StartNewLife();
			}
			else if (level->TimeRemaining() == TimeSpan::Zero())
			{
				if (level->ReachedExit())
					LoadNextLevel();
				else
					ReloadCurrentLevel();
			}
		}

		wasContinuePressed = continuePressed;
	}
}


int main()
{	
	auto game1 = Platformer::Game1();
	game1.Run();
	//game1 = nullptr;

	return 0;
}
