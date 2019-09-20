#include "PlayGameState.h"
#include "../images/Images.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void PlayGameState::activate(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;

  this->maxSeriesRow = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleMaxRows));
  this->maxSeriesCol = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleMaxCols));

	this->marginLeft = 3 + this->maxSeriesRow * 5;
	this->marginTop = 2 + this->maxSeriesCol * 7;

	this->counter = 0;
	this->bCount = 0;
	this->gameOver = false;

	this->puzzle.setX(0);
	this->puzzle.setY(0);
	this->puzzle.updateRowCols();

}

