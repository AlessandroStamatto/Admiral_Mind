#include "headers/Application.hpp"
#include "headers/Utility.hpp"
#include "headers/State.hpp"
#include "headers/StateIdentifiers.hpp"
#include "headers/LoginState.hpp"
#include "headers/RegisterState.hpp"
#include "headers/HomeState.hpp"
#include "headers/BoardSetupState.hpp"
#include "headers/GameState.hpp"
#include "headers/GameOverState.hpp"


const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

const int wWidth =  933;
const int wHeight = 700;

Application::Application()
: mWindow(sf::VideoMode(wWidth, wHeight), "AdmiralMind Battleship", sf::Style::Close)
, mTextures()
, mFonts()
, mPlayer()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(true);

    static bool notLoaded = true;
    if (notLoaded) {
    	mFonts.load(Fonts::Main, 	  "Media/Sansation.ttf");
        mFonts.load(Fonts::Normal,    "Media/Fonts/AGENCYR.TTF");
        mFonts.load(Fonts::Bold,      "Media/Fonts/AGENCYB.TTF");

    	mTextures.load(Textures::TitleScreen,		"Media/Textures/screens/background.login.png");
    	mTextures.load(Textures::HomeScreen,		"Media/Textures/screens/background.home.png");

        mTextures.load(Textures::fields3,                 "Media/Textures/fields/3_fields.png");
        mTextures.load(Textures::fields5,                 "Media/Textures/fields/5_fields.png");
        mTextures.load(Textures::serverField,             "Media/Textures/fields/serverField.png");
        mTextures.load(Textures::serverFieldSelected,     "Media/Textures/fields/serverFieldSelected.png");
        mTextures.load(Textures::serverFieldChoosen,      "Media/Textures/fields/serverFieldChoosen.png");
        mTextures.load(Textures::boardSetupField,         "Media/Textures/fields/boardSetupField.png");
        mTextures.load(Textures::boardSetupFieldSelected, "Media/Textures/fields/boardSetupFieldSelected.png");

        mTextures.load(Textures::rollbar,         "Media/Textures/rollbar/rollbar.png");
        mTextures.load(Textures::roll,            "Media/Textures/rollbar/roll.png");

        mTextures.load(Textures::smallButton,             "Media/Textures/button/small.png");
        mTextures.load(Textures::bigButton,               "Media/Textures/button/big.png");

    	mTextures.load(Textures::Login,		        "Media/Textures/button/login.png");
        mTextures.load(Textures::Register,          "Media/Textures/button/register.png");

    	mTextures.load(Textures::OneShotMinus,           "Media/Textures/battleopts/oneShotMinus.png"  );
        mTextures.load(Textures::OneShotPlus,            "Media/Textures/battleopts/oneShotPlus.png"   );
        mTextures.load(Textures::ThreeShotMinus,         "Media/Textures/battleopts/threeShotMinus.png");
        mTextures.load(Textures::ThreeShotPlus,          "Media/Textures/battleopts/threeShotPlus.png" );

        mTextures.load(Textures::NotAttached,          "Media/Textures/battleopts/notAttached.png" );
        mTextures.load(Textures::Attached,             "Media/Textures/battleopts/attached.png"    );

        mTextures.load(Textures::Island,               "Media/Textures/battleopts/island.png");
        mTextures.load(Textures::NoIsland,             "Media/Textures/battleopts/noIsland.png");

        mTextures.load(Textures::Cloud,               "Media/Textures/battleopts/cloud.png");
        mTextures.load(Textures::NoCloud,             "Media/Textures/battleopts/noCloud.png");

        mTextures.load(Textures::Day,               "Media/Textures/battleopts/day.png");
        mTextures.load(Textures::Night,             "Media/Textures/battleopts/night.png");

        mTextures.load(Textures::OneMine,            "Media/Textures/battleopts/oneMine.png");
        mTextures.load(Textures::TwoMine,            "Media/Textures/battleopts/twoMine.png");
        mTextures.load(Textures::NoMine,             "Media/Textures/battleopts/noMine.png");

        mTextures.load(Textures::keyboard,           "Media/Textures/battleopts/i_keyboard.png");
        mTextures.load(Textures::keyboardbci,        "Media/Textures/battleopts/i_keyboardbci.png");
        mTextures.load(Textures::bcikeyboard,        "Media/Textures/battleopts/i_bcikeyboard.png");
        mTextures.load(Textures::bci,                "Media/Textures/battleopts/i_bci.png");

        mTextures.load(Textures::radar,         "Media/Textures/misc/radar.png");
        mTextures.load(Textures::player1,       "Media/Textures/misc/player1.png");
        mTextures.load(Textures::player2,       "Media/Textures/misc/player2.png");
        mTextures.load(Textures::playerX,       "Media/Textures/misc/playerX.png");
        mTextures.load(Textures::battlePopup,   "Media/Textures/misc/battlePopup.png");
    }



	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::Login);
}

void Application::registerStates()
{
    mStateStack.registerState<LoginState>(States::Login);
    mStateStack.registerState<RegisterState>(States::Register);
    mStateStack.registerState<HomeState>(States::Home);
    mStateStack.registerState<BoardSetupState>(States::BoardSetup);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<GameOverState>(States::GameOver);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		//updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed) {
			mStateStack.mContext.net->sendDisconnect();
			mWindow.close();
		}
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}
