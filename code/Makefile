usage:
	echo Usage: make linux, make windows or make windows64

debug:
	date +%R
	g++ ./screenStates/BoardSetupState.cpp ./screenStates/GameOverState.cpp ./screenStates/GameState.cpp ./screenStates/HomeState.cpp ./screenStates/LoginState.cpp ./screenStates/RegisterState.cpp ./GameEngine/AlphaText.cpp ./GameEngine/Command.cpp ./GameEngine/CommandQueue.cpp ./GameEngine/GameNetwork.cpp ./GameEngine/MultiSprite.cpp ./GameEngine/MusicPlayer.cpp ./GameEngine/SceneNode.cpp ./GameEngine/SpriteNode.cpp ./GameEngine/SpriteTimer.cpp ./GameEngine/State.cpp ./GameEngine/StateStack.cpp ./GameEngine/TextNode.cpp ./GameEngine/Utility.cpp ./GUI/BattleOptionsSprite.cpp ./GUI/SpriteInput.cpp ./GUI/ScreenButton.cpp ./Application.cpp ./Board.cpp ./Main.cpp ./Player.cpp ./Save.cpp ./World.cpp -o ./../AdmiralMind.exe -m32 -std=c++11 -Ofast -IC:/dev/cpp/includes -LC:/dev/cpp/libs32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
	date +%R
	beep

windows32:
	date +%R
	g++ -mwindows ./screenStates/BoardSetupState.cpp ./screenStates/GameOverState.cpp ./screenStates/GameState.cpp ./screenStates/HomeState.cpp ./screenStates/LoginState.cpp ./screenStates/RegisterState.cpp ./GameEngine/AlphaText.cpp ./GameEngine/Command.cpp ./GameEngine/CommandQueue.cpp ./GameEngine/GameNetwork.cpp ./GameEngine/MultiSprite.cpp ./GameEngine/MusicPlayer.cpp ./GameEngine/SceneNode.cpp ./GameEngine/SpriteNode.cpp ./GameEngine/SpriteTimer.cpp ./GameEngine/State.cpp ./GameEngine/StateStack.cpp ./GameEngine/TextNode.cpp ./GameEngine/Utility.cpp ./GUI/BattleOptionsSprite.cpp ./GUI/SpriteInput.cpp ./GUI/ScreenButton.cpp ./Application.cpp ./Board.cpp ./Main.cpp ./Player.cpp ./Save.cpp ./World.cpp -o ./../AdmiralMind.exe -m32 -std=c++11 -Ofast -IC:/dev/cpp/includes -LC:/dev/cpp/libs32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
	date +%R
	beep

windows64:
	date +%R
	g++ -mwindows ./screenStates/BoardSetupState.cpp ./screenStates/GameOverState.cpp ./screenStates/GameState.cpp ./screenStates/HomeState.cpp ./screenStates/LoginState.cpp ./screenStates/RegisterState.cpp ./GameEngine/AlphaText.cpp ./GameEngine/Command.cpp ./GameEngine/CommandQueue.cpp ./GameEngine/GameNetwork.cpp ./GameEngine/MultiSprite.cpp ./GameEngine/MusicPlayer.cpp ./GameEngine/SceneNode.cpp ./GameEngine/SpriteNode.cpp ./GameEngine/SpriteTimer.cpp ./GameEngine/State.cpp ./GameEngine/StateStack.cpp ./GameEngine/TextNode.cpp ./GameEngine/Utility.cpp ./GUI/BattleOptionsSprite.cpp ./GUI/SpriteInput.cpp ./GUI/ScreenButton.cpp ./Application.cpp ./Board.cpp ./Main.cpp ./Player.cpp ./Save.cpp ./World.cpp -o ./../AdmiralMind.exe -std=c++11 -Ofast -IC:/dev/cpp/includes -LC:/dev/cpp/libs64 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
	date +%R
	beep

linux:
	date +%R
	g++ -DSFML_STATIC ./screenStates/BoardSetupState.cpp ./screenStates/GameOverState.cpp ./screenStates/GameState.cpp ./screenStates/HomeState.cpp ./screenStates/LoginState.cpp ./screenStates/RegisterState.cpp ./GameEngine/AlphaText.cpp ./GameEngine/Command.cpp ./GameEngine/CommandQueue.cpp ./GameEngine/GameNetwork.cpp ./GameEngine/MultiSprite.cpp ./GameEngine/MusicPlayer.cpp ./GameEngine/SceneNode.cpp ./GameEngine/SpriteNode.cpp ./GameEngine/SpriteTimer.cpp ./GameEngine/State.cpp ./GameEngine/StateStack.cpp ./GameEngine/TextNode.cpp ./GameEngine/Utility.cpp ./GUI/BattleOptionsSprite.cpp ./GUI/SpriteInput.cpp ./GUI/ScreenButton.cpp ./Application.cpp ./Board.cpp ./Main.cpp ./Player.cpp ./Save.cpp ./World.cpp -o ./../admiralmind -std=c++11 -Os -lsfml-graphics-s -lsfml-network-s -lsfml-audio-s -lsfml-window-s -lsfml-system-s -lpthread -lX11 -lopenal -lGL -lfreetype -lXrandr -lsndfile -lGLEW -ljpeg
	date +%R

#-mwindows para rodar sem command
#	g++ -DSFML_STATIC ./screenStates/BoardSetupState.cpp ./screenStates/GameOverState.cpp ./screenStates/GameState.cpp ./screenStates/HomeState.cpp ./screenStates/LoginState.cpp ./screenStates/RegisterState.cpp ./GameEngine/AlphaText.cpp ./GameEngine/Command.cpp ./GameEngine/CommandQueue.cpp ./GameEngine/GameNetwork.cpp ./GameEngine/MultiSprite.cpp ./GameEngine/MusicPlayer.cpp ./GameEngine/SceneNode.cpp ./GameEngine/SpriteNode.cpp ./GameEngine/SpriteTimer.cpp ./GameEngine/State.cpp ./GameEngine/StateStack.cpp ./GameEngine/TextNode.cpp ./GameEngine/Utility.cpp ./GUI/BattleOptionsSprite.cpp ./GUI/SpriteInput.cpp ./GUI/ScreenButton.cpp ./Application.cpp ./Board.cpp ./Main.cpp ./Player.cpp ./Save.cpp ./World.cpp -o ./../AdmiralMind -std=c++11 -Os -lsfml-graphics-s -lsfml-network-s -lsfml-audio-s -lsfml-window-s -lsfml-system-s -lpthread -lX11 -lopenal -lGL -lfreetype -lXrandr -lsndfile -lGLEW -ljpeg