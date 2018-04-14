#ifndef SPRITETIMER_HPP
#define SPRITETIMER_HPP

#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "colorDefines.hpp"

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System.hpp"
#include <vector>
#include <string>

class SpriteTimer : public SceneNode
{
	public:
	
	SpriteTimer(int maxTime_, const FontHolder& fonts, const TextureHolder& textures);
		
	bool timeEnded ();
	void reset();
	void updateCurrent(sf::Time dt, CommandQueue& commands);
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Clock clock;
	int maxTime;
	sf::Text timeString;
    sf::Sprite radarSprite;
};

#endif // SPRITETIMER_HPP
