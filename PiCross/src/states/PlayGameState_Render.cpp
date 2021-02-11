#include "PlayGameState.h"
#include "../images/Images.h"


// ---------------------------------------------------------------------------------------------------------------------------
//  Render the state .. 
// ---------------------------------------------------------------------------------------------------------------------------
// 
void PlayGameState::render(StateMachine & machine) {

  uint8_t const solidLines[] =
   {
     0, 0, 0, 0, 0,    // 0
     0, 0, 0, 0, 0,    // 1
     0, 2, 0, 0, 0,    // 2
     0, 3, 0, 0, 0,    // 3
     0, 4, 0, 0, 0,    // 4
     0, 5, 0, 0, 0,    // 5
     0, 3, 6, 0, 0,    // 6
     0, 7, 0, 0, 0,    // 7
     0, 4, 8, 0, 0,    // 8
     0, 3, 6, 9, 0,    // 9
     0, 5, 10, 0, 0,   // 10
     0, 11, 0, 0, 0,   // 11
     0, 4, 8, 12, 0,   // 12
     0, 13, 0, 0, 0,   // 13
     0, 7, 14, 0, 0,   // 14
     0, 5, 10, 15, 0,  // 15
     0, 4, 8, 12, 16,  // 16
   };

	auto & arduboy = machine.getContext().arduboy;

  bool flash = arduboy.getFrameCountHalf(48);  
  uint8_t size = this->puzzle.getSize();
  uint8_t completedRows = 0;

  for (uint8_t x = 0; x <= size; x++) {


    // Horizontal and Vertical lines ..

    for (uint8_t z = size * 5; z < (size * 5) + 5; z++) {

      if (x == solidLines[z]) {
        arduboy.drawFastVLine(this->marginLeft + (x * Constants::GridWidthX) - this->xOffset, this->marginTop - this->yOffset, size * Constants::GridWidthY, WHITE); 
        arduboy.drawFastHLine(this->marginLeft - this->xOffset, this->marginTop + (x * Constants::GridWidthY) - this->yOffset, size * Constants::GridWidthX, WHITE);  
      }

    }

    arduboy.drawVerticalDottedLine(0, this->marginTop + (size * Constants::GridWidthY) - this->yOffset, this->marginLeft + (x * Constants::GridWidthX) - this->xOffset);  
    arduboy.drawHorizontalDottedLine(0, this->marginLeft + (size * Constants::GridWidthX), this->marginTop + (x * Constants::GridWidthY) - this->yOffset);  

  }

  for (uint8_t x = 0; x < size; x++) {

    for (uint8_t y = 0; y < size; y++) {

      GridValue gridValue = this->puzzle.getGrid(x, y);

      switch (gridValue) {

        case GridValue::Blank:
          break;

        case GridValue::Selected:
          arduboy.fillRect(this->marginLeft + (x * Constants::GridWidthX) + 2 - this->xOffset, this->marginTop + (y * Constants::GridWidthY) + 2 - this->yOffset, 5, 5, WHITE);
          break;

        case GridValue::Marked:
          Sprites::drawSelfMasked(this->marginLeft + (x * Constants::GridWidthX) + 2 - this->xOffset, this->marginTop + (y * Constants::GridWidthY) + 2 - this->yOffset, Images::Marked, 0);
          break;
          
      }
    
    }

  }


  // Render column headers ..

  for (uint8_t x = 0; x < size; x++) {

    if (this->puzzle.isColMatch(x)) {

      arduboy.fillRect(this->marginLeft + (x * Constants::GridWidthX) + 1 - this->xOffset, -this->yOffset, 7, this->marginTop - 1, WHITE);
      font3x5.setTextColor(BLACK);

      completedRows++;

    }
    else {

      font3x5.setTextColor(WHITE);

    }

    for (uint8_t y = 0; y < Constants::NumberOfNumbers; y++) {

      uint8_t val = this->puzzle.getCol(x, y);

      if (val != 0) {
      
        font3x5.setCursor(this->marginLeft + (x * Constants::GridWidthX) + 3 - (val >= 10 ? 3 : (val == 1 ? 1 : 0)) - this->xOffset, -this->yOffset + y * 7);

        font3x5.print(val);
        font3x5.print("\n");
      }
  
    }

  }


  // Render row headers ..

  for (uint8_t y = 0; y < size; y++) {
      // Serial.print(y);
      // Serial.print(": ");

    if (this->puzzle.isRowMatch(y)) {

      arduboy.fillRect(-this->xOffset, this->marginTop + (y * Constants::GridWidthY) + 1 - this->yOffset, this->marginLeft - 1, 7, WHITE);
      font3x5.setTextColor(BLACK);

      completedRows++;

    }
    else {

      font3x5.setTextColor(WHITE);

    }
    
    uint8_t xOffset = this->xOffset;

    for (uint8_t x = 0; x < Constants::NumberOfNumbers; x++) {

      uint8_t val = this->puzzle.getRow(y, x);

      if (val != 0) {

        switch (val) {

          case 1:
            font3x5.setCursor(xOffset - 1, this->marginTop + (y * Constants::GridWidthY) + 1 - this->yOffset);
            xOffset = xOffset + 4;
            font3x5.print(val);
            break;

          case 2 ... 9:
            font3x5.setCursor(xOffset, this->marginTop + (y * Constants::GridWidthY) + 1 - this->yOffset);
            font3x5.print(val);
            xOffset = xOffset + 5;
            break;

          case 10 ... 20:
            font3x5.setCursor(xOffset - 1, this->marginTop + (y * Constants::GridWidthY) + 1 - this->yOffset);
            font3x5.print("1");
            xOffset = xOffset + 3;

            font3x5.setCursor(xOffset, this->marginTop + (y * Constants::GridWidthY) + 1 - this->yOffset);
            if (val % 10 == 1) {
              xOffset = xOffset + 3;
            }
            else {
              xOffset = xOffset + 5;
            }

            font3x5.print(val % 10);

          default: break;

        }

      }
  
    }

  }


  // Game over?

  if (completedRows == 2 * size) {

    this->gameOver = true;
    eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzlesSolved + this->puzzle.getPuzzleIndex()), 1);

  }

  if (this->gameOver) {

    if (this->counter == 64) {

      const uint8_t *puzzle = pgm_read_ptr(&Puzzles::puzzles[this->puzzle.getPuzzleIndex()]);   
      uint8_t width = pgm_read_byte(&puzzle[0]);
      uint8_t height = pgm_read_byte(&puzzle[1]);

      Sprites::drawExternalMask(14, 20, Images::Congratulations,  Images::Congratulations_Mask, 0, 0);
      Sprites::drawSelfMasked(101 - (width / 2), 32 - (height / 2), pgm_read_word(&Puzzles::puzzles[this->puzzle.getPuzzleIndex()]), 0);

    }

  }
  else  if (this->bCount == Constants::BButtonDelay) {

    Sprites::drawExternalMask(14, 20, Images::LeaveGame,  Images::Congratulations_Mask, 0, 0);

  }
  else {

    if (flash) {
      Sprites::drawExternalMask(this->marginLeft + (this->puzzle.getX() * Constants::GridWidthX) - this->xOffset, this->marginTop + (this->puzzle.getY() * Constants::GridWidthY) - this->yOffset, Images::Cursor, Images::Cursor_Mask, 0, 0);
      arduboy.drawRect(this->marginLeft + (this->puzzle.getX() * Constants::GridWidthX) - this->xOffset, this->marginTop + (this->puzzle.getY() * Constants::GridWidthY) - this->yOffset, 9, 9);
    }

  }

}

