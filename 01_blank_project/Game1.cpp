#include <Xna/Framework.hpp>
#include <Xna/Graphics.hpp>

using namespace std;
using namespace Xna;

// Game1 class, standard XNA template, with its loading, update, and draw functions.
class Game1 : public Game {
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

        base::LoadContent();
    }

    void Update(GameTime& gameTime) override {

        base::Update(gameTime);
    }

    void Draw(GameTime& gameTime) override {
        // Clears the screen with the classic XNA blue.
        // Colors::CornflowerBlue can also be used.
        graphics.GraphicsDevice()->Clear(Color::CornflowerBlue());

        base::Draw(gameTime);
    }

private:
    // GraphicsDeviceManager and SpriteBatch manage an internal pointer;
    // thus, they can receive null, meaning the instance is not initialized.
    GraphicsDeviceManager graphics = nullptr;
    SpriteBatch spriteBatch = nullptr;

    // To make the base function call explicit, C# style.
    using base = Game;
};

int main()
{
    auto game1 = Game1();
    game1.Run();

    return 0;
}