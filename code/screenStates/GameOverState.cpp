#include "../headers/GameOverState.hpp"
#include "../headers/Utility.hpp"
#include "../headers/Player.hpp"
#include "../headers/ResourceHolder.hpp"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"


GameOverState::GameOverState(StateStack& stack, Context context)
: State(stack, context)
, mGameOverText()
, mElapsedTime(sf::Time::Zero)
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mGameOverText.setFont(font);

	if (getContext().player->victory)
		mGameOverText.setString("Victory!");
	else
		mGameOverText.setString("Defeated...");
	mGameOverText.setCharacterSize(70);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
}

bool GameOverState::update(sf::Time dt)
{
	// Show state for 2 seconds, after return to menu
	mElapsedTime += dt;
	if (mElapsedTime > sf::seconds(2))
	{
		getContext().net->sendGameEnd();
		requestStateClear();
		requestStackPush(States::Home);
	}
	getContext().net->update();
	return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
	return false;
}
