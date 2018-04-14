#ifndef SPRITEINPUT_HPP
#define SPRITEINPUT_HPP

#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "colorDefines.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window/Event.hpp"
#include <vector>
#include <string>

class SpriteInput : public SceneNode
{
	public:
	
	SpriteInput(const FontHolder& fonts, sf::String lab, bool text_is_secret = false);
	void handleEvent(const sf::Event& event);
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    bool contains (int mx, int my);
    void select();
    void unselect();
    void adjustText();
    std::string getText();
    void updateCurrent(sf::Time dt, CommandQueue& commands);

	sf::Text text;
    sf::Text label;
    sf::Text labelContour;
    bool secret;
    bool selected = false;
    bool increasing = false;
    std::string secretText;
	const FontHolder&    mFonts;
    sf::RectangleShape   textSquare; 
    sf::RectangleShape   cursor;
    sf::Color            color;
};

#endif // SPRITEINPUT_HPP
