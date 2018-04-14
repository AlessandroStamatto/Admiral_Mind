#include "headers/World.hpp"

#include "SFML/Graphics/RenderWindow.hpp"

#include "./headers/Utility.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>
#include <iostream>
#include <fstream>

using namespace sf;
using namespace std;

World::World(sf::RenderWindow& window, FontHolder& fonts, Player& _player, GameNetwork& _net, mapOptions& _battleOpts, std::deque<std::string>& chatBuff, TextureHolder& textures)
: mWindow(window), mFonts(fonts), mWorldView(window.getDefaultView()), chatBuffer (chatBuff), newMsg (false)
, mTextures(textures) , mSceneGraph(), mSceneLayers(), mPlayer(_player), net(_net), battleOpts(_battleOpts)
, player1Photo      (mTextures.get(Textures::player1))
, player2Photo      (mTextures.get(Textures::player2))
, player1Name       ("Player1", mFonts.get(Fonts::Bold), 14)
, player2Name       ("Player2", mFonts.get(Fonts::Bold), 14)
{
	loadTextures();
	buildScene();

    battleOptsSprite = new BattleOptionsSprite (mTextures, _battleOpts);
    battleOptsSprite->setPosition (700, 0);
    
	// Prepare the view
	mWorldView.setCenter(466, 350);
}

void World::loadTextures()
{
    static bool notLoaded = true;
    if (notLoaded) 
    {
        mTextures.load(Textures::GameBackground,     "Media/Textures/screens/background.game.png");
    
        mTextures.load(Textures::BoardFire,    "Media/Textures/board/fire.png");
        mTextures.load(Textures::BoardSea,     "Media/Textures/board/sea.png");
        mTextures.load(Textures::BoardSelected,"Media/Textures/board/selected.png");
        mTextures.load(Textures::IslandSquare, "Media/Textures/board/islandSquare.png");

        mTextures.load(Textures::DestroyedCarrier,"Media/Textures/ships/carrier_destroyed.png");
        mTextures.load(Textures::DestroyedBattleship,"Media/Textures/ships/battleship_destroyed.png");
        mTextures.load(Textures::DestroyedSubmarine,"Media/Textures/ships/submarine_destroyed.png");
        mTextures.load(Textures::DestroyedCruiser,"Media/Textures/ships/cruiser_destroyed.png");
        mTextures.load(Textures::DestroyedDestroyer,"Media/Textures/ships/destroyer_destroyed.png");
        mTextures.load(Textures::DestroyedMine,"Media/Textures/ships/mine_destroyed.png");

        mTextures.load(Textures::Missile,    "Media/Textures/misc/missile.png");
        notLoaded = false;
    }
}

void World::update(sf::Time dt)
{
    if (totalTimeText != nullptr and timeTextReady) {
        int mins = static_cast<int>(totalTime.getElapsedTime().asSeconds() / 60);
        int secs = static_cast<int>(totalTime.getElapsedTime().asSeconds()) % 60;
        std::string secs_str = toString(secs);
        std::string mins_str = toString(mins);
        if (secs < 10)
            secs_str = std::string("0") + secs_str;
        if (mins < 10)
            mins_str = std::string("0") + mins_str;
        totalTimeText->setString (mins_str + ":" + secs_str);
    }

    // cout << ">> World Update\n";
	// Forward commands to scene graph
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	// Regular update step
	mSceneGraph.update(dt, mCommandQueue);

    if ( (not firstTime) and (bTimer != nullptr) ) {
        if (bTimer->timeEnded() and playerTurn == 1) {
                mPlayerBoard->reset();
                mEnemyBoard->reset();
                bTimer->reset();

                playerTurn = 2;
                net.sendTurn();
        }

        if (mPlayerBoard != nullptr and mEnemyBoard != nullptr)
            if (mPlayerBoard->fired or mEnemyBoard->fired) {
                mPlayerBoard->reset();
                mEnemyBoard->reset();
                bTimer->reset();
            }    
    }

    packetID id;
    while (static_cast<bool> (id = net.receive()) ) {
        switch (id) {
            case packetID::Ships : {
                cout << "Ships received!\n";
                if (firstTime) {
                    enemyShips = net.receiveShips();
                    // Add the enemy board to the scene
                    std::unique_ptr<Board> boardSprite(new Board(mTextures, mFonts, net, 10, 10, 2, playerTurn, enemyShips, battleOpts));
                    mEnemyBoard = boardSprite.get();
                    //mEnemyBoard->setPosition (550, 200); //enemy position
                    mEnemyBoard->setPosition (510, 140); //enemy position
                    mEnemyBoard->defeated = false;
                    if (mPlayerBoard != nullptr) {
                        mPlayerBoard->enemyBoard = mEnemyBoard;
                        mEnemyBoard->enemyBoard = mPlayerBoard;
                    } else {
                        cout << "Bad pointers at " << __LINE__ << "\n";
                    }
                    if (not enemyName.empty())
                        mEnemyBoard->setName(enemyName);
                    mSceneLayers[BoardLayer]->attachChild(std::move(boardSprite));
                    firstTime = false;                    

                    std::unique_ptr<SpriteTimer> battleTimer( new SpriteTimer (30, mFonts, mTextures) );
                    bTimer = battleTimer.get();
                    bTimer->setPosition (468, 599);
                    mSceneLayers[BoardLayer]->attachChild(std::move(battleTimer));
                    cout << "Timer activated!\n";

                    std::unique_ptr<TextNode>   timeText (new TextNode(mFonts, "00:00"));
                    totalTimeText = timeText.get();
                    totalTimeText->mText.setColor(DARK_BLUE);
                    totalTimeText->mText.setCharacterSize(16);
                    centerOrigin (totalTimeText->mText);
                    totalTimeText->setPosition(468, 680);
                    totalTime.restart();
                    mSceneLayers[TopLayer]->attachChild(std::move(timeText));
                    timeTextReady = true;
                }
                break;
            }
            case packetID::Turn : 
                cout << "Turn received!\n";
                playerTurn = playerTurn == 1 ? 2 : 1;
                if ( (not firstTime) and (bTimer != nullptr) ) 
                    bTimer->reset();
                break;
            case packetID::Fire : net.receiveFire (mPlayerBoard->cursorI, mPlayerBoard->cursorJ);
                cout << "fire received\n";
                mPlayerBoard->fire();
                break;
            case packetID::Mine : net.receiveMine (mEnemyBoard->cursorI, mEnemyBoard->cursorJ);
                cout << "mine received\n";
                mEnemyBoard->fire(true);
                break;
            case packetID::Name :
                cout << "name received\n";
                enemyName = net.receiveName();
                player2Name.setString (enemyName);
                centerOrigin(player2Name);
                player2Name.setPosition (610, 679);
                if (not firstTime)
                    if (mEnemyBoard != nullptr)
                        mEnemyBoard->setName(enemyName);
                break;
            case packetID::GameEnd : {
                if (not mPlayerBoard->defeated)
                    mEnemyBoard->defeated = true;
                cout << "GameEnd received!\n";
                break;
            }
            case packetID::Chat : {
                chatBuffer.push_back ("Enemy> " + net.receiveChat() + "\n");
                newMsg = true;
                cout << "Chat received!\n";
                break;
            }
        }
    }
    // cout << "World Update >>\n";
}

bool World::playerDefeated() {
    //cout << "-- Before nullptr check -- \n";
    if (firstTime) return false;
    if ( (mEnemyBoard == nullptr) || (mPlayerBoard == nullptr) ){ 
        //cout << "Boards not constructed yet!\n";
        return false;
    } else {
        return ( mPlayerBoard->defeated || mEnemyBoard->defeated );
    }
}

bool World::playerVictory() {
    if (firstTime) return false;
    if (mEnemyBoard == nullptr) return false;
	return (not mPlayerBoard->defeated and mEnemyBoard->defeated);
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
    mWindow.draw (*battleOptsSprite);
    mWindow.draw (player1Photo);
    mWindow.draw (player2Photo);
    mWindow.draw (player1Name);
    mWindow.draw (player2Name);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == TopLayer) ? Category::SceneTopLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the ocean background
	sf::Texture& texture = mTextures.get(Textures::GameBackground);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture));
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));


	playerTurn = 1;

	// Add the player board to the scene
	std::unique_ptr<Board> boardSprite(new Board(mTextures, mFonts, net, 10, 10, 1, playerTurn, mPlayer.ships, battleOpts));
	mPlayerBoard = boardSprite.get();
	//mPlayerBoard->setPosition (100, 200); //ally position +21,-60
    mPlayerBoard->setPosition (121, 140); //ally position
	mSceneLayers[BoardLayer]->attachChild(std::move(boardSprite));

    mPlayerBoard->setName (mPlayer.name);
    mPlayerBoard->defeated = false;

    net.sendName (mPlayer.name);
    net.sendShips(mPlayer.ships);
    cout << "-- after send ships -- \n";

    player1Name.setColor (LIGHT_BLUE);
    player2Name.setColor (LIGHT_BLUE);

    centerOrigin(player1Name);
    centerOrigin(player2Name);

    player1Name.setString (mPlayer.name);
    centerOrigin(player1Name);

    player1Name.setPosition (324, 678);
    player2Name.setPosition (610, 678);

    player1Photo.setPosition (287, 576);
    player2Photo.setPosition (571, 576);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}
