#ifndef SCREENBUTTON_HPP
#define SCREENBUTTON_HPP

#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "colorDefines.hpp"

#include "SFML/Graphics.hpp"

#include <string>

class ScreenButton : public sf::Transformable, public sf::Drawable
{
	public:
	
    ScreenButton(std::string str, const sf::Font& font, const sf::Texture& texture, int fontSize = 22);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::FloatRect getGlobalBounds() const;

	sf::Text text;
    sf::Sprite sprite;

};

#endif // SCREENBUTTON_HPP
