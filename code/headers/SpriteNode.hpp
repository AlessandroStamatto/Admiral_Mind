#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include "SceneNode.hpp"

#include "SFML/Graphics/Sprite.hpp"

class SpriteNode : public SceneNode
{
	public:
		explicit			SpriteNode(const sf::Texture& texture);
							SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);

		virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void                setTexture(const sf::Texture& texture, bool redefine = true);


		sf::Sprite			mSprite;
};

#endif // SPRITENODE_HPP
