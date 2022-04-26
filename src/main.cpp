#include <GyverOLED.h>
#include <EncButton.h>
#include "menu_enc_oled/menu_enc_oled.h"

// #define OLED_SOFT_BUFFER_64     // Буфер на стороне МК
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
// GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
// GyverOLED<SSD1306_128x64> oled;
EncButton<EB_TICK, D6, D5, D7> enc;  // энкодер с кнопкой <A, B, KEY>

#define ITEMS 3               // Общее кол во пунктов, без заголовка
#define MYSCALE 2
#define MY_PERIOD_COUNT 3

String chosen_period_status;

void setup() {
  Serial.begin(9600);
  oled.init();           // Инциализация дисплея
  oled.setContrast(100); // Макс. яркость
  oled.setScale(MYSCALE);
  Wire.setClock(400000L);
}

void loop() {
  static int8_t pointer = 2; // Переменная указатель

  enc.tick();                     // опрос происходит здесь

  if (enc.turn()) {
    if (enc.left()) {
      pointer = constrain(pointer - MYSCALE, 2, ITEMS*MYSCALE); // Двигаем указатель в пределах дисплея
    }

    if (enc.right()) {
      pointer = constrain(pointer + MYSCALE, 2, ITEMS*MYSCALE);
    }

    printMainMenu();
    printPointer(pointer);  // Вывод указателя
    oled.update();          // Выводим кадр на дисплей
  }

  if (enc.click()) {   // Нажатие на ОК - переход в пункт меню
    switch (pointer) {  // По номеру указателей располагаем вложенные функции (можно вложенные меню)
      // case 0: break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
      case 2: periodCallback(); break;
      case 4: durationCallback(); break;
      case 6: nothingCallback(); break;
    }

    printMainMenu();
    printPointer(pointer);  // Вывод указателя
    oled.update();          // Выводим кадр на дисплей
  }
}

void printMainMenu(void) {
  /* меню */
  oled.clear();           // Очищаем буфер
  oled.home();            // Курсор в левый верхний угол
  oled.println(F(" SmartPump"));
  oled.print(F(" Prd:  ")); oled.println(chosen_period_status);
  oled.println(F(" Drtn: ")); //oled.println("42");
  oled.println(F(" Smth: "));
}

void printPointer(uint8_t pointer) {
  oled.setCursor(0, pointer);  // Указатель в начале строки
  oled.print(">");
}

void periodCallback(void) {
  oled.clear();
  oled.home();
  oled.println(F("Set period"));
  oled.setCursor(20, 4);
  // printAvailableOptions();
  // printChosenBox(chosen_period_status);
  // oled.print(chosen_period_status);
  oled.update();

  String tmp_per_status;

  while (1) {
    delay(1);  // to prevent software WDT resetting after 3 sec of blocking
    enc.tick();

    static uint8_t my_period = 0; // Переменная указатель
    if (enc.turn()) {
      oled.clear();
      oled.home();
      oled.println(F("Set period"));

      if (enc.left()) {
        my_period = constrain(my_period - 1, 0, MY_PERIOD_COUNT-1);
      }
      if (enc.right()) {
        my_period = constrain(my_period + 1, 0, MY_PERIOD_COUNT-1);
      }

      tmp_per_status = getPrintValue(static_cast<MY_PERIOD>(my_period));
      // printChosenBox(tmp_per_status);
      oled.setCursor(10, 4);  // Указатель в начале строки
      oled.println(tmp_per_status);
      oled.update();
    }
    if (enc.click()) {
      chosen_period_status = tmp_per_status;
      // Serial.print("Chosen: "); Serial.println(chosen_period_status);
      return;
    } // return возвращает нас в предыдущее меню
  }
}

void durationCallback(void) {
  oled.clear();
  oled.home();
  oled.print(F("Set duration"));
  oled.update();
  while (1) {
    delay(1);  // to prevent software WDT resetting after 3 sec of blocking
    enc.tick();
    if (enc.click()) return; // return возвращает нас в предыдущее меню
  }
}

void nothingCallback(void) {
  oled.clear();
  oled.home();
  oled.print(F("Hello!"));
  oled.update();
  while (1) {
    enc.tick();
    if (enc.click()) return; // return возвращает нас в предыдущее меню
  }
}

String getPrintValue(MY_PERIOD period) {
  if (period == MY_PERIOD::TEN_SEC)
    return String("10s");
  if (period == MY_PERIOD::ONE_HOUR)
    return String("1h");
  if (period == MY_PERIOD::ONE_DAY)
    return String("1d");
  else {
    return String("None");
  }
}

// int getMs(MY_PERIOD period) {
//   if (period == MY_PERIOD::TEN_SEC)
//     return 10*1000;
//   if (period == MY_PERIOD::ONE_HOUR)
//     return 60*60*1000;
//   if (period == MY_PERIOD::ONE_DAY)
//     return 24*60*60*1000;
//   else return -1;
// }