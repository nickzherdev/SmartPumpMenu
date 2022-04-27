#include "encoder.h"
#include "display.h"
#include "options.h"
#include "telegram_bot.h"

Settings settings;

void setup() {
  Serial.begin(115200);
  init_display();
  connect_wifi();
}

void loop() {
  handle_user_input_encoder(settings);
  start_bot();
}
