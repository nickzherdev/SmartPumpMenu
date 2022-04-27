#include <EncButton.h>
#include "encoder.h"
#include "display.h"
#include "options.h"

#define ITEMS 2               // Общее кол во пунктов, без заголовка
#define MY_PERIOD_COUNT 5
#define MY_DURATION_COUNT 6

EncButton<EB_TICK, D6, D5, D7> enc;  // энкодер с кнопкой <A, B, KEY>

void handle_user_input_encoder(Settings &settings) {
  enc.tick();                     // опрос происходит здесь  
  static int8_t pointer = 2; // Переменная указатель

  if (enc.turn()) {
    if (enc.left()) {
      pointer = constrain(pointer - 2, 2, ITEMS*2); // Двигаем указатель в пределах дисплея
    }

    if (enc.right()) {
      pointer = constrain(pointer + 2, 2, ITEMS*2);
    }

    printMainMenu(settings);
    printPointer(pointer);  // Вывод указателя
    update_display();
  }

  if (enc.click()) {   // Нажатие на ОК - переход в пункт меню
    switch (pointer) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
      case 2: periodCallback(settings.chosen_period); break;
      case 4: durationCallback(settings.chosen_duration); break;
    }

    printMainMenu(settings);
    printPointer(pointer);  // Вывод указателя
    update_display();
  }
}

void periodCallback(uint8_t &chosen_period) {
  setup_period_callback_menu();
  printPeriodAvailableOptions();
  printChosenBox(chosen_period);
  update_display();

  while (1) {
    delay(1);  // to prevent software WDT resetting after 3 sec of blocking
    enc.tick();

    static uint8_t tmp_period = 0; // Переменная указатель
    if (enc.turn()) {
      setup_period_callback_menu();

      if (enc.left()) {
        tmp_period = constrain(tmp_period - 1, 0, MY_PERIOD_COUNT-1);
      }
      if (enc.right()) {
        tmp_period = constrain(tmp_period + 1, 0, MY_PERIOD_COUNT-1);
      }

      setup_period_callback_menu();
      printPeriodAvailableOptions();
      printChosenBox(tmp_period);
      update_display();
    }
    if (enc.click()) {
      chosen_period = tmp_period;
      return; // return возвращает нас в предыдущее меню
    } 
  }
}

void durationCallback(uint8_t &chosen_duration) {
  setup_duration_callback_menu();
  printDurationAvailableOptions();
  printChosenBox(chosen_duration);
  update_display();

  while (1) {
    delay(1);  // to prevent software WDT resetting after 3 sec of blocking
    enc.tick();

    static uint8_t tmp_duration = 0; // Переменная указатель
    if (enc.turn()) {
      setup_duration_callback_menu();

      if (enc.left()) {
        tmp_duration = constrain(tmp_duration - 1, 0, MY_DURATION_COUNT-1);
      }
      if (enc.right()) {
        tmp_duration = constrain(tmp_duration + 1, 0, MY_DURATION_COUNT-1);
      }

      setup_duration_callback_menu();
      printDurationAvailableOptions();
      printChosenBox(tmp_duration);
      update_display();
    }
    if (enc.click()) {
      chosen_duration = tmp_duration;
      return; // return возвращает нас в предыдущее меню
    } 
  }
}
