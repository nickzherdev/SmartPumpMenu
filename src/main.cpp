#include "encoder.h"
#include "display.h"
#include "options.h"

Settings settings;

void setup() {
  Serial.begin(9600);
  init_display();
}

void loop() {
  handle_user_input_encoder(settings);
}
