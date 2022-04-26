#pragma once
#include <Arduino.h>

void printMainMenu(void);

void printPointer(uint8_t pointer);

void printDots(void);

void periodCallback(void);

void durationCallback(void);

void nothingCallback(void);

enum class MY_PERIOD { 
  TEN_SEC, 
  ONE_HOUR, 
  ONE_DAY,
  TWO_DAYS,
  ONE_WEEK,
};

int getPeriodMs(MY_PERIOD period);

String getPeriodPrintValue(MY_PERIOD period);

void printPeriodAvailableOptions(void);

void printChosenBox(uint8_t option);
