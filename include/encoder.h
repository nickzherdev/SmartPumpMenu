#pragma once
#include <Arduino.h>
#include "options.h"

void handle_user_input_encoder(Settings &settings);

void periodCallback(uint8_t &chosen_period);

void durationCallback(uint8_t &chosen_duration);
