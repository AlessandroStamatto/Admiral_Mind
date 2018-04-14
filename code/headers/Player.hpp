#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Command.hpp"
#include "Board.hpp"

#include "SFML/Window/Event.hpp"
#include <map>
#include <vector>
#include <string>


class CommandQueue;

struct Settings {
	int boardNCols, boardNRows;
};

class Player
{
	public:
		enum Action
		{
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,
			Fire,
			ActionCount
		};


	public:
								Player();

		void					handleEvent(const sf::Event& event, CommandQueue& commands);
		void					handleRealtimeInput(CommandQueue& commands);

		void					assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key		getAssignedKey(Action action) const;

	private:
		void					initializeActions();
		static bool				isRealtimeAction(Action action);

	public:
		bool victory;
		std::vector<Ship>	                  ships;
		std::string							  name;
		Settings							  settings;

	private:
		std::map<sf::Keyboard::Key, Action>		mKeyBinding;
		std::map<Action, Command>				mActionBinding;
};

#endif // PLAYER_HPP
