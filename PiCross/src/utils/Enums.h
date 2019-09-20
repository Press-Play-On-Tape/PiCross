#pragma once

#include <stdint.h>
#include "Utils.h"

enum class GameStateType : uint8_t {
	SplashScreen,
	TitleScreen,
	SelectPuzzle, 
  PlayGame,
  GameOver
};

enum class GridValue : uint8_t {
	Blank,
	Selected,
  Marked,
	SelectedInImage = 16
};
