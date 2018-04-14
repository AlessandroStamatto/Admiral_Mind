#include "../headers/HomeState.hpp"
#include "../headers/Utility.hpp"
#include "../headers/ResourceHolder.hpp"
#include "../headers/player.hpp"
#include "../headers/GameDatatypes.hpp"

#include "../headers/event_macros.hpp"
    
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

HomeState::HomeState(StateStack& stack, Context context)
: State(stack, context), net (*context.net), fonts(*context.fonts), textures(*context.textures),
rankingText("Ranking"      , context.fonts->get(Fonts::Normal), 22),
onlineText ("Online"       , context.fonts->get(Fonts::Normal), 22),
challengeText  ("Challenge !"  , context.fonts->get(Fonts::Normal), 52),
accept     ("Accept"       , context.fonts->get(Fonts::Bold)  , 18),
refuse     ("Refuse"       , context.fonts->get(Fonts::Bold)  , 18),
plpop1     ("Player 1"     , context.fonts->get(Fonts::Normal), 18),
plpop2     ("Player 2"     , context.fonts->get(Fonts::Normal), 18),
serverFields (10, Sprite{context.textures->get(Textures::serverField)}),
rollbarSprite (context.textures->get(Textures::rollbar)),
rollSprite    (context.textures->get(Textures::roll)),
player_1      (context.textures->get(Textures::player1)),
player_2      (context.textures->get(Textures::playerX)),
popup         (context.textures->get(Textures::battlePopup)), 
currentField(-1), choosenField(-1)
{
    if (context.gameOpts->at("sound"))
        context.music->play(Music::MenuTheme);

    auto& bo = *context.battleOpts;
    bo["fireRate"] = 1; 
    bo["allowAttached"] = 0; 
    bo["withBonus"] = 1;
    bo["island"] = 0;
    bo["cloud"] = 0;
    bo["mines"] = 0;
    bo["night"] = 0;
    bo["bci"] = 0;
    bo["opponentbci"] = 0;
    bo["keyboard"] = 1;
    bo["opponentkeyboard"] = 1;

    net.sendOptions(bo);

	mBackgroundSprite.setTexture(context.textures->get(Textures::HomeScreen));
    centerOrigin (mBackgroundSprite);
    centerOrigin (popup);
    popup.setPosition(466, 350);

    onlineText.setColor (LIGHT_BLUE);
    rankingText.setColor (LIGHT_BLUE);
    challengeText.setColor (DARK_BLUE);

    accept.setColor(sf::Color::White);
    refuse.setColor(LIGHT_BLUE);
    plpop1.setColor(LIGHT_BLUE);
    plpop2.setColor(LIGHT_BLUE);

    centerOrigin(accept);
    centerOrigin(refuse);
    centerOrigin(plpop1);
    centerOrigin(plpop2);

    accept.setPosition (248 + 268, 332 + 173);
    refuse.setPosition (164 + 268, 332 + 173);
    plpop1.setPosition (73  + 268, 290 + 173);
    plpop2.setPosition (310 + 268, 290 + 173);

    centerOrigin(challengeText);

    onlineText.setPosition(135, 248);
    rankingText.setPosition(748, 248);
    challengeText.setPosition (465, 185);

    int x = 114, y = 278;
    for (auto& sfield : serverFields) {
        sfield.setPosition(x, y);
        y += 36;
    }

    rollbarSprite.setPosition(560, 275);
    rollSprite.setPosition(564, 296);

    loadRanking();
    player_1.setPosition (9  , 8);
    player_2.setPosition (848, 8);
}

void HomeState::loadRanking() {
    vector<string> tempNames = {"Super Sarah", "Alex Matto", "Ruroni Kenshin", "Mister S", "Ana Designer"};
    int ny = 290;
    for (const auto& name : tempNames) {
        rankingNames.emplace_back (name, getContext().fonts->get(Fonts::Bold), 18);
        sf::Text& current = rankingNames.back();
        current.setColor (LIGHT_BLUE);
        centerOrigin(current);
        current.setPosition(720, ny);
        ny += 34;
    }
}

void HomeState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
    window.draw(onlineText);
    window.draw(rankingText);
    window.draw(challengeText);

    for (auto& sfield : serverFields)
        window.draw(sfield);

    window.draw(rollbarSprite);
    window.draw(rollSprite);

    for (int i = 0; i < playersInfo.size(); ++i) {
        window.draw(*playerlistTexts[i]);
        window.draw(*playerlistBattleOpts[i]);
    }

    if (myText != nullptr) {
        window.draw(*myText);
        window.draw(*myBattleOptionsSprite);
    }

    for (const auto& rname : rankingNames) {
        window.draw(rname);
    }

    window.draw(player_1);
    window.draw(player_2);

    if (showPopup) {
        window.draw(popup);
        window.draw(accept);
        window.draw(refuse);
        window.draw(plpop1);
        window.draw(plpop2);
    };
}

bool HomeState::update(sf::Time)
{
    packetID id;
    while (static_cast<bool> (id = net.receive()) ) {
        switch (id) {
            case packetID::Playerlist : {
                cout << "Playerlist received: ";
                playersInfo = net.receivePlayerList();
                playerlistTexts.clear();
                playerlistBattleOpts.clear();      

                int myIndex = 0;
                for (; playersInfo[myIndex].id != net.myID; ++ myIndex);
                myInfo = playersInfo[myIndex];
                myText.reset (new TextNode(fonts, myInfo.name));
                myBattleOptionsSprite.reset (new BattleOptionsSprite(textures, myInfo.battleOpts));
                myBattleOptionsSprite->setPosition(33, 117);
                playersInfo.erase(playersInfo.begin() + myIndex);
                myText->mText.setColor (LIGHT_BLUE);
                myText->mText.setCharacterSize (18);
                myText->mText.setFont (getContext().fonts->get(Fonts::Bold));
                myText->mText.setOrigin (0, 0);
                myText->mText.setPosition(164, 86);

                sort(begin(playersInfo), end(playersInfo), [](playerInfo a, playerInfo  b) {
                    return a.name < b.name;   
                });

                int y = 290;
                for (auto& pinfo : playersInfo) {
                    playerlistTexts.emplace_back(new TextNode(fonts, pinfo.name));
                    playerlistBattleOpts.emplace_back(new BattleOptionsSprite(textures, pinfo.battleOpts));
                    sf::Text& mtext = playerlistTexts.back()->mText;
                    centerOrigin(mtext);
                    mtext.setPosition(185, y);
                    playerlistBattleOpts.back()->setPosition(300, y-7);
                    y += 36;
                    
                    switch (pinfo.status) {
                        case statusID::playing :
                            mtext.setColor (Color::Red); break;
                        case statusID::online :
                            mtext.setColor (Color::Green); break;
                        default : {
                            mtext.setColor ({192,192,192});
                        }
                    }
                    mtext.setCharacterSize(24);
                }
                break;
            }
            case packetID::Connect : {
                net.receiveConnect();
                *getContext().battleOpts = myInfo.battleOpts;
                requestStateClear();
                requestStackPush(States::BoardSetup);
            }
            case packetID::Challenge : {
                net.receiveChallenge();
                cout << "Received challenge from player " << net.enemyID << "\n";

                size_t i = 0;
                for (auto& pinfo : playersInfo) {
                    if (pinfo.id == net.enemyID) enemyIndex = i;
                    ++i;
                }

                plpop1.setString(myInfo.name);
                plpop2.setString(playersInfo[enemyIndex].name);
                showPopup = true;
                break;
            }
        }

    }
    net.update();
    for (auto& serverField : serverFields)
        serverField.setTexture(textures.get(Textures::serverField));
    if (0 <= currentField and currentField < serverFields.size())
        serverFields[currentField].setTexture(textures.get(Textures::serverFieldSelected)); 
    if (0 <= choosenField and choosenField < serverFields.size())
        serverFields[choosenField].setTexture(textures.get(Textures::serverFieldChoosen));
	return true;
}

void HomeState::challengePlayer(int opponentID) {
    
    size_t i = 0;
    for (auto& pinfo : playersInfo) {
        if (pinfo.id == opponentID) enemyIndex = i;
        ++i;
    }

    if (enemyIndex >= 0 and enemyIndex < playersInfo.size() and
    playersInfo[enemyIndex].status == statusID::online) {
        net.enemyID = opponentID;
        net.sendChallenge();
    }
}

void HomeState::startBattle(int opponentID) {
    
    size_t i = 0;
    for (auto& pinfo : playersInfo) {
        if (pinfo.id == net.enemyID) enemyIndex = i;
        ++i;
    }

    if (enemyIndex >= 0 and enemyIndex < playersInfo.size() and
    playersInfo[enemyIndex].status == statusID::online) {
        auto& myBattleOpts = *getContext().battleOpts;
        myBattleOpts = playersInfo[enemyIndex].battleOpts;
        net.sendConnect();
        requestStateClear();
        requestStackPush(States::BoardSetup);
    }
}

bool HomeState::handleEvent(const sf::Event& event)
{
    if ( MOUSE_RELEASED  ) {
        if (MOUSE_BUTTON(Left)) {

            if (showPopup) {
                if (MOUSE_BUTTON_ON(refuse)) {
                    showPopup = false;
                }
                else if (MOUSE_BUTTON_ON(accept)) {
                    startBattle (net.enemyID);
                }

                return true;
            }

            int playerIndex = 0;
            for (auto& playerText : playerlistTexts) {
                if (CONTAINS_MOUSE(playerText)
                    and playersInfo[playerIndex].status == statusID::online) {
                    
                    enemyIndex = playerIndex;
                    net.enemyID = playersInfo[playerIndex].id;
                    choosenField = playerIndex;
                }

                ++playerIndex;
            }
            if (myBattleOptionsSprite->handleEvent(event))
                net.sendOptions(myInfo.battleOpts);
            if (MOUSE_BUTTON_ON(challengeText))
                challengePlayer (net.enemyID);
        }
    }
    else if (MOUSE_MOVED) {
        int sfindex = 0;
        for (auto& serverField : serverFields) {
            if (MOUSE_MOVED_ON(serverField))
                currentField = sfindex;
            ++sfindex;
        }
    }

    if ( KEY_RELEASED )
        if (ON_KEY(Up)) {   
            
        }
        else if (ON_KEY(Down) or ON_KEY(Tab)) {
            
        }
        else if (ON_KEY(Return)) {
           
        }

}
