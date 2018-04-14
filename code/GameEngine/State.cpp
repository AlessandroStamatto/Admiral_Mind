#include "../headers/State.hpp"
#include "../headers/StateStack.hpp"


State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player)
: window(&window)
, textures(&textures)
, fonts(&fonts)
, player(&player)
{
    net = new GameNetwork();
    music = new MusicPlayer();
    battleOpts = new mapOptions();
    gameOpts = new mapOptions();
    justRegistered = new bool ();
    *battleOpts = { {"fireRate", 3}, {"allowAttached", 0}, {"withBonus" , 1} };
    *gameOpts = { {"sound", 0} };
    *justRegistered = false;
}

State::State(StateStack& stack, Context context)
: mStack(&stack)
, mContext(context)
{
}

State::State(StateStack& stack, Context&& context)
: mStack(&stack)
, mContext(std::move(context))
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}
