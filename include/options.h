#pragma once
#include <Arduino.h>

struct Settings {
    uint8_t chosen_period;
    uint8_t chosen_duration;
};

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
