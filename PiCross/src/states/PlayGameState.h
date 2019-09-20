#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "../entities/Entities.h"
#include "BaseState.h"

class PlayGameState : public BaseState {

  private:

    Puzzle puzzle;


  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

  private:

    uint8_t maxSeriesRow = 0;
    uint8_t maxSeriesCol = 0;
    uint8_t marginTop = 0;
    uint8_t marginLeft = 0;

    bool gameOver = false;
    uint8_t counter = 0;
    uint8_t xOffset = 0;
    uint8_t yOffset = 0;

    uint8_t keyCount = 0;
    uint8_t bCount = 0;
    GridValue lastUpdate = GridValue::Blank;
};




