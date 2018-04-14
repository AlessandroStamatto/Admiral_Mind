#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"
#include "SpriteInput.hpp"
#include "AlphaText.hpp"
#include "GameNetwork.hpp"

#include "SFML/Graphics.hpp"

#include <string>
#include <deque>


class GameState : public State
{
	public:
							GameState(StateStack& stack, Context context);

		virtual void		draw();
		virtual bool		update(sf::Time dt);
		virtual bool		handleEvent(const sf::Event& event);
                void        sendMessage (std::string msg);
                void        printChat ();

		World				mWorld;
		Player&				mPlayer;
        SpriteInput         chatInput;
        AlphaText           chatOutput;
        bool                onChat;
        CommandQueue        ignoreCommands;
        GameNetwork&        net;
        std::deque<std::string> chatBuffer;
};

#endif // GAMESTATE_HPP