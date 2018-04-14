#ifndef HOMESTATE_HPP
#define HOMESTATE_HPP

#include "State.hpp"
#include "TextNode.hpp"
#include "BattleOptionsSprite.hpp"
#include "SFML/Graphics.hpp"

#include <array>
#include <memory>
#include <string>

class HomeState : public State
{
	public:
								HomeState(StateStack& stack, Context context);

		virtual void			draw();
		virtual bool			update(sf::Time dt);
		virtual bool			handleEvent(const sf::Event& event);
                void            challengePlayer(int opponentID);
                void            startBattle(int opponentID);
                void            loadRanking();

        sf::Sprite                                                mBackgroundSprite;
        sf::Sprite                                                rollbarSprite;
        sf::Sprite                                                rollSprite;
        sf::Sprite                                                player_1;
        sf::Sprite                                                player_2;
        sf::Sprite                                                popup;
        bool                                                      showPopup = false;
        std::vector<playerInfo>                                   playersInfo;
        std::vector<std::unique_ptr<TextNode>>                    playerlistTexts;
        std::vector<std::unique_ptr<BattleOptionsSprite>>         playerlistBattleOpts;
        std::vector<sf::Sprite>                                   serverFields;

        playerInfo                                                myInfo;
        std::unique_ptr<TextNode>                                 myText;
        std::unique_ptr<BattleOptionsSprite>                      myBattleOptionsSprite;
        size_t                                                    enemyIndex = 0;

        GameNetwork&                                              net;
        FontHolder&                                               fonts;
        TextureHolder&                                            textures;
        sf::Text                                                  rankingText;
        sf::Text                                                  onlineText;
        sf::Text                                                  challengeText;
        sf::Text                                                  accept, refuse, plpop1, plpop2; 
        int                                                       currentField, choosenField;
        std::vector<sf::Text>                                     rankingNames; 

        
};

#endif // HOMESTATE_HPP
