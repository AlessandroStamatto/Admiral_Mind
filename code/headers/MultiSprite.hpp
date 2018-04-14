#ifndef MULTISPRITE_HPP
#define MULTISPRITE_HPP

#include "SceneNode.hpp"
#include <vector>

#include "SFML/Graphics.hpp"

class MultiSprite : public SceneNode
{
	public:
		          			MultiSprite(std::vector<sf::Texture> textures);

		virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

        void alternate ();

        void select (int i);

        void selectNext ();

        bool contains (int mx, int my, sf::Transform trans);

		sf::Sprite			            mSprite;
        int                             index = 0;
        std::vector<sf::Texture>        mTextures;
};

#endif // MULTISPRITE_HPP
