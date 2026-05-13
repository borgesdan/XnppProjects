#include "AnimationPlayer.hpp"
#include <stdexcept>
#include <cstdint>

using namespace Xna;

namespace Platformer {
	void AnimationPlayer::PlayAnimation(std::shared_ptr<Platformer::Animation> const& animation) {
		if (Animation() == animation)
			return;

		this->animation = animation;
		frameIndex = 0;
		time = 0.0f;
	}

	void AnimationPlayer::Draw(Xna::GameTime& gameTime, Xna::SpriteBatch& spriteBatch, Xna::Vector2 const& position, Xna::SpriteEffects const& spriteEffects) {
        if (Animation() == nullptr)
            throw std::runtime_error("No animation is currently playing.");
        
        time += (float)gameTime.ElapsedGameTime().TotalSeconds();
        
        while (time > Animation()->FrameTime())
        {
            time -= Animation()->FrameTime();
            
            if (Animation()->IsLooping())
            {
                frameIndex = (frameIndex + 1) % Animation()->FrameCount();
            }
            else
            {
                frameIndex = std::min(frameIndex + 1, Animation()->FrameCount() - 1);
            }
        }        
        auto source = Xna::Rectangle(FrameIndex() * Animation()->Texture().Height(), 0, Animation()->Texture().Height(), Animation()->Texture().Height());

        // Draw the current frame.
        //spriteBatch.Draw(Animation()->Texture(), position, source, Color::White(), 0.0f, Origin(), 1.0f, spriteEffects, 0.0f);
        spriteBatch.Draw(Animation()->Texture(), position, Color::White(), source, 0, Origin(), 1, spriteEffects, 0);
	}
}