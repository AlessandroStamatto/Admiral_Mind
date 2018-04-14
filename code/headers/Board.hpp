#ifndef BOARD_HPP
#define BOARD_HPP

#include "SceneNode.hpp"
#include "Command.hpp"
#include "ResourceHolder.hpp"
#include "GameDatatypes.hpp"
#include "GameNetwork.hpp"
#include "colorDefines.hpp"

#include "SFML/Graphics.hpp"
#include <vector>
#include <string>

class Board : public SceneNode
{
	public:
								Board(const TextureHolder& textures, const FontHolder& fonts, GameNetwork& net_, int rowSize, int colSize, 
									int playerID, int& playerTurn, std::vector<Ship> & playerShips_, mapOptions& battleOpts);
				void			reset ();
				void			setName(std::string playerName);
			    void            updateBoard ();
			    void            fire (int dueToMine = false);
			    unsigned int    getCategory() const;

		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);

		const TextureHolder& mTextures;
		const FontHolder&    mFonts;
		bool fired = false;
		int nMines;
		std::vector<std::vector<sf::Sprite>>  boardSprites;
        std::vector<sf::Sprite>               shipSprites;
        std::vector<sf::Sprite>               topSprites;
		std::vector<std::vector<SquareState>> boardState;
		std::vector<Ship>	                  ships; 	  
        sf::Sprite                            fullboard;
		int                                   cursorI, cursorJ;
		int                                   mnRows, mnCols;
		bool								  giveBonus;
		bool							      withIsland = false;
		bool								  withCloud = false;
		bool								  gainedBonus = false;
        bool                                  isNight = false;
		int                                   nShoots, shootsFired, nDestroyed;
		sf::Sprite 							  cursorSprite, islandSprite;
		sf::Text   							  boardTitle;
		std::vector<sf::Text>                 shipNames;
		int   								  playerID;
		bool  								  defeated;
		int& 	 							  playerTurn;
		int                                   wxoffset, wyoffset;
		int                                   totalFired;
		sf::Text                              totalFiredText;
		GameNetwork&						  net;
        std::vector <sf::Texture>             destroyedTextures;
        std::vector <sf::Sprite>			  missiles;
        Board*                                enemyBoard;
};

#endif // BOARD_HPP
