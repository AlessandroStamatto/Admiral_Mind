#include "../headers/MultiSprite.hpp"
#include "../headers/Utility.hpp"

#include <iostream>

#include "SFML/Graphics/RenderTarget.hpp"

MultiSprite::MultiSprite(std::vector<sf::Texture> textures)
{
    centerOrigin (mSprite);
    mTextures = textures;
    index = 0;
    select(index);
}	

void MultiSprite::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

void MultiSprite::alternate () {
    select ((index + 1) % mTextures.size());
}

void MultiSprite::select (int i) {
    index = i;
    mSprite.setTexture (mTextures[index], true);
}

void MultiSprite::selectNext () {
    mSprite.setTexture (mTextures[++index % mTextures.size()], true);
}

bool MultiSprite::contains (int mx, int my, sf::Transform trans) { 
    sf::IntRect square = static_cast<sf::IntRect> ((getTransform() * trans).transformRect(mSprite.getGlobalBounds()));
    return square.contains(mx, my);
}