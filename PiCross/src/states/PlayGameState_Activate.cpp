#include "PlayGameState.h"
#include "../images/Images.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void PlayGameState::activate(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;

  	this->maxSeriesRow = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleMaxRows));
  	this->maxSeriesCol = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleMaxCols));

	this->marginTop = 2 + this->maxSeriesCol * 7;

	this->counter = 0;
	this->bCount = 0;
	this->gameOver = false;

	this->puzzle.setX(0);
	this->puzzle.setY(0);
	this->puzzle.updateRowCols();

	uint8_t maxWidth = 0;

	for (uint8_t y = 0; y < this->puzzle.getSize(); y++) {	
	
		uint8_t width = 3;

		for (uint8_t x = 0; x < maxSeriesRow; x++) {

			switch (this->puzzle.getRow(y, x)) {

				case 1:
					width = width + 4;
					break;

				case 2 ... 9:
					width = width + 5;
					break;

				case 10 ... 99:
					width = width + 7;
					break;

			}

		}

		if (width > maxWidth) {
			maxWidth = width;
		}
	
	}

	this->marginLeft = maxWidth;

}

