#ifndef CATEGORY_HPP
#define CATEGORY_HPP


// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None				= 0,
		SceneTopLayer		= 1 << 0,
		Board		    = 1 << 1

	};
}

#endif // CATEGORY_HPP
