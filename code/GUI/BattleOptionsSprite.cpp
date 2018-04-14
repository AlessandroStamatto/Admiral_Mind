#include "../headers/BattleOptionsSprite.hpp"
#include "../headers/Utility.hpp"

#include "SFML/Graphics/RenderTarget.hpp"

#include "../headers/event_macros.hpp"

#include <iostream>

using namespace std;

#define addMulti2(textr1, textr2) sprites.emplace_back (new MultiSprite ({ textures.get(Textures::textr1), textures.get(Textures::textr2) }))

#define addMulti3(textr1, textr2, textr3) sprites.emplace_back (new MultiSprite (\
{ textures.get(Textures::textr1), textures.get(Textures::textr2), textures.get(Textures::textr3)}))

#define addMulti4(textr1, textr2, textr3, textr4) sprites.emplace_back (new MultiSprite (\
{ textures.get(Textures::textr1), textures.get(Textures::textr2), textures.get(Textures::textr3), textures.get(Textures::textr4) }))

BattleOptionsSprite::BattleOptionsSprite(TextureHolder& textures, mapOptions& battleOpts_):
battleOpts(battleOpts_)
{
    addMulti4 ( OneShotMinus, OneShotPlus, ThreeShotMinus, ThreeShotPlus ); //fireRate + bonus
    addMulti2 ( NotAttached, Attached); //allow ships attached or not
    addMulti2 ( NoIsland, Island); //map with islands or not
    addMulti2 ( NoCloud, Cloud); //clouds covering battlefield
    addMulti3 ( NoMine, OneMine, TwoMine); //mines on the battlefield
    addMulti2 ( Day, Night); //show or not feedback
    addMulti4 ( keyboard, keyboardbci, bcikeyboard, bci ); //interact form

    int x = 0;
    for (auto& spr : sprites) {
        spr->setPosition (x, 0);
        x += 32;
    }

    //Handles fireRate and bonus
    if (battleOpts["fireRate"] == 3)
        sprites[0]->select (2);
    if (battleOpts["withBonus"])
        sprites[0]->selectNext();

    //Handles if it is allowed to have ships attached, or not
    if (battleOpts["allowAttached"])
        sprites[1]->selectNext();

    //Handles the exitence of islands
    if (battleOpts["island"])
        sprites[2]->selectNext();

    if (battleOpts["cloud"])
        sprites[3]->selectNext();

    sprites[4]->select (battleOpts["mines"]);

    if (battleOpts["night"])
        sprites[5]->selectNext();

    // keyboard, keyboardbci, bcikeyboard, bci
    if (battleOpts["keyboard"] and battleOpts["opponentbci"])
        sprites[6]->selectNext();
    else if (battleOpts["bci"] and battleOpts["opponentkeyboard"])
        sprites[6]->select(2);
    else if (battleOpts["bci"] and battleOpts["opponentbci"])
        sprites[6]->select(3);
}	

void BattleOptionsSprite::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto& sprite : sprites)
	   target.draw(*sprite, states);
}

bool BattleOptionsSprite::handleEvent(const sf::Event& event) {
    int i = 0;
    for (auto& spr : sprites) {
        if (spr->contains(event.mouseButton.x, event.mouseButton.y, getTransform())){
            spr->alternate();
            break;
        }
        ++i;
    }
    if (i >= sprites.size()) return false;

    int sprI = sprites[i]->index;
    switch (i) {
        case 0 : {
            battleOpts["fireRate"] = sprI < 2 ? 1 : 3;
            battleOpts["withBonus"] = sprI % 2;
            break;
        }
        case 1 : {
            battleOpts["allowAttached"] = sprI;
            break;
        }
        case 2 : {
            battleOpts["island"] = sprI;
            break;
        }
        case 3 : {
            battleOpts["cloud"] = sprI;
            break;
        }
        case 4 : {
            battleOpts["mines"] = sprI;
            break;
        }
        case 5 : {
            battleOpts["night"] = sprI;
            break;
        }
        case 6 : { // keyboard, keyboardbci, bcikeyboard, bci
            switch (sprI) {
                case 0: battleOpts["keyboard"] =1; battleOpts["opponentkeyboard"] =1;
                        battleOpts["bci"] = 0; battleOpts["opponentbci"] = 0; break;  
                case 1: battleOpts["keyboard"] =1; battleOpts["opponentkeyboard"] =0;
                        battleOpts["bci"] = 0; battleOpts["opponentbci"] = 1; break;
                case 2: battleOpts["keyboard"] =0; battleOpts["opponentkeyboard"] =1;
                        battleOpts["bci"] = 1; battleOpts["opponentbci"] = 0; break;
                case 3: battleOpts["keyboard"] =0; battleOpts["opponentkeyboard"] =0;
                        battleOpts["bci"] = 1; battleOpts["opponentbci"] = 1; break;
            }
        }
        default : cout << "Unknown Option";
    }
    return true;
}