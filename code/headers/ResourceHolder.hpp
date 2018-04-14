#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <iostream>

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Font.hpp"

#include "ResourceIdentifiers.hpp"

template <typename Resource, typename Identifier>
class ResourceHolder
{
	public:
		void						load(Identifier id, const std::string& filename);

		template <typename Parameter>
		void						load(Identifier id, const std::string& filename, const Parameter& secondParam);

		Resource&					get(Identifier id);
		const Resource&				get(Identifier id) const;


	private:
		void						insertResource(Identifier id, std::unique_ptr<Resource> resource);

		std::map<Identifier, std::unique_ptr<Resource>>	mResourceMap;
		std::string lastFileLoaded;
};

#include "ResourceHolder.inl"

typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>			FontHolder;

#endif // RESOURCEHOLDER_HPP
