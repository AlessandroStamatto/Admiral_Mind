#ifndef REGISTERSTATE_HPP
#define REGISTERSTATE_HPP

#include "State.hpp"
#include "SpriteInput.hpp"
#include "ScreenButton.hpp"
#include "CommandQueue.hpp"
#include "AlphaText.hpp"
#include "SFML/Graphics.hpp"
#include "Save.hpp"
#include "colorDefines.hpp"

#include <array>

class RegisterState : public State
{
	public:
						RegisterState(StateStack& stack, Context context);

		virtual void			draw();
		virtual bool			update(sf::Time dt);
		virtual bool			handleEvent(const sf::Event& event);
                void                            goToHome();
                void                            goToLogin();
                void                            registration();

        sf::Sprite              mBackgroundSprite;
        sf::Text                registerButton;
        ScreenButton            backButton;
        sf::Sprite              fields5;
        SpriteInput             username;
        SpriteInput             realname;
        SpriteInput             email;
        SpriteInput             password1;
        SpriteInput             password2;
        CommandQueue            ignoreCommandQueue;
        AlphaText*              mAlphaText;
        GameSave                gameSave;

        int selectedBox = 0;
        std::array<SpriteInput*, 5> inputBoxes;
};

#endif // REGISTERSTATE_HPP
