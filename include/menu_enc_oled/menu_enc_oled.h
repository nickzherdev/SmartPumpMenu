#pragma once
#include <Arduino.h>

void printMainMenu(void);

void printPointer(uint8_t pointer);

void printDots(void);

void periodCallback(void);

void durationCallback(void);

void nothingCallback(void);

// enum class MY_PERIOD { 
//   TEN_SEC, 
//   ONE_HOUR, 
//   // HALF_DAY,
//   ONE_DAY,
//   // TWO_DAYS,
//   // ONE_WEEK,
// };

enum class MY_PERIOD { 
  TEN_SEC, 
  ONE_HOUR, 
  ONE_DAY,
};

// int getMs(MY_PERIOD period);

String getPrintValue(MY_PERIOD period);
