#include "TitleScreenState.h"
#include "../images/Images.h"

constexpr const static uint8_t UPLOAD_DELAY = 16;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void TitleScreenState::activate(StateMachine & machine) {

	(void)machine;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void TitleScreenState::update(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
  auto justPressed = arduboy.justPressedButtons();
  auto pressed = arduboy.pressedButtons();


	// Restart ?

	if (pressed & DOWN_BUTTON) {

		if (restart < UPLOAD_DELAY) {
			restart++;
		}
		else {
			arduboy.exitToBootloader();
		}

	}
	else {
		restart = 0;
	}


	// Handle other input ..

	if (justPressed & A_BUTTON) {
		machine.changeState(GameStateType::SelectPuzzle); 
	}

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void TitleScreenState::render(StateMachine & machine) {

	(void)machine;

	Sprites::drawOverwrite(3, 4, Images::TitleScreen_Top, 0);
	Sprites::drawOverwrite(3, 3, Images::TitleScreen_Top, 0);

	Sprites::drawOverwrite(16, 25, Images::TitleScreen_Middle, 0);

	Sprites::drawOverwrite(27, 45, Images::TitleScreen_Bottom, 0);
	Sprites::drawOverwrite(27, 44, Images::TitleScreen_Bottom, 0);

}
