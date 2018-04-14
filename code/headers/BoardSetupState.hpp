#ifndef BOARDSETUPSTATE_HPP
#define BOARDSETUPSTATE_HPP

#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "SpriteTimer.hpp"
#include "ScreenButton.hpp"

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"

enum shipBoardIds {Carrier_id = 0, Battleship_id, Submarine_id, Cruiser_id, Destroyer_id, Mine_id};

enum class selectionState {ship, board};

struct shipInfo {
        int i=-500, j=-500, rot=0, size;
        shipInfo (int s) {
                size = s;
        }
};

class BoardSetupState : public State
{
	public:
 
        BoardSetupState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);

        void                loadTextures ();
        void                buildScene ();
        void                buildIsland();

        void removeShip (int shipIndex);
        void putOnPosition (int shipIndex);
        void adjustShipPosition (int shipIndex);
        bool isOccupied(int i, int j);
        void shuffle();
        void finalShuffle();
        void selectPrevious ();
        void selectNext ();
        bool checkSurround(int i, int j);

        void startGame();

        int                                   nMines;
	
        Player&				      mPlayer;
        TextureHolder&                        mTextures;
        std::vector<sf::Sprite>               boardSetupFields;
        std::vector<sf::Text>                 shipNames;
        std::vector<std::vector<sf::Sprite>>  boardSprites;
        std::vector<sf::Sprite>               shipSprites;
        std::vector<sf::Sprite>               shipsOnBoard;
        std::vector<sf::Texture>              unselectedTextures;
        std::vector<sf::Texture>              selectedTextures;
        std::vector<sf::Texture>              grayTextures;
        std::vector<sf::Texture>              topTextures;
        std::vector<shipInfo>                 shipsInfo;
        std::vector<int>                      unavailable;
        std::vector<std::vector<int>>         occupied;
        int                                   nShipsSelected;
        int                                   selectedShip;
        int                                   selectedField;
        bool                                  withIsland;
        selectionState                        selecting;
        sf::Sprite                            islandSprite;
        sf::Sprite                            background;

        ScreenButton                          readyButton;
        ScreenButton                          shuffleButton;
        
        sf::Sprite                            fullboard;
        sf::Text                              fleetText;
        SpriteTimer                           setupTimer;
        CommandQueue                          ignoreCommands;

        int sizeWithoutMines;
        Ship islandShip; //it's not really a ship... but the struct is a convenient way to save a island!
};

#endif // BOARDSETUPSTATE_HPP