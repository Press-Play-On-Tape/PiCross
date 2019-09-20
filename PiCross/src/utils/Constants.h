#pragma once

namespace Constants {

  constexpr uint8_t GridWidthX = 8;
  constexpr uint8_t GridWidthY = 8;
  constexpr uint8_t NumberOfNumbers = 8;

  constexpr uint16_t PuzzleStartChar1 = 150;
  constexpr uint16_t PuzzleStartChar2 = PuzzleStartChar1 + 1;
  constexpr uint16_t PuzzleIndex = PuzzleStartChar2 + 1;
  constexpr uint16_t PuzzleWidth = PuzzleIndex + 2;
  constexpr uint16_t PuzzleHeight = PuzzleWidth + 1;
  constexpr uint16_t PuzzleMaxRows = PuzzleHeight + 1;
  constexpr uint16_t PuzzleMaxCols = PuzzleMaxRows + 1;
  constexpr uint16_t PuzzleStart = PuzzleMaxCols + 1;
  constexpr uint16_t PuzzleCols = PuzzleStart + (16 * 16);
  constexpr uint16_t PuzzleRows = PuzzleCols + (16 * NumberOfNumbers);

  constexpr uint16_t PuzzlesSolved = PuzzleRows + (16 * NumberOfNumbers);

  constexpr uint16_t Select_Spacing = 25;
  constexpr uint16_t Select_Centre = 55;
  constexpr uint16_t Select_Top = 20;
  constexpr uint16_t Select_Label = Select_Top + 23;

  constexpr uint16_t BButtonDelay = 64;
  constexpr uint8_t KeyRepeatDelay = 15;

}