#include "./../headers/Utility.hpp"
#include "./../headers/SpriteTimer.hpp"


#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

#include <iostream>

using namespace std;

SpriteTimer::SpriteTimer (int maxTime_, const FontHolder& fonts, const TextureHolder& textures)
: maxTime (maxTime_), timeString(toString(maxTime_), fonts.get(Fonts::Bold), 64)
, radarSprite (textures.get(Textures::radar))
{
    centerOrigin (timeString);
    radarSprite.setOrigin(6, 58);
    radarSprite.setPosition (-4, 13);
}

bool SpriteTimer::timeEnded () {
    return clock.getElapsedTime().asSeconds() >= maxTime;
}

void SpriteTimer::reset() {
    clock.restart();
}

void SpriteTimer::updateCurrent(sf::Time dt, CommandQueue& commands) {
    int remainingTime = maxTime - static_cast<int>(clock.getElapsedTime().asSeconds());
    if (remainingTime < 0) remainingTime = 0;

    if (remainingTime < 10)
        timeString.setString("0" + toString(remainingTime));
    else
        timeString.setString(toString(remainingTime));

    int oneThird = static_cast<int> (maxTime/3);
    if (remainingTime < oneThird)
        timeString.setColor(sf::Color::Red);
    else if (remainingTime < 2 * oneThird)
        timeString.setColor(DARK_BLUE);
    else
        timeString.setColor(LIGHT_BLUE);

    if (9 < remainingTime and remainingTime < 20)
        timeString.setPosition (7, 0);
    else
        timeString.setPosition (0, 0);

    radarSprite.rotate (30 * dt.asSeconds());
}

void SpriteTimer::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw (timeString, states);
    target.draw (radarSprite, states);
}