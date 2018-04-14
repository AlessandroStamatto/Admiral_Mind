#ifndef STATE_HPP
#define STATE_HPP

#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include "GameNetwork.hpp"
#include "Player.hpp"
#include "MusicPlayer.hpp"
#include "GameDatatypes.hpp"

#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include <memory>


class StateStack;

class State
{
	public:
		typedef std::unique_ptr<State> Ptr;

		struct Context
		{
								Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player);

			sf::RenderWindow*	 			window;
			TextureHolder*		 			textures;
			FontHolder*			 			fonts;
			Player*				 			player;
			GameNetwork*		 			net;
			MusicPlayer*		 			music;
			mapOptions*                     battleOpts;
			mapOptions*	 	                gameOpts;
			bool*                           justRegistered;
		};


	public:
							State(StateStack& stack, Context context);
							State(StateStack& stack, Context&& context);
		virtual				~State();

		virtual void		draw() = 0;
		virtual bool		update(sf::Time dt) = 0;
		virtual bool		handleEvent(const sf::Event& event) = 0;


	protected:
		void				requestStackPush(States::ID stateID);
		void				requestStackPop();
		void				requestStateClear();

		Context				getContext() const;


	private:
		StateStack*			mStack;
		Context				mContext;
};

#endif // STATE_HPP
