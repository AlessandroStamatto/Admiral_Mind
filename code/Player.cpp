#include "headers/Player.hpp"
#include "headers/CommandQueue.hpp"
#include "headers/Board.hpp"

#include <map>
#include <string>
#include <algorithm>

using namespace std::placeholders;

struct CursorMover
{
    CursorMover (int i, int j)
    : row(i), col(j)
    {
    }

    void operator() (Board& board, sf::Time) const
    {
        if ((board.playerID == 1) || (board.playerTurn == 2)) return;

        board.cursorI += row;
        if (board.cursorI < 0)
        	board.cursorI = board.mnRows - 1;
        else
        	board.cursorI %= board.mnRows;	
         
        board.cursorJ += col; 
        if (board.cursorJ < 0)
        	board.cursorJ = board.mnCols - 1;
        else
        	board.cursorJ %= board.mnCols;	
    }

    int row, col;
};

struct MouseCursor
{
    MouseCursor (int mouseX, int mouseY)
    : x (mouseX), y(mouseY)
    {
    }

    void operator() (Board& board, sf::Time) const {
        if ((board.playerID == 1) || (board.playerTurn == 2)) return;

        for (int i = 0; i < board.boardSprites.size(); ++i)
            for (int j = 0; j < board.boardSprites[i].size(); ++j)
                if (static_cast<sf::IntRect> (board.getTransform().transformRect(board.boardSprites[i][j].getGlobalBounds())).contains({x, y})) 
                {
                    board.cursorI = i;
                    board.cursorJ = j;
                    board.fire();
                    board.net.sendFire(board.cursorI, board.cursorJ);                        
                    return;
                }
    }

    int x, y;
};

struct FireIt {
    FireIt () {}

    void operator() (Board& board, sf::Time) const {
        if ((board.playerID == 1) || (board.playerTurn == 2)) return;
        board.fire();
        board.net.sendFire(board.cursorI, board.cursorJ);        
    }
};

Player::Player()
{
    settings.boardNRows = settings.boardNCols = 10;

	// Set initial key bindings
	mKeyBinding[sf::Keyboard::A] = MoveLeft;
	mKeyBinding[sf::Keyboard::D] = MoveRight;
	mKeyBinding[sf::Keyboard::W] = MoveUp;
	mKeyBinding[sf::Keyboard::S] = MoveDown;
	mKeyBinding[sf::Keyboard::F] = Fire;
 
	// Set initial action bindings
	initializeActions();	

	// Assign all categories to player's cursor
	for (auto& pair : mActionBinding)
		pair.second.category = Category::Board;

    victory = false;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
    else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            Command mouseFire;
            mouseFire.category = Category::Board;
            mouseFire.action   = derivedAction<Board>(MouseCursor(event.mouseButton.x, event.mouseButton.y));
            commands.push(mouseFire);
        }
    }
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	for (auto pair : mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
	mActionBinding[MoveLeft].action      = derivedAction<Board>(CursorMover( 0, -1));
	mActionBinding[MoveRight].action     = derivedAction<Board>(CursorMover( 0, +1));
	mActionBinding[MoveUp].action        = derivedAction<Board>(CursorMover(-1,  0));
	mActionBinding[MoveDown].action      = derivedAction<Board>(CursorMover(+1,  0));
	mActionBinding[Fire].action          = derivedAction<Board>(FireIt());
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case MoveLeft:
		case MoveRight:
		case MoveDown:
		case MoveUp:
		case Fire:
			return false;

		default:
			return false;
	}
}
