#include "headers/Board.hpp"
#include "headers/Utility.hpp"
#include "headers/CommandQueue.hpp"
#include "headers/ResourceHolder.hpp"


#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

#include <cmath>
#include <iostream>
#include <utility>

using namespace std;
using namespace sf;

Board::Board(const TextureHolder& textures, const FontHolder& fonts, GameNetwork& net_, int nRows, int nCols, 
    int player_ID, int& player_turn, std::vector<Ship> & playerShips_, mapOptions& battleOpts)
: boardSprites(nRows, vector<sf::Sprite>(nCols)), mTextures (textures), mFonts (fonts), net(net_),
cursorI (0), cursorJ (0), boardState (nRows, vector<SquareState> (nCols, SquareState::Covered)),
mnRows (nRows), mnCols (nCols), cursorSprite (mTextures.get(Textures::BoardSelected)), shootsFired(0),
playerID(player_ID), playerTurn (player_turn), totalFired(0),
nShoots(battleOpts["fireRate"]), giveBonus(battleOpts["withBonus"]), nMines(battleOpts["mines"]), nDestroyed (5),
islandSprite (mTextures.get(Textures::BoardIsland)), withIsland(battleOpts["island"]), withCloud(battleOpts["cloud"]),
isNight (battleOpts["night"]), shipNames  (7, sf::Text{"", fonts.get(Fonts::Bold) , 16}), 
totalFiredText ("00", fonts.get(Fonts::Bold) , 16),
fullboard (mTextures.get(Textures::fullboard))
{
    if (playerID == 2) {
        boardTitle = sf::Text ("Enemy Battlefield", mFonts.get(Fonts::Bold), 24);
    }
    else {
        boardTitle = sf::Text ("Ally Battlefield" , mFonts.get(Fonts::Bold), 24);
    }

    shipNames[0].setString("Aircraft Carrier");
    shipNames[1].setString("Battleship");
    shipNames[2].setString("Submarine");
    shipNames[3].setString("Cruiser");
    shipNames[4].setString("Destroyer");
    if (nMines > 0)
        shipNames[5].setString("Mines");

	float positionX = 0;
	float positionY = 0;
	for (auto& row : boardSprites) {
		for (auto& element : row) {
			element.setTexture(mTextures.get(Textures::BoardCovered), true);
			element.setPosition(positionX, positionY);
			positionX += 30;
		}
		positionY += 30;
		positionX = 0;
	}

    cursorSprite.setOrigin (0, 0);

    fullboard.setPosition(0, 0);

    positionY = 480; //hacky: missiles are drawed player reverted...
    positionX = 261     +152;
    if (playerID == 2)
        positionX = 22 -152;

    totalFiredText.setColor (DARK_BLUE);
    if (playerID == 1)
        totalFiredText.setPosition (positionX+6, positionY+59);
    else
        totalFiredText.setPosition (positionX+1, positionY+59);

    for (int i = 0; i < nShoots; ++i) {
        missiles.emplace_back(mTextures.get(Textures::Missile));
        missiles[i].setPosition (positionX, positionY);
        positionY -= 35;
    }

    shipSprites.emplace_back (mTextures.get(Textures::CarrierGray));
    destroyedTextures.push_back (mTextures.get(Textures::DestroyedCarrier));
    shipSprites.emplace_back (mTextures.get(Textures::BattleshipGray));
    destroyedTextures.push_back (mTextures.get(Textures::DestroyedBattleship));
    shipSprites.emplace_back (mTextures.get(Textures::SubmarineGray));
    destroyedTextures.push_back (mTextures.get(Textures::DestroyedSubmarine));
    shipSprites.emplace_back (mTextures.get(Textures::CruiserGray));
    destroyedTextures.push_back (mTextures.get(Textures::DestroyedCruiser));
    shipSprites.emplace_back (mTextures.get(Textures::DestroyerGray));
    destroyedTextures.push_back (mTextures.get(Textures::DestroyedDestroyer));

    destroyedTextures.push_back  (mTextures.get(Textures::DestroyedMine)); //mine extra case...

    for (int i = 0; i < nMines; ++i) { //mine extra case...
        shipSprites.emplace_back (mTextures.get(Textures::MineGray));
    }

    int shX = -41 , shY = 430, ind = 0;
    if (playerID == 2) shX = 215;
    for (auto& shipspr : shipSprites) {
        centerOrigin (shipspr);
        shipspr.setScale (0.75, 0.75);
        shipspr.setPosition (shX + (140 * (ind%2)), shY + (ind/2)*45);

        shipNames[ind].setColor (DARK_BLUE);
        centerOrigin (shipNames[ind]);
        shipNames[ind].setPosition (shX + (140 * (ind%2)), shY + 13 + (ind/2)*45);

        ++ind; 
    }
    shipSprites[0].move(0, +5); //adjust for carrier
      shipNames[0].move(0, +5);

    if (nMines > 1) {
        shipSprites[5].move (-22, 0);
        shipNames[5].move   (-22, 0);
        shipSprites[6].move (140+8, -45);
    }

    topSprites.emplace_back (mTextures.get(Textures::Carrier));
    topSprites.emplace_back (mTextures.get(Textures::Battleship));
    topSprites.emplace_back (mTextures.get(Textures::Submarine));
    topSprites.emplace_back (mTextures.get(Textures::Cruiser));
    topSprites.emplace_back (mTextures.get(Textures::Destroyer));

    for (int i = 0; i < nMines; ++i)
        topSprites.emplace_back (mTextures.get(Textures::Mine));

    cout << "isNight: " << isNight << "playerID: " << playerID << " expression: " << (isNight and playerID == 2) << "\n";
    if (isNight and playerID == 2)
        positionY = -500;
    else 
        positionY = (30 * mnCols) + 25;

    defeated = false;

    boardTitle.setPosition ( 0 , -30);

    ships =  playerShips_;

    cout << "BACK: " << ships.back();

    if (playerID == 1) {

        cout << "\n";
        for (auto& ship : ships)
            cout << ship;

        float x, y;
        int sIndex, rot;
        sIndex = 0;
        for (auto& shp : ships) {
            if (shp.name == ShipID::Island) {
                islandSprite.setOrigin   (0, 0);
                islandSprite.setPosition (30*shp.positions[0].j, 30*shp.positions[0].i);
                continue;
            }

            auto rot = shp.vertical() ? 1 : 0;
            if (rot) {
                topSprites[sIndex].setOrigin(0, 0);
                topSprites[sIndex].setRotation(90);
                topSprites[sIndex].setOrigin(0, 25);
            } else {
                topSprites[sIndex].setOrigin(0, 0);
                topSprites[sIndex].setRotation(0);
            }
            topSprites[sIndex].setPosition (30*shp.positions[0].j, 30*shp.positions[0].i);

            ++sIndex;
        }
    } 
}

void Board::reset () {
    fired = false;
    shootsFired =  0;
    gainedBonus = false;
}

void Board::setName(string playerName) {
    boardTitle.setString (playerName);
}

void Board::fire (int dueToMine /*default to false */) {
    
    //If the board is owned by the player in turn
    //then ignore board commands

    int i = cursorI; int j = cursorJ;
    if (boardState[i][j] != SquareState::Covered) return;


    for (int id = 0; id < ships.size(); ++id) {
        for (int pos = 0; pos < ships[id].positions.size(); ++pos) {
            if (i == ships[id].positions[pos].i and j == ships[id].positions[pos].j) {

                if (ships[id].name == ShipID::Island)
                    boardState[i][j] = SquareState::Island;    
                else
                    boardState[i][j] = SquareState::Fire;

                if (ships[id].name == ShipID::Mine) {
                    enemyBoard->cursorI = ships[id].positions[pos].i;
                    enemyBoard->cursorJ = ships[id].positions[pos].j;
                    enemyBoard->fire(true);
                    net.sendMine(ships[id].positions[pos].i, ships[id].positions[pos].j);
                }
                ships[id].positions.erase(begin(ships[id].positions) + pos);

                bool notShipOrMine = (ships[id].name != ShipID::Island) and (ships[id].name != ShipID::Mine);

                if (ships[id].positions.empty() and ships[id].name != ShipID::Island) {
                    shipSprites[id].setTexture (destroyedTextures[id < 5 ? id : 5]);
                    if (notShipOrMine)
                        --nDestroyed;
                }
                if (dueToMine) return;
                ++shootsFired;
                ++totalFired;
                if (notShipOrMine and giveBonus and not gainedBonus) {
                    --shootsFired;
                    gainedBonus = true;
                }
                if (shootsFired == nShoots) {
                    playerTurn = playerTurn == 1 ? 2 : 1;
                    shootsFired = 0;
                    fired = true;
                }
                return;
            }
        }
    }
    boardState[i][j] = SquareState::Sea;
    if (dueToMine) return;
    ++shootsFired;
    ++totalFired;
    if (shootsFired == nShoots) {
        playerTurn = playerTurn == 1 ? 2 : 1;
        shootsFired = 0;
        fired = true;
    }
}



unsigned int Board::getCategory() const
{
	return Category::Board;
}

void Board::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (playerID == 1 and withIsland)
        target.draw (islandSprite, states);

	for (auto& spriteRow : boardSprites)
		for (auto& square : spriteRow)
			target.draw(square, states);

   target.draw (fullboard, states);

    if (playerID == 1) 
        for (auto& topSprite : topSprites)
            target.draw (topSprite, states);

    for (int i = 0; i < boardSprites.size(); ++i)
        for (int j = 0; j < boardSprites[0].size(); ++j)
            if (boardState[i][j] == SquareState::Fire)
                target.draw(boardSprites[i][j], states);

	if ((playerTurn == 1) and (playerID == 2)) {
		target.draw (cursorSprite, states);
    }

    target.draw (boardTitle, states);

    for (auto& shipSprite : shipSprites)
        target.draw (shipSprite, states);

    for (auto& shipName : shipNames)
        target.draw (shipName, states);

    if (playerTurn != playerID)
        for (int i = 0; i < nShoots - shootsFired; ++i)
            target.draw(missiles[i], states);

    target.draw(totalFiredText, states);
}   

void Board::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateBoard();
    //Matrix (I,J) notation is inverted from X-Y notation:
	cursorSprite.setPosition (cursorJ * 30, cursorI * 30);
    if (!nDestroyed) {
        cout << "all destroyed...\n";
        defeated = true; 
    }

    if (playerTurn == playerID)
        boardTitle.setColor(sf::Color::Cyan);
    else
        boardTitle.setColor(sf::Color::Red);

    if (totalFired < 10)
        totalFiredText.setString (std::string("0") + toString(totalFired));
    else
        totalFiredText.setString(toString(totalFired));
}

void Board::updateBoard () {

    if (playerTurn != playerID) {
        fullboard.setTexture (mTextures.get(Textures::fullboardSelected), true);
    } else {
        fullboard.setTexture (mTextures.get(Textures::fullboard), true);
    }

	for (int i = 0; i < mnRows; ++i) 
		for (int j = 0; j < mnCols; ++j) {
			SquareState square = boardState[i][j];
			switch (square)
			{
				case SquareState::Covered:
					boardSprites[i][j].setTexture(mTextures.get(Textures::BoardCovered));
					break;
				case SquareState::Sea:
					boardSprites[i][j].setTexture(mTextures.get(Textures::BoardSea));
					break;
				case SquareState::Fire:
					boardSprites[i][j].setTexture(mTextures.get(Textures::BoardFire));
					break;
                case SquareState::Island:
                    boardSprites[i][j].setTexture(mTextures.get(Textures::IslandSquare));
                    break;
			}
		}
}