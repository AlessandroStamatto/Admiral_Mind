#ifndef BATTLEOPTIONSSPRITE_HPP
#define BATTLEOPTIONSSPRITE_HPP

#include "MultiSprite.hpp"
#include "ResourceHolder.hpp"
#include "GameDatatypes.hpp"
#include <array>
#include <vector>
#include <memory>

#include "SFML/Graphics.hpp"

class BattleOptionsSprite : public SceneNode
{
	public:
		          			BattleOptionsSprite(TextureHolder& textures, mapOptions& battleOpts_);

		virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

                bool        handleEvent(const sf::Event& event);

		std::vector<std::unique_ptr<MultiSprite>>           sprites;
        mapOptions&                                         battleOpts;

        std::map<std::string, std::vector<int>> options = {
            {"fireRate" , {1, 3}}
           ,{"withBonus", {0, 1}}
        };
};

#endif // BATTLEOPTIONSSPRITE_HPP
