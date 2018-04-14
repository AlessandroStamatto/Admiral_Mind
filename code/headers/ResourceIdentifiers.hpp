#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP

namespace Textures
{
	enum ID
	{
		BoardSetupBackground,
        GameBackground,
		TitleScreen,
        HomeScreen,
        fullboard,
        fullboardSelected,
        boardSetupField,
        boardSetupFieldSelected,
		BoardShip,
		BoardFire,
		BoardSea,
		BoardSelected,
        BoardCovered,
        IslandSquare,
        BoardIsland,
		Carrier,
		CarrierSelected,
        CarrierGray,
		DestroyedCarrier,
		Battleship,
		BattleshipSelected,
        BattleshipGray,
		DestroyedBattleship,
		Submarine,
		SubmarineSelected,
        SubmarineGray,
		DestroyedSubmarine,
		Cruiser,
		CruiserSelected,
        CruiserGray,
		DestroyedCruiser,
		Destroyer,
		DestroyerSelected,
        DestroyerGray,
		DestroyedDestroyer,
        Mine,
        MineSelected,
        MineGray,
        DestroyedMine,
        Missile,
        Ready,
        NotReady,
        Shuffle,
        Login,
        Back,
        OneShotMinus,    
        OneShotPlus,
        ThreeShotMinus,  
        ThreeShotPlus,
        NotAttached,
        Attached,
        Island,
        NoIsland,
        Cloud,
        NoCloud,
        OneMine,
        TwoMine,
        NoMine,
        Night,
        Day,
        keyboard,
        bci,
        bcikeyboard,
        keyboardbci,
        Register,
        fields3,
        fields5,
        serverField,
        serverFieldSelected,
        serverFieldChoosen,
        smallButton,
        bigButton,
        roll,
        rollbar,
        radar,
        player1,
        player2,
        playerX,
        battlePopup
	};
}

namespace Fonts
{
	enum ID	{ Main, Normal, Bold };
}

namespace Music
{
    enum ID { MenuTheme, BattleTheme };
}


#endif // RESOURCEIDENTIFIERS_HPP
