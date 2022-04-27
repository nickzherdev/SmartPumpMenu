#pragma once
#include <Arduino.h>

void handle_user_input_encoder(void);

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

enum class MY_DURATION {
  FIVE_SEC,
  SEVEN_SEC,
  TEN_SEC,
  FIFTEEN_SEC,
  THIRTY_SEC,
  ONE_MINUTE
};

int getPeriodMs(MY_PERIOD period);

String getPeriodPrintValue(MY_PERIOD period);

void printPeriodAvailableOptions(void);

void printChosenBox(uint8_t option);

String getDurationdPrintValue(MY_DURATION duration);

void printDurationAvailableOptions(void);