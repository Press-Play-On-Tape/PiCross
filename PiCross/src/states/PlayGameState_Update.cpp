#include "PlayGameState.h"
#include "../images/Images.h"


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void PlayGameState::update(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto justPressed = arduboy.justPressedButtons();
	auto pressed = arduboy.pressedButtons();


	// Normal play ..

	if (justPressed & LEFT_BUTTON || ((pressed & LEFT_BUTTON) && arduboy.isFrameCount(Constants::KeyRepeatDelay, this->keyCount)))	{
	
		this->puzzle.decX();
		if (justPressed & LEFT_BUTTON) this->keyCount = arduboy.getFrameCount(Constants::KeyRepeatDelay);
		if (!(justPressed & A_BUTTON) && (pressed & A_BUTTON)) { this->puzzle.setGrid(this->lastUpdate); }
		if (!(justPressed & B_BUTTON) && (pressed & B_BUTTON)) { this->puzzle.setGrid(this->lastUpdate); }
		return;

	}

	if (justPressed & RIGHT_BUTTON || ((pressed & RIGHT_BUTTON) && arduboy.isFrameCount(Constants::KeyRepeatDelay, this->keyCount)))	{

		this->puzzle.incX();
		if (justPressed & RIGHT_BUTTON) this->keyCount = arduboy.getFrameCount(Constants::KeyRepeatDelay);
		if (!(justPressed & A_BUTTON) && (pressed & A_BUTTON)) { this->puzzle.setGrid(this->lastUpdate); }
		if (!(justPressed & B_BUTTON) && (pressed & B_BUTTON)) { this->puzzle.setGrid(this->lastUpdate); }
		return;

	}

	if (justPressed & UP_BUTTON || ((pressed & UP_BUTTON) && arduboy.isFrameCount(Constants::KeyRepeatDelay, this->keyCount)))	{

		this->puzzle.decY();
		if (justPressed & UP_BUTTON) this->keyCount = arduboy.getFrameCount(Constants::KeyRepeatDelay);
		if (!(justPressed & A_BUTTON) && (pressed & A_BUTTON)) { this->puzzle.setGrid(this->lastUpdate); }
		if (!(justPressed & B_BUTTON) && (pressed & B_BUTTON)) { this->puzzle.setGrid(this->lastUpdate); }
		return;

	}

	if (justPressed & DOWN_BUTTON || ((pressed & DOWN_BUTTON) && arduboy.isFrameCount(Constants::KeyRepeatDelay, this->keyCount)))	{

		this->puzzle.incY();
		if (justPressed & DOWN_BUTTON) this->keyCount = arduboy.getFrameCount(Constants::KeyRepeatDelay);
		if (!(justPressed & A_BUTTON) && (pressed & A_BUTTON)) { this->puzzle.setGrid(this->lastUpdate); }
		if (!(justPressed & B_BUTTON) && (pressed & B_BUTTON)) { this->puzzle.setGrid(this->lastUpdate); }
		return;

	}

	if (!this->gameOver && this->bCount < Constants::BButtonDelay) {

		uint8_t xPos = this->marginLeft + (this->puzzle.getX() * Constants::GridWidthX);
		uint8_t yPos = this->marginTop + (this->puzzle.getY() * Constants::GridWidthY);

		if (xPos >= 119) {
			this->xOffset = xPos - 119;
		}
		else {
			this->xOffset = 0;
		}

		if (yPos >= 55) {
			this->yOffset = yPos - 55;
		}
		else {
			this->yOffset = 0;
		}

		if (justPressed & A_BUTTON) {

			GridValue current = this->puzzle.getGrid();

			switch (current) {

				case GridValue::Blank:	
				case GridValue::Marked:	
					this->puzzle.setGrid(GridValue::Selected);
					this->lastUpdate = GridValue::Selected;
					break;

				case GridValue::Selected:	
					this->puzzle.setGrid(GridValue::Blank);
					this->lastUpdate = GridValue::Blank;
					break;
					
				default: break;

			}
			
		}

		if (justPressed & B_BUTTON) {

			GridValue current = this->puzzle.getGrid();

			switch (current) {

				case GridValue::Blank:	
				case GridValue::Selected:	
					this->puzzle.setGrid(GridValue::Marked);
					this->lastUpdate = GridValue::Marked;
					break;

				case GridValue::Marked:	
					this->puzzle.setGrid(GridValue::Blank);
					this->lastUpdate = GridValue::Blank;
					break;
										
				default: break;
					
			}
			
		}

		if (pressed & A_BUTTON && pressed & B_BUTTON) {
			if (this->bCount < Constants::BButtonDelay) {
				this->bCount++;
			}
		}

		if (arduboy.justReleased(A_BUTTON) && arduboy.justReleased(B_BUTTON)) {
			this->bCount = 0;
		}

	}
	else {

		if (this->gameOver) {

			if (this->counter < 64) {

				this->counter++;
				
			}
			else {

				if (justPressed & A_BUTTON) {

					machine.changeState(GameStateType::SelectPuzzle);

				}

			}

		}

		if (this->bCount >= Constants::BButtonDelay) {

			if (justPressed & A_BUTTON) {

				machine.changeState(GameStateType::SelectPuzzle);

			}

			if (justPressed & B_BUTTON) {
			
				this->bCount = 0;
			
			}

		}

	}

}