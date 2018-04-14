#ifndef LOGINSTATE_HPP
#define LOGINSTATE_HPP

#include "State.hpp"
#include "SpriteInput.hpp"
#include "SpriteNode.hpp"
#include "ScreenButton.hpp"
#include "Save.hpp"
#include "CommandQueue.hpp"
#include "AlphaText.hpp"
#include "colorDefines.hpp"
#include "SFML/Graphics.hpp"

#include <array>

class LoginState : public State
{
	public:
						LoginState(StateStack& stack, Context context);

		virtual void			draw();
		virtual bool			update(sf::Time dt);
		virtual bool			handleEvent(const sf::Event& event);
                void                            goToHome();
                void                            registration();

        sf::Sprite              mBackgroundSprite;
        sf::Text                loginButton;
        ScreenButton            registerButton;
        sf::Sprite              loginFields;
        SpriteInput             username;
        SpriteInput             password;
        SpriteInput             server;
        CommandQueue            ignoreCommandQueue;
        AlphaText*              mAlphaText;

        int selectedBox = 0;
        std::array<SpriteInput*, 3> inputBoxes; 
        GameSave gameSave;

};

#endif // LOGINSTATE_HPP
