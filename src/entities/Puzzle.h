#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../utils/Constants.h"
#include "../utils/Enums.h"

class Puzzle { 

  public:

    Puzzle();

    static uint16_t getPuzzleIndex();
    static void setPuzzleIndex(uint16_t value);

    uint8_t getX();
    uint8_t getY();
    uint8_t getSize();
    GridValue getGrid();
    GridValue getGrid(uint8_t x, uint8_t y);
    uint8_t getCol(uint8_t col, uint8_t index);
    uint8_t getRow(uint8_t col, uint8_t index);

    void setX(uint8_t value);
    void setY(uint8_t value);
    void setSize(uint8_t value);
    void setGrid(GridValue value);
    void setGrid(uint8_t x, uint8_t y, GridValue value);
    void setCol(uint8_t col, uint8_t index, uint8_t value);
    void setRow(uint8_t col, uint8_t index, uint8_t value);

    void incX();
    void decX();
    void incY();
    void decY();

    bool isColMatch(uint8_t col);
    bool isRowMatch(uint8_t row);
    void updateRowCols();

  private:


  private:

    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t rows[Constants::NumberOfNumbers * 16];
    uint8_t cols[Constants::NumberOfNumbers * 16];

};
