#include "../headers/GameState.hpp"
#include <iostream>

using namespace std;


GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mWorld(*context.window, *context.fonts, *context.player, *context.net, *context.battleOpts, chatBuffer, *context.textures)
, mPlayer(*context.player), onChat(false), chatInput (*context.fonts, "Send message:")
, chatOutput (*context.fonts, "", 8), net (*context.net)
{
	cout << "-- World constructed -- \n";
	if (context.gameOpts->at("sound")) {
	    context.music->stop();
		context.music->play(Music::BattleTheme);
	}
	chatInput.setPosition  (350, 500);
	chatOutput.setPosition (330, 300);
	chatOutput.setScale    (0.50, 0.50);
}

void GameState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	mWorld.draw();
	window.draw(chatOutput);
	if (onChat)
		window.draw(chatInput);
}

void GameState::printChat () {
	while (chatBuffer.size() > 6) chatBuffer.pop_front();
	std::string tempText;
	for (auto& msg : chatBuffer)
		tempText += msg;
	chatOutput.setString (tempText, sf::Color::White);
	chatOutput.update(sf::seconds(0), ignoreCommands);
	cout << tempText << "\n";
}

bool GameState::update(sf::Time dt)
{
	// cout << ">> GameState Update\n";

	mWorld.update(dt);
	if (mWorld.newMsg) {
		printChat();
		mWorld.newMsg = false;
	}

    if (not onChat)
		chatOutput.update(dt, ignoreCommands);

	// cout << "-- Checking if a player was defeated -- \n";

	if (mWorld.playerDefeated())
	{
		cout << "Player defeated\n";
		if (mWorld.playerVictory()) {
			mPlayer.victory = true;
			getContext().player->victory = true;

			if (mWorld.totalTimeText != nullptr and mWorld.mPlayerBoard != nullptr and mWorld.mEnemyBoard != nullptr) {
				string totalTime {mWorld.totalTimeText->mText.getString()};
				int shoots1 = mWorld.mEnemyBoard->totalFired;
				int shoots2 = mWorld.mPlayerBoard->totalFired;
				getContext().net->sendMatchWinner(totalTime, shoots1, shoots2);
			}
		}
		
		requestStackPush(States::GameOver);
	}

	// cout << "-- Checking Commands -- \n";

	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);

	// cout << "GameState Update >> \n";

	getContext().net->update();
	chatInput.update (dt, commands);

	return true;
}

void GameState::sendMessage (string msg) {
	if (msg == "") return;
	net.sendChat (msg);
	chatBuffer.push_back ("  You> " + msg + "\n");
	printChat();
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commands);

	// Escape pressed, trigger {the pause screen}. Temporarely redesigned to go back to home
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
		getContext().net->sendGameEnd();
		requestStateClear();
		requestStackPush(States::Home);
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
		if (onChat) {
			sendMessage(chatInput.getText()); 
			chatInput.secretText = "";
			onChat = false;
			chatInput.unselect();
		} else {
			printChat();
			onChat = true;
			chatInput.select();
		}

	if (onChat) chatInput.handleEvent (event);

	return true;
}