#ifndef WORLD_HPP
#define WORLD_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "CommandQueue.hpp"
#include "Command.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "GameNetwork.hpp"
#include "SpriteTimer.hpp"
#include "GameDatatypes.hpp"
#include "TextNode.hpp"
#include "AlphaText.hpp"
#include "BattleOptionsSprite.hpp"
#include "colorDefines.hpp"

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <array>
#include <vector>
#include <queue>
#include <deque>
#include <string>


// Forward declaration
namespace sf
{
    class RenderWindow;
}

class World : private sf::NonCopyable
{
    public:
        explicit                            World(sf::RenderWindow& window, FontHolder& fonts, Player& _player, GameNetwork& _net,\
         mapOptions& _battleOpts, std::deque<std::string>& chatBuff, TextureHolder& textures);
        
        void                                update(sf::Time dt);
        void                                draw();
        bool                                playerDefeated();
        bool                                playerVictory();
        
        CommandQueue&                       getCommandQueue();


    private:
        void                                loadTextures();     
        void                                buildScene();

        sf::FloatRect                       getViewBounds() const;
        sf::FloatRect                       getBattlefieldBounds() const;


    private:

        enum Layer
        {
            Background,
            BoardLayer,
            TopLayer,
            LayerCount
        };

    public:
        int          playerTurn;
        Board*       mPlayerBoard; 
        Board*       mEnemyBoard;
        AlphaText*   mAlphaText;    
        SpriteTimer* bTimer;
        TextNode*    totalTimeText;
        GameNetwork& net;
        sf::Packet   packet;
        std::vector<Ship>   enemyShips;
        std::string enemyName;
        bool firstTime = true;
        bool newMsg;

    private:
        Player&                             mPlayer;
        sf::RenderWindow&                   mWindow;
        sf::View                            mWorldView;
        TextureHolder&                      mTextures;
        FontHolder&                         mFonts;
        mapOptions&                         battleOpts;
        BattleOptionsSprite*                battleOptsSprite;
        std::deque<std::string>&            chatBuffer;
        std::array<SceneNode*, LayerCount>  mSceneLayers;

        SceneNode                           mSceneGraph;
        CommandQueue                        mCommandQueue;

        sf::FloatRect                       mWorldBounds;
        sf::Clock                           totalTime;
        bool                                timeTextReady = false;

        sf::Text                            player1Name, player2Name;
        sf::Sprite                          player1Photo, player2Photo;
};

#endif // WORLD_HPP
