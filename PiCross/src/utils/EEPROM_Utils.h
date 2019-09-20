#pragma once

#include "Arduboy2Ext.h"
#include "Constants.h"



class EEPROM_Utils {

  public: 

    EEPROM_Utils() {};
        
    static void initEEPROM(bool forceClear);

};


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'P' and 'I' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */
void EEPROM_Utils::initEEPROM(bool forceClear) {

  byte c1 = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStartChar1));
  byte c2 = eeprom_read_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStartChar2));

  if (forceClear || c1 != 'P' || c2 != 'I') { 

    const uint16_t index = 0;
    eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStartChar1), 'P');
    eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleStartChar2), 'I');
    eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleWidth), 0);
    eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzleHeight), 0);
    eeprom_update_word(reinterpret_cast<uint16_t *>(Constants::PuzzleIndex), index);

    for (uint16_t x = 0; x < 12 * 25; x++) {
      eeprom_update_byte(reinterpret_cast<uint8_t *>(Constants::PuzzlesSolved + x), 0);
    }

  }

}

