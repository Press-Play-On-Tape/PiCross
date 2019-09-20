#include "SplashScreenState.h"
#include "../utils/Arduboy2Ext.h"
#include "../images/Images.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void SplashScreenState::activate(StateMachine & machine) {

  (void)machine;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void SplashScreenState::update(StateMachine & machine) { 

	auto & arduboy = machine.getContext().arduboy;
	auto justPressed = arduboy.justPressedButtons();

  if (justPressed > 0 && this->counter > 0) {

    this->counter = 124;

  }

  if (justPressed > 0 && this->counter == 0) {

    this->counter = 1;

    TCCR3A = _BV(COM3A0); // set toggle on compare mode (which connects the pin)
    OCR3A = 3905; // 128 Hz

  }

  if (this->counter > 0) {

    this->counter++;

    switch (this->counter) {

      case 2:     
        OCR3A = 18000;    
        break;

      case 5:     
        TCCR3A = 0;       
        break;
        
      case 125:   
        machine.changeState(GameStateType::TitleScreen);    
        break;

      default:
        break;

    }

  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void SplashScreenState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;

  Sprites::drawOverwrite(47, 17, Images::Ppot_Buttons, 0);
  Sprites::drawOverwrite(43, 26, Images::Ppot_ButtonUp, 0);
  Sprites::drawOverwrite(73, 26, Images::Ppot_ButtonUp, 0);

  if (counter == 0) {

    Sprites::drawOverwrite(58, 26, Images::Ppot_ButtonUp, 0);
    Sprites::drawOverwrite(26, 46, Images::Ppot_Caption, 0);

  }
  else {

    Sprites::drawOverwrite(58, 26, Images::Ppot_ButtonDown, 0);
    Sprites::drawOverwrite(44, 46, Images::Ppot_Loading, 0);

    uint8_t i = (counter / 15) % 4;

    for (uint8_t j = 0; j < i; j++) {

        arduboy.drawPixel(79 + (j * 2), 49, WHITE);

    }

  }

}
