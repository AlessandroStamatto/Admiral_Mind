#include "../headers/BoardSetupState.hpp"
#include "../headers/Utility.hpp"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"

#include <vector>
#include <iostream>

#include "../headers/event_macros.hpp"

using namespace std;
using namespace sf;

#define BOARD_X_BASE  392
#define BOARD_Y_BASE  229

BoardSetupState::BoardSetupState(StateStack& stack, Context context)
: State(stack, context)
, mPlayer(*context.player)
, mTextures (*context.textures)
, boardSprites (context.player->settings.boardNRows , std::vector<sf::Sprite> (context.player->settings.boardNCols) )
, occupied     (context.player->settings.boardNRows , std::vector<int>        (context.player->settings.boardNCols, 0))
, nMines ( (*context.battleOpts)["mines"] ), withIsland((*context.battleOpts)["island"])
, shipsOnBoard (5 + nMines)
, shipSprites (5 + nMines), unavailable (5 + nMines, 0)
, selectedShip (Carrier_id), selecting (selectionState::ship), selectedField(0)
, nShipsSelected (5 + nMines), setupTimer (45, *context.fonts, *context.textures)
, fleetText  ("Fleet", context.fonts->get(Fonts::Normal), 26)
, shipNames  (6, sf::Text{"", context.fonts->get(Fonts::Bold) , 16})
, boardSetupFields (6, Sprite{context.textures->get(Textures::boardSetupField)})
, readyButton ("Ready", context.fonts->get(Fonts::Normal), context.textures->get(Textures::bigButton), 36)
, shuffleButton ("Shuffle", context.fonts->get(Fonts::Normal), context.textures->get(Textures::bigButton), 36)
{
    sizeWithoutMines = 5;
    shipsInfo = {{5}, {4}, {3}, {3}, {2}};
    for (int i = 0; i < nMines; ++i) shipsInfo.emplace_back (1);

    context.player->ships = std::vector<Ship> (5 + nMines);

    for (int i = 0; i < 5; ++i)
        context.player->ships[i].name = static_cast<ShipID> (i);
    for (int i = sizeWithoutMines; i < sizeWithoutMines + nMines; ++i)
        context.player->ships[i].name = ShipID::Mine;

    loadTextures();
    buildScene();
    if (withIsland)
        buildIsland();
}

void BoardSetupState::loadTextures ()
{
    static bool notLoaded = true;
    if (notLoaded) {
        mTextures.load(Textures::fullboard,                 "Media/Textures/board/board.png");
        mTextures.load(Textures::fullboardSelected,         "Media/Textures/board/boardSelected.png");

        mTextures.load(Textures::BoardSetupBackground,        "Media/Textures/screens/background.BoardSetup.png");
        mTextures.load(Textures::BoardShip,                   "Media/Textures/board/ship.png");
        mTextures.load(Textures::BoardCovered,                "Media/Textures/board/covered.png");
        mTextures.load(Textures::BoardIsland,                 "Media/Textures/board/island.png");

        mTextures.load(Textures::Carrier,      "Media/Textures/ships/carrier.png");
        mTextures.load(Textures::Battleship,   "Media/Textures/ships/battleship.png");
        mTextures.load(Textures::Submarine,    "Media/Textures/ships/submarine.png");
        mTextures.load(Textures::Cruiser,      "Media/Textures/ships/cruiser.png");
        mTextures.load(Textures::Destroyer,    "Media/Textures/ships/destroyer.png");
        mTextures.load(Textures::Mine,         "Media/Textures/ships/mine.png");

        mTextures.load(Textures::CarrierSelected,      "Media/Textures/ships/carrierSelected.png");
        mTextures.load(Textures::BattleshipSelected,   "Media/Textures/ships/battleshipSelected.png");
        mTextures.load(Textures::SubmarineSelected,    "Media/Textures/ships/submarineSelected.png");
        mTextures.load(Textures::CruiserSelected,      "Media/Textures/ships/cruiserSelected.png");
        mTextures.load(Textures::DestroyerSelected,    "Media/Textures/ships/destroyerSelected.png");
        mTextures.load(Textures::MineSelected,         "Media/Textures/ships/mineSelected.png");

        mTextures.load(Textures::CarrierGray,      "Media/Textures/ships/carrierGray.png");
        mTextures.load(Textures::BattleshipGray,   "Media/Textures/ships/battleshipGray.png");
        mTextures.load(Textures::SubmarineGray,    "Media/Textures/ships/submarineGray.png");
        mTextures.load(Textures::CruiserGray,      "Media/Textures/ships/cruiserGray.png");
        mTextures.load(Textures::DestroyerGray,    "Media/Textures/ships/destroyerGray.png");
        mTextures.load(Textures::MineGray,         "Media/Textures/ships/mineGray.png");


        mTextures.load(Textures::Ready,       "Media/Textures/button/ready.png"  );
        mTextures.load(Textures::NotReady,    "Media/Textures/button/notReady.png"  );
        mTextures.load(Textures::Shuffle,     "Media/Textures/button/shuffle.png");
        notLoaded = false;
    }

    unselectedTextures = {
                        mTextures.get(Textures::Carrier),
                        mTextures.get(Textures::Battleship),
                        mTextures.get(Textures::Submarine),
                        mTextures.get(Textures::Cruiser),
                        mTextures.get(Textures::Destroyer)
                    };
                    if (nMines)
                        unselectedTextures.emplace_back (mTextures.get(Textures::Mine));

    selectedTextures = {
                        mTextures.get(Textures::CarrierSelected),
                        mTextures.get(Textures::BattleshipSelected),
                        mTextures.get(Textures::SubmarineSelected),
                        mTextures.get(Textures::CruiserSelected),
                        mTextures.get(Textures::DestroyerSelected)
                    };
                    if (nMines)
                        selectedTextures.emplace_back (mTextures.get(Textures::MineSelected));

    grayTextures = {
                        mTextures.get(Textures::CarrierGray),
                        mTextures.get(Textures::BattleshipGray),
                        mTextures.get(Textures::SubmarineGray),
                        mTextures.get(Textures::CruiserGray),
                        mTextures.get(Textures::DestroyerGray)
                    };
                    if (nMines)
                        grayTextures.emplace_back (mTextures.get(Textures::MineGray));

}

void BoardSetupState::buildIsland () {
    int i = randomInt(occupied.size() - 1);
    int j = randomInt(occupied.size() - 1);
    occupied[i][j] = 1;
    occupied[i][j+1] = 1;
    occupied[i+1][j] = 1;
    occupied[i+1][j+1] = 1;
    cout << "island in: " << i << " " << j << "\n";

    islandSprite.setTexture (mTextures.get(Textures::BoardIsland), true);
    islandSprite.setPosition (BOARD_X_BASE + 30*j, BOARD_Y_BASE + 30*i);

    islandShip.name = ShipID::Island; //it's not really a ship... but the struct is a convenient way to save a island!
    islandShip.positions = {{i, j}, {i, j+1}, {i+1, j}, {i+1, j+1}};
    cout << islandShip;
}

void BoardSetupState::buildScene ()
{
    background.setTexture (mTextures.get(Textures::BoardSetupBackground), true);

    int xfld = 114, yfld = 278;
    for (auto& fld : boardSetupFields) {
        fld.setPosition (xfld, yfld);
        yfld += 60;
    }

    shuffleButton.setPosition(407, 560);
    readyButton.setPosition (574, 560);

    setupTimer.setPosition(97, 145);

    fullboard.setTexture (mTextures.get(Textures::fullboardSelected), true);
    fullboard.setPosition(392, 229);

    for (int i = 0; i < sizeWithoutMines; ++i)
        shipSprites[i].setTexture (unselectedTextures[i]);
    for (int i = sizeWithoutMines; i < sizeWithoutMines + nMines; ++i)
        shipSprites[i].setTexture (unselectedTextures[sizeWithoutMines]);

    if (selectedShip < sizeWithoutMines)
        shipSprites[selectedShip].setTexture (selectedTextures[selectedShip]);
    else
        shipSprites[selectedShip].setTexture (selectedTextures[sizeWithoutMines]);

    int positionX = 392;
    int positionY = 229;
    for (auto& row : boardSprites) {
        for (auto& element : row) {
            element.setOrigin (0, 0);
            element.setTexture(mTextures.get(Textures::BoardCovered), true);
            element.setPosition(positionX, positionY);
            positionX += 30;
        }
        positionY += 30;

        positionX = 392;
    }

    shipNames[0].setString("Aircraft Carrier");
    shipNames[1].setString("Battleship");
    shipNames[2].setString("Submarine");
    shipNames[3].setString("Cruiser");
    shipNames[4].setString("Destroyer");
    shipNames[5].setString("Underwater Mines");

    int nameWidth;
    positionX = 287;
    positionY = 315;
    for (auto& shipName : shipNames) {
        shipName.setColor (DARK_BLUE);
        nameWidth = shipName.getLocalBounds().width;
        shipName.setOrigin (nameWidth, 0);
        shipName.setPosition (positionX, positionY);
        positionY += 60;   
    }

    xfld = 114 + 90; yfld = 278 + 30;
    for (auto& element : shipSprites) {
        element.setOrigin (element.getLocalBounds().width / 2, 13);
        element.setPosition (xfld, yfld);
        yfld += 60;
    }
    shipSprites[0].move (0, -10); //adjust carrier
    if (shipSprites.size() > 6) { //adjust if there is 2 mines
        shipSprites[5].move (-26, -10);
        shipSprites[6].move (+26, -60 - 10);
    }

    int shipIndex = 0;
    for (auto& ship : shipsOnBoard) {
        ship.setTexture  (unselectedTextures[shipIndex], true);
        ship.setPosition (-500 , -500);
        if (shipIndex < sizeWithoutMines)
            ++shipIndex;
    }

    fleetText.setColor    (DARK_BLUE);
    fleetText.setPosition (133, 245);
}

void BoardSetupState::draw()
{

    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());
    window.draw(background);
    for (auto& fld : boardSetupFields) 
        window.draw(fld);
    window.draw(islandSprite);

    window.draw(fullboard);

    for (auto& shipSprite : shipSprites)
        window.draw (shipSprite);

    for (auto& shipName : shipNames)
        window.draw (shipName);

    int shipIndex = 0;
    for (auto& shipOnBoard : shipsOnBoard) {
        if (unavailable[shipIndex])
            window.draw (shipOnBoard);
        ++shipIndex;
    }

    window.draw (shipsOnBoard[selectedShip]);
    window.draw (readyButton);
    window.draw (shuffleButton);
    window.draw (setupTimer);
    window.draw (fleetText);

    // for (auto& row : boardSprites)
    //     for (auto& element : row)
    //         window.draw(element);
}   

void BoardSetupState::startGame() {
    finalShuffle();

    if (withIsland)
        // island is inserted as the "last ship", hacky...
        mPlayer.ships.push_back(islandShip); 

    requestStateClear();
    requestStackPush(States::Game);
}

bool BoardSetupState::update(sf::Time dt)
{
    setupTimer.update(dt, ignoreCommands);

    int shipIndex = 0;
    for (auto& ship : shipsInfo) {
        if (ship.rot) {
            shipsOnBoard[shipIndex].setOrigin(0, 0);
            shipsOnBoard[shipIndex].setRotation(90);
            shipsOnBoard[shipIndex].setOrigin(0, 25);
        } else {
            shipsOnBoard[shipIndex].setOrigin(0, 0);
            shipsOnBoard[shipIndex].setRotation(0);
        }

        shipsOnBoard[shipIndex].setPosition (BOARD_X_BASE + 30*ship.j, BOARD_Y_BASE + 30*ship.i);
        
        if (shipIndex < sizeWithoutMines){
            if (unavailable[shipIndex])
                shipSprites[shipIndex].setTexture (grayTextures[shipIndex]);
            else if (shipIndex == selectedShip)
                shipSprites[shipIndex].setTexture (selectedTextures[shipIndex]);
            else
                shipSprites[shipIndex].setTexture (unselectedTextures[shipIndex]);
        } else {
            if (unavailable[shipIndex])
                shipSprites[shipIndex].setTexture (grayTextures[sizeWithoutMines]);
            else if (shipIndex == selectedShip)
                shipSprites[shipIndex].setTexture (selectedTextures[sizeWithoutMines]);
            else
                shipSprites[shipIndex].setTexture (unselectedTextures[sizeWithoutMines]);
        }

        ++shipIndex;
    }
    for (auto& bsfield : boardSetupFields)
        bsfield.setTexture (mTextures.get(Textures::boardSetupField));
    boardSetupFields[selectedField].setTexture (mTextures.get(Textures::boardSetupFieldSelected));

    // if (!nShipsSelected) //fix ready button to go gray
    //     readySprite.setTexture(mTextures.get(Textures::Ready), true);

    if (setupTimer.timeEnded()) {
        startGame();
    }

    getContext().net->update();

	return true;
}

void BoardSetupState::adjustShipPosition (int shipIndex) {
    int shipRot = shipsInfo[shipIndex].rot, shipI = shipsInfo[shipIndex].i, shipJ = shipsInfo[shipIndex].j;
    int shipSize = shipsInfo[shipIndex].size;
    if (shipRot) {
        if (shipJ >= boardSprites.size())
            shipsInfo[shipIndex].j = boardSprites.size() -1;
        if (shipI + shipSize >= boardSprites.size())
            shipsInfo[shipIndex].i = boardSprites.size() - shipsInfo[shipIndex].size;
    }
    else {
        if (shipsInfo[shipIndex].j + shipSize >= boardSprites.size())
            shipsInfo[shipIndex].j = boardSprites.size() - shipsInfo[shipIndex].size;
        if (shipI >= boardSprites.size())
            shipsInfo[shipIndex].i = boardSprites.size() -1;
    }
}

void BoardSetupState::removeShip (int shipIndex) {
    if (not unavailable[shipIndex])
        return;
    for (auto& pos : mPlayer.ships[shipIndex].positions)
        occupied[pos.i][pos.j] = 0;

    mPlayer.ships[shipIndex].positions.clear();
    unavailable[shipIndex] = 0;
    ++nShipsSelected;
}

bool BoardSetupState::isOccupied (int i, int j) {
    if ((i >=0) and (i < occupied.size()) and
        (j >= 0) and (j < occupied[0].size()))
        return occupied[i][j];

    return false;
}

bool BoardSetupState::checkSurround(int i, int j) {
    std::vector<sPos> squareSurround = {
        {i-1, j-1}, {i-1, j}, {i-1, j+1},
        {i, j-1  },/*       */{i, j+1  },
        {i+1, j-1}, {i+1, j}, {i+1, j+1}
    };

    for (auto& ele : squareSurround)
         if (isOccupied(ele.i, ele.j))
            return true;

    return false;
}

void BoardSetupState::finalShuffle() {
    for (int shipIndex = 0; shipIndex < unavailable.size(); ++shipIndex) {
        while (not unavailable[shipIndex]) {
            shipsInfo[shipIndex].rot = randomInt(2);
            shipsInfo[shipIndex].i = randomInt(occupied.size());
            shipsInfo[shipIndex].j = randomInt(occupied[0].size());
            putOnPosition (shipIndex);
        }
    }
}

void BoardSetupState::shuffle() {
    for (int shipIndex = 0; shipIndex < unavailable.size(); ++shipIndex) {
        removeShip(shipIndex);
        while (not unavailable[shipIndex]) {
            shipsInfo[shipIndex].rot = randomInt(2);
            shipsInfo[shipIndex].i = randomInt(occupied.size());
            shipsInfo[shipIndex].j = randomInt(occupied[0].size());
            putOnPosition (shipIndex);
        }
    }
}

void BoardSetupState::putOnPosition (int shipIndex) {
    removeShip (shipIndex);
    adjustShipPosition(shipIndex);

    int shipRot = shipsInfo[shipIndex].rot, shipI = shipsInfo[shipIndex].i, shipJ = shipsInfo[shipIndex].j;
    int shipSize = shipsInfo[shipIndex].size;
    if (shipRot == 0) {
        bool notOccupied = true;
        for (int j = shipJ; j < shipJ + shipSize; ++j) {
            if (occupied[shipI][j])
                return;
        }
        if (not getContext().battleOpts->at("allowAttached")) {
            if (shipI > 0)
                for (int j = shipJ; j < shipJ + shipSize; ++j)
                    if (occupied[shipI-1][j])
                        return;
            if (shipI < occupied.size() - 1)
                for (int j = shipJ; j < shipJ + shipSize; ++j)
                    if (occupied[shipI+1][j])
                        return;
            
            for (int j = shipsInfo[shipIndex].j; j < shipsInfo[shipIndex].j + shipsInfo[shipIndex].size; ++j)
                if (checkSurround(shipsInfo[shipIndex].i, j))
                    return;
        }

        if (notOccupied) {
            for (int j = shipJ; j < shipJ + shipSize; ++j) {
                occupied[shipI][j] = 1;
                mPlayer.ships[shipIndex].positions.push_back ({shipI, j});
            }
        }
    } else {
        bool notOccupied = true;
        for (int i = shipI; i < shipI + shipSize; ++i)
            if (occupied[i][shipJ])
                return;
        if (not getContext().battleOpts->at("allowAttached")) {
            if (shipJ > 0)
                for (int i = shipI; i < shipI + shipSize; ++i)
                    if (occupied[i][shipJ-1])
                        return;
            if (shipJ < occupied[0].size() - 1)
                for (int i = shipI; i < shipI + shipSize; ++i)
                    if (occupied[i][shipJ+1])
                        return;
            
            for (int i = shipsInfo[shipIndex].i; i < shipsInfo[shipIndex].i + shipsInfo[shipIndex].size; ++i)
                if (checkSurround(i, shipsInfo[shipIndex].j))
                    return;
        }

        if (notOccupied) {
            for (int i = shipI; i < shipI + shipSize; ++i) {
                occupied[i][shipJ] = 1;
                mPlayer.ships[shipIndex].positions.push_back ({i, shipJ});
            } 
        }
    }

    //If function reaches here, then it successfully putted the ship on board
    unavailable[shipIndex] = 1;
    --nShipsSelected;
    selecting = selectionState::ship;
}

void BoardSetupState::selectPrevious () {
    do {
        selectedShip = selectedShip == 0 ? shipSprites.size() - 1 : selectedShip - 1;
    } while (unavailable[selectedShip] and nShipsSelected > 0);
    selectedField = selectedShip < 6 ? selectedShip : 5;
}

void BoardSetupState::selectNext () {
    do {
        selectedShip = (selectedShip + 1) % shipSprites.size();
    } while (unavailable[selectedShip] and nShipsSelected > 0);
    selectedField = selectedShip < 6 ? selectedShip : 5;
}

bool BoardSetupState::handleEvent(const sf::Event& event)
{
////////////////////////////////// - SELECTING A SHIP - ///////////////////////////////////////////////////////////
    if (selecting == selectionState::ship) {
        if (KEY_RELEASED) {
            if (ON_KEY(W) || ON_KEY(Up)) {
                selectPrevious();
            }
            else if (ON_KEY(S) || ON_KEY(Down)) {
                selectNext();
            }
            else if (ON_KEY(Return) || ON_KEY(Space)) {
                selecting = selectionState::board;
            }
        }

        if (MOUSE_MOVED) {
            for (int i = 0; i < shipSprites.size(); ++i)
                if ( MOUSE_MOVED_ON(shipSprites[i]) and not unavailable[i]) {
                        selectedShip = i;
                }
            for (int i = 0; i < boardSetupFields.size(); ++i)
                if (MOUSE_MOVED_ON(boardSetupFields[i])) {
                        selectedField = i;
                }

        } else if (MOUSE_PRESSED) {
            if (MOUSE_BUTTON(Left)) {
                for (int i = 0; i < shipSprites.size(); ++i)
                    if ( MOUSE_BUTTON_ON(shipSprites[i]) and not unavailable[i] ) {  
                        selecting = selectionState::board;
                }
                for (int i = 0; i < shipsOnBoard.size(); ++i)
                    if (MOUSE_BUTTON_ON(shipsOnBoard[i])) {
                        selectedShip = i;
                        removeShip(selectedShip);
                        selecting = selectionState::board;
                    }
            
                if (MOUSE_BUTTON_ON(readyButton)) {
                    if (!nShipsSelected)
                        startGame();
                }
                if (MOUSE_BUTTON_ON(shuffleButton)) {
                    shuffle();
                }
            }
        }
    }
///////////////////////////////////// - MOVING AND POSITIONING A SHIP - //////////////////////////////////////////////////////////
      else if (selecting == selectionState::board) {
        if (KEY_RELEASED) {
            if      (ON_KEY(W) || ON_KEY(Up))
                --shipsInfo[selectedShip].i;
            else if (ON_KEY(S) || ON_KEY(Down))
                ++shipsInfo[selectedShip].i;
            else if (ON_KEY(D) || ON_KEY(Right))
                ++shipsInfo[selectedShip].j;
            else if (ON_KEY(A) || ON_KEY(Left))
                --shipsInfo[selectedShip].j;
            else if (ON_KEY(R) || ON_KEY(F))
                shipsInfo[selectedShip].rot = shipsInfo[selectedShip].rot == 0 ? 1 : 0;
            else if (ON_KEY(Return) || ON_KEY(Space))
                putOnPosition(selectedShip);

            adjustShipPosition(selectedShip);

        } else if (MOUSE_MOVED) {
            for (int i = 0; i < boardSprites.size(); ++i)
                    for (int j = 0; j < boardSprites[0].size(); ++j)
                        if (MOUSE_MOVED_ON(boardSprites[i][j])) {
                            shipsInfo[selectedShip].i = i;
                            shipsInfo[selectedShip].j = j;
                            adjustShipPosition(selectedShip);
                        }
        } else if (MOUSE_PRESSED) {

            if        (MOUSE_BUTTON(Right)) {
                shipsInfo[selectedShip].rot = shipsInfo[selectedShip].rot == 0 ? 1 : 0;
                adjustShipPosition(selectedShip);

            } else if (MOUSE_BUTTON(Left))  {
                putOnPosition(selectedShip);
            }
        } else if (MOUSE_RELEASED) {
            if (MOUSE_BUTTON(Left))
                putOnPosition(selectedShip);
        } 
    }
}