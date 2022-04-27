#pragma once
#include <Arduino.h>
#include "options.h"

void init_display(void);

void update_display(void);

void printMainMenu(Settings &settings);

void printPointer(const uint8_t pointer);

void printChosenBox(const uint8_t option);

void printPeriodAvailableOptions(void);

void printDurationAvailableOptions(void);

int getPeriodMs(MY_PERIOD period);

int getDurationMs(MY_DURATION duration);

String getPeriodPrintValue(MY_PERIOD period);

String getDurationdPrintValue(MY_DURATION duration);

void setup_period_callback_menu(void);

void setup_duration_callback_menu(void);