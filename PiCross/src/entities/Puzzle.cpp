#include "Puzzle.h"

#include "../utils/Constants.h"
#include "../utils/Utils.h"
#include "../utils/Enums.h"
#include "../images/Images.h"

Puzzle::Puzzle() { }


uint8_t Puzzle::getX() {

  return this->x;

}

uint8_t Puzzle::getY() {

  return this->y;

}

uint8_t Puzzle::getSize() {

  return eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth));

}

uint16_t Puzzle::getPuzzleIndex() {

  uint16_t retValue = eeprom_read_word(reinterpret_cast<uint16_t *>(Constants::PuzzleIndex));
  return retValue;

}

GridValue Puzzle::getGrid() {

  uint8_t width = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth));
  uint8_t val = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStart + (this->y * width) + (this->x))) & 0x0f;

  return static_cast<GridValue>(val);

}

GridValue Puzzle::getGrid(uint8_t x, uint8_t y) {

  uint8_t width = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth));
  uint8_t val = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStart + (y * width) + x)) & 0x0f;

  return static_cast<GridValue>(val);

}

uint8_t Puzzle::getCol(uint8_t col, uint8_t index) {

  return eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleCols + (col * Constants::NumberOfNumbers) + index));

}

uint8_t Puzzle::getRow(uint8_t row, uint8_t index) {

  return eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleRows + (row * Constants::NumberOfNumbers) + index));

}

void Puzzle::setX(uint8_t value) {

  this->x = value;
  
}

void Puzzle::setY(uint8_t value) {
  
  this->y = value;
  
}

void Puzzle::setSize(uint8_t value) {
  
  eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth), value);
  eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleHeight), value);
  
}

void Puzzle::setPuzzleIndex(uint16_t value) {
  
  eeprom_update_word(reinterpret_cast<uint16_t *>(Constants::PuzzleIndex), value);
  
}

void Puzzle::setGrid(GridValue value) {

  uint8_t width = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth));
  uint16_t memLoc = Constants::PuzzleStart + (this->y * width) + x;
  uint8_t update = (eeprom_read_byte(reinterpret_cast<uint8_t *>(memLoc)) & static_cast<uint8_t>(GridValue::SelectedInImage)) | static_cast<uint8_t>(value);

  eeprom_update_byte(reinterpret_cast<uint8_t *>(memLoc), static_cast<uint8_t>(update));

  this->updateRowCols();

}

void Puzzle::setGrid(uint8_t x, uint8_t y, GridValue value) {

  uint8_t width = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth));

  eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStart + (y * width) + x), static_cast<uint8_t>(value));

  this->updateRowCols();

}

void Puzzle::setCol(uint8_t col, uint8_t index, uint8_t value) {
  
  eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleCols + (col * Constants::NumberOfNumbers) + index), value);

}

void Puzzle::setRow(uint8_t row, uint8_t index, uint8_t value) {
  
  eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleRows + (row * Constants::NumberOfNumbers) + index), value);

}

void Puzzle::incX() {
  
  uint8_t width = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth));

  this->x++;
  if (this->x == width) this->x = 0;
  
}

void Puzzle::decX() {
    
  uint8_t width = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth));

  if (this->x == 0) this->x = width;
  this->x--;
  
}

void Puzzle::incY() {
    
  uint8_t height = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleHeight));
  
  this->y++;
  if (this->y == height) this->y = 0;
  
}

void Puzzle::decY() {
    
  uint8_t height = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleHeight));
  
  if (this->y == 0) this->y = height;
  this->y--;
  
}

bool Puzzle::isColMatch(uint8_t col) {

  for (uint8_t z = 0; z < Constants::NumberOfNumbers; z++) {

    if (eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleCols + (col * Constants::NumberOfNumbers) + z)) != this->cols[(col * Constants::NumberOfNumbers) + z]) {
      return false;
    }

  }

  return true;

}

bool Puzzle::isRowMatch(uint8_t row) {

  for (uint8_t z = 0; z < Constants::NumberOfNumbers; z++) {

    if (eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleRows + (row * Constants::NumberOfNumbers) + z)) != this->rows[(row * Constants::NumberOfNumbers) + z]) {
      return false;
    }

  }

  return true;

}
 
void Puzzle::updateRowCols() {

  uint8_t width = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth));
  uint8_t height = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleHeight));

  // Rows ..

  for (uint8_t y = 0; y < height; y++){

    uint8_t series[Constants::NumberOfNumbers];
    memset(series, 0, Constants::NumberOfNumbers);
    int8_t seriesIdx = -1;

    uint8_t lastData = 0;

    for (uint8_t x = 0; x < width; x++){

      uint8_t data = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStart + (y * width) + x)) & 0x0F;

      if (lastData != data) {

        if (data == static_cast<uint8_t>(GridValue::Selected)) {
          seriesIdx++;
          if (seriesIdx == Constants::NumberOfNumbers) break;
        }
        lastData = data;

      }

      if (data == static_cast<uint8_t>(GridValue::Selected)) {
        series[seriesIdx]++;
      }

    }

    for (uint8_t z = 0; z < Constants::NumberOfNumbers; z++){

      this->rows[(y * Constants::NumberOfNumbers) + z] = series[z];

    }

  }


  // Column Headers ..
  
  for (uint8_t x = 0; x < width; x++){

    uint8_t series[Constants::NumberOfNumbers];
    memset(series, 0, Constants::NumberOfNumbers);
    int8_t seriesIdx = -1;

    uint8_t lastData = 0;

    for (uint8_t y = 0; y < height; y++){

      uint8_t data = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStart + (y * width) + x)) & 0x0F;

      if (lastData != data) {

        if (data == static_cast<uint8_t>(GridValue::Selected)) {
          seriesIdx++;
          if (seriesIdx == Constants::NumberOfNumbers) break;
        }
        lastData = data;

      }
      if (data == static_cast<uint8_t>(GridValue::Selected)) {
        series[seriesIdx]++;
      }

    }

    for (uint8_t z = 0; z < Constants::NumberOfNumbers; z++){

      this->cols[(x * Constants::NumberOfNumbers) + z] = series[z];

    }

  }        

}