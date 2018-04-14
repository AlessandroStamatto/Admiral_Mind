#include "../headers/SpriteNode.hpp"
#include "../headers/Utility.hpp"

#include "SFML/Graphics/RenderTarget.hpp"


SpriteNode::SpriteNode(const sf::Texture& texture)
: mSprite(texture)
{
    centerOrigin (mSprite);
}	

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect)
: mSprite(texture, textureRect)
{
}

void SpriteNode::setTexture(const sf::Texture& texture, bool redefine) {
    mSprite.setTexture (texture);
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}