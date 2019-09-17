#include "SelectPuzzleState.h"

#include "../utils/Constants.h"
#include "../utils/Arduboy2Ext.h"
#include "../images/Images.h"
#include "../entities/Puzzle.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void SelectPuzzleState::activate(StateMachine & machine) {

  (void)machine;

  uint16_t numberOfImages = ArrayLength(Puzzles::puzzles);
  this->puzzleIndex = Puzzle::getPuzzleIndex();


  // If the current puzzle is equal to 0 and there are no dimensions, it is probably time to flash the memory with a puzzle ..

  if (eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleHeight)) == 0) {

    this->populatePuzzle(0);

  }


  // If the current puzzle has been completed, then look for the next unfinished puzzle ..

  if (eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzlesSolved + this->puzzleIndex)) == 1) {

    for (uint16_t x = this->puzzleIndex + 1; x < numberOfImages; x++) {

      if (eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzlesSolved + x)) == 0) {

        this->puzzleIndex = x;
        break;

      }

    }

  }

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void SelectPuzzleState::update(StateMachine & machine) { 

	auto & arduboy = machine.getContext().arduboy;
	auto justPressed = arduboy.justPressedButtons();

  uint8_t puzzleIndexMod25 = this->puzzleIndex % 25;
  uint8_t puzzleRange = this->puzzleIndex / 25;


  // Navigation .. 

  if ((justPressed & LEFT_BUTTON) && puzzleIndexMod25 > 0) {

    this->puzzleIndex--;

  }

  if ((justPressed & RIGHT_BUTTON) && puzzleIndexMod25 < 24) {

    this->puzzleIndex++;

  }

  if ((justPressed & UP_BUTTON) && puzzleRange > 0) {

    this->puzzleIndex = this->puzzleIndex - 25;

  }

  if ((justPressed & DOWN_BUTTON) && puzzleRange < 11) {

    this->puzzleIndex = this->puzzleIndex + 25;

  }



  // Populate puzzle ..

  if (justPressed & A_BUTTON) {

    if (eeprom_read_word(reinterpret_cast<uint16_t *>(Constants::PuzzleIndex)) != this->puzzleIndex) {
      this->populatePuzzle(this->puzzleIndex);
    }
    else {

      if (eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzlesSolved + this->puzzleIndex)) == 1) {
        eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzlesSolved + this->puzzleIndex), 0);
        this->populatePuzzle(this->puzzleIndex);
      }

    }

 		machine.changeState(GameStateType::PlayGame); 

  }

}


// ----------------------------------------------------------------------------
//  Render out a puxxle .. 
//
void SelectPuzzleState::populatePuzzle(uint16_t puzzleIndex) {

  uint16_t idx = 0;
  uint8_t maxSeriesRow = 0;
  uint8_t maxSeriesCol = 0;

  const uint8_t *puzzle = pgm_read_word_near(&Puzzles::puzzles[puzzleIndex]);

  uint8_t width = pgm_read_byte(&puzzle[idx++]);
  uint8_t height = pgm_read_byte(&puzzle[idx++]);
  uint8_t height8 = (height % 8 == 0 ? height / 8 : (height / 8) + 1);

  Puzzle::setPuzzleIndex(puzzleIndex);
  eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth), width);
  eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleHeight), height);

  for (uint16_t y = 0; y < height8; y++){

    for (uint16_t x = 0; x < width; x++){

      uint8_t data = pgm_read_byte(&puzzle[idx++]);

      for (uint8_t z = 0; z < 8; z++) {

        uint8_t val = (data & (1 << z)) > 0 ? static_cast<uint8_t>(GridValue::SelectedInImage) : 0;
        uint16_t memLoc = Constants::PuzzleStart + (y * 8 * width) + (z * width) + x;

        eeprom_update_byte(reinterpret_cast<uint8_t *>(memLoc), val);

      }

    }

  }


  // Rows ..

  for (uint8_t y = 0; y < height; y++){

    uint8_t series[Constants::NumberOfNumbers];
    memset(series, 0, Constants::NumberOfNumbers);
    int8_t seriesIdx = -1;

    uint8_t lastData = 0;

    for (uint8_t x = 0; x < width; x++){

      uint8_t data = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStart + (y * width) + x));

      if (lastData != data) {

        if (data == static_cast<uint8_t>(GridValue::SelectedInImage)) {
          seriesIdx++;
          if (seriesIdx == Constants::NumberOfNumbers) break;
        }
        lastData = data;

      }

      if (data == static_cast<uint8_t>(GridValue::SelectedInImage)) {
        series[seriesIdx]++;
      }

    }


    for (uint8_t z = 0; z < Constants::NumberOfNumbers; z++) {

      if (series[z] > 0 && maxSeriesRow < z + 1) maxSeriesRow = z + 1;
      eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleRows + (y * Constants::NumberOfNumbers) + z), series[z]);

    }

  }


  // Column Headers ..
  
  for (uint8_t x = 0; x < width; x++){

    uint8_t series[Constants::NumberOfNumbers];
    memset(series, 0, Constants::NumberOfNumbers);
    int8_t seriesIdx = -1;

    uint8_t lastData = 0;

    for (uint8_t y = 0; y < height; y++){

      uint8_t data = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStart + (y * width) + x));

      if (lastData != data) {

        if (data == static_cast<uint8_t>(GridValue::SelectedInImage)) {
          seriesIdx++;
          if (seriesIdx == Constants::NumberOfNumbers) break;
        }
        lastData = data;

      }

      if (data == static_cast<uint8_t>(GridValue::SelectedInImage)) {
        series[seriesIdx]++;
      }

    }

    for (uint8_t z = 0; z < Constants::NumberOfNumbers; z++){

      if (series[z] > 0 && maxSeriesCol < z + 1) maxSeriesCol = z + 1;
      eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleCols + (x * Constants::NumberOfNumbers) + z), series[z]);

    }

  }

  eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleMaxRows), maxSeriesRow);
  eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleMaxCols), maxSeriesCol);
  
}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void SelectPuzzleState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
  
  uint16_t puzzleRange = this->puzzleIndex / 25;
  uint8_t puzzleIndexMod25 = this->puzzleIndex % 25;
  uint8_t completed = 0;

  int8_t lowerLimit = (puzzleIndexMod25 - 2 < 0 ? 0 : (puzzleIndexMod25 >= 22 ? 20 : puzzleIndexMod25 - 2));
  int8_t upperLimit = (lowerLimit + 5 <= 25 ? lowerLimit + 5 : 25);
  int8_t cursorPosition = (puzzleIndexMod25 < 2 ? lowerLimit + puzzleIndexMod25 : (puzzleIndexMod25 > 22 ? lowerLimit + puzzleIndexMod25 - 20 : lowerLimit + 2));


  // How many of the current range have been completed?

  for (int8_t x = 0; x < 25; x++) {

    if (eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzlesSolved + (puzzleRange * 25) + x)) == 1) {  

      completed++;

    }

  }

  completed = completed * 4;

  Sprites::drawOverwrite(0, 0, Images::Selector_Top, 0);
  Sprites::drawOverwrite(0, 48, Images::Selector_Bottom, 0);

  bool flash = arduboy.getFrameCountHalf(48);  

  for (int8_t x = lowerLimit; x < upperLimit; x++) {

    uint8_t xPos = x - lowerLimit;

    const uint8_t *puzzle = pgm_read_word_near(&Puzzles::puzzles[(puzzleRange * 25) + x]);

    uint8_t width = pgm_read_byte(&puzzle[0]);
    uint8_t height = pgm_read_byte(&puzzle[1]);

    if ((flash && (x == cursorPosition)) || (x != cursorPosition)) {
      Sprites::drawSelfMasked(4 + (xPos * Constants::Select_Spacing), Constants::Select_Top, Images::Box, 0);
    }

    if (eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzlesSolved + (puzzleRange * 25) + x)) == 1) {  
      Sprites::drawSelfMasked(4 + (xPos * Constants::Select_Spacing) + 10 - (width / 2), Constants::Select_Top + 10 - (height / 2), pgm_read_word(&Puzzles::puzzles[(puzzleRange * 25) + x]), 0);
    }
    else {
      Sprites::drawSelfMasked(10 + (xPos * Constants::Select_Spacing), Constants::Select_Top, Images::QuestionMark, 0);
    }

    font3x5.setCursor(4 + (xPos * Constants::Select_Spacing) + 7, Constants::Select_Label);
    if (x + 1 < 10) font3x5.print("0");
    font3x5.print(x + 1);

  }

  font3x5.setCursor(42, 0);
  font3x5.print(puzzleRange + 5);
  font3x5.print("x");
  font3x5.print(puzzleRange + 5);

  font3x5.setCursor(93, 0);
  if (completed < 100) font3x5.print("0");
  if (completed < 10)  font3x5.print("0");
  font3x5.print(completed);

  if (puzzleRange > 0) Sprites::drawSelfMasked(65, 2, Images::ArrowUp, 0);
  if (puzzleRange < 11) Sprites::drawSelfMasked(73, 2, Images::ArrowDown, 0);

}
