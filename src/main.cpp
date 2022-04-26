#include <GyverOLED.h>
#include <EncButton.h>
#include "menu_enc_oled/menu_enc_oled.h"

// #define OLED_SOFT_BUFFER_64     // Буфер на стороне МК
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
// GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
// GyverOLED<SSD1306_128x64> oled;
EncButton<EB_TICK, D6, D5, D7> enc;  // энкодер с кнопкой <A, B, KEY>

#define ITEMS 2               // Общее кол во пунктов, без заголовка
#define MYSCALE 2
#define MY_PERIOD_COUNT 5
#define MY_DURATION_COUNT 6

uint8_t chosen_period;
uint8_t chosen_duration;

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
  auto chosen_period_ = getPeriodPrintValue(static_cast<MY_PERIOD>(chosen_period));
  oled.print(F(" Prd:  ")); oled.println(chosen_period_);
  auto chosen_duration_ = getDurationdPrintValue(static_cast<MY_DURATION>(chosen_duration));
  oled.print(F(" Drtn: ")); oled.println(chosen_duration_);
}

void printPointer(uint8_t pointer) {
  oled.setCursor(0, pointer);  // Указатель в начале строки
  oled.print(">");
}

void periodCallback(void) {
  oled.clear();
  oled.home();
  oled.println(F("Set period"));
  printPeriodAvailableOptions();
  printChosenBox(chosen_period);
  oled.update();

  while (1) {
    delay(1);  // to prevent software WDT resetting after 3 sec of blocking
    enc.tick();

    static uint8_t tmp_period = 0; // Переменная указатель
    if (enc.turn()) {
      oled.clear();
      oled.home();
      oled.println(F("Set period"));

      if (enc.left()) {
        tmp_period = constrain(tmp_period - 1, 0, MY_PERIOD_COUNT-1);
      }
      if (enc.right()) {
        tmp_period = constrain(tmp_period + 1, 0, MY_PERIOD_COUNT-1);
      }

      oled.clear();
      oled.home();
      oled.println(F("Set period"));
      printPeriodAvailableOptions();
      printChosenBox(tmp_period);
      oled.update();
    }
    if (enc.click()) {
      chosen_period = tmp_period;
      return; // return возвращает нас в предыдущее меню
    } 
  }
}

void durationCallback(void) {
  oled.clear();
  oled.home();
  oled.print(F("Set duration"));
  printDurationAvailableOptions();
  printChosenBox(chosen_duration);
  oled.update();

  while (1) {
    delay(1);  // to prevent software WDT resetting after 3 sec of blocking
    enc.tick();

    static uint8_t tmp_duration = 0; // Переменная указатель
    if (enc.turn()) {
      oled.clear();
      oled.home();
      oled.println(F("Set duration"));

      if (enc.left()) {
        tmp_duration = constrain(tmp_duration - 1, 0, MY_DURATION_COUNT-1);
      }
      if (enc.right()) {
        tmp_duration = constrain(tmp_duration + 1, 0, MY_DURATION_COUNT-1);
      }

      oled.clear();
      oled.home();
      oled.println(F("Set duration"));
      printDurationAvailableOptions();
      printChosenBox(tmp_duration);
      oled.update();
    }
    if (enc.click()) {
      chosen_duration = tmp_duration;
      return; // return возвращает нас в предыдущее меню
    } 
  }
}

String getPeriodPrintValue(MY_PERIOD period) {
  if (period == MY_PERIOD::TEN_SEC)
    return String("10s");
  if (period == MY_PERIOD::ONE_HOUR)
    return String("1h");
  if (period == MY_PERIOD::ONE_DAY)
    return String("1d");
  if (period == MY_PERIOD::TWO_DAYS)
    return String("2d");
  if (period == MY_PERIOD::ONE_WEEK)
    return String("1w");
  else {
    return String("None");
  }
}

String getDurationdPrintValue(MY_DURATION duration) {
  if (duration == MY_DURATION::FIVE_SEC)
    return String("5s");
  if (duration == MY_DURATION::SEVEN_SEC)
    return String("7s");
  if (duration == MY_DURATION::TEN_SEC)
    return String("10s");
  if (duration == MY_DURATION::FIFTEEN_SEC)
    return String("15s");
  if (duration == MY_DURATION::THIRTY_SEC)
    return String("30s");
  if (duration == MY_DURATION::ONE_MINUTE)
    return String("1m");
  else {
    return String("None");
  }
}

void printPeriodAvailableOptions(void) {
  oled.setCursor(1, 3);
  auto v0 = getPeriodPrintValue(MY_PERIOD::TEN_SEC);
  oled.print(v0); oled.print(" ");
  auto v1 = getPeriodPrintValue(MY_PERIOD::ONE_HOUR);
  oled.print(v1); oled.print(" ");
  auto v2 = getPeriodPrintValue(MY_PERIOD::ONE_DAY);
  oled.print(v2); oled.print(" ");

  oled.setCursor(1, 6);
  auto v3 = getPeriodPrintValue(MY_PERIOD::TWO_DAYS);
  oled.print(" "); oled.print(v3); oled.print(" ");
  auto v4 = getPeriodPrintValue(MY_PERIOD::ONE_WEEK);
  oled.print(v4); oled.print(" ");

  oled.update();
}

void printDurationAvailableOptions(void) {
  oled.setCursor(0, 3);
  auto v0 = getDurationdPrintValue(MY_DURATION::FIVE_SEC);
  oled.print(" "); oled.print(v0); oled.print(" ");
  auto v1 = getDurationdPrintValue(MY_DURATION::SEVEN_SEC);
  oled.print(v1); oled.print(" ");
  auto v2 = getDurationdPrintValue(MY_DURATION::TEN_SEC);
  oled.print(v2); oled.print(" ");

  oled.setCursor(0, 6);
  auto v3 = getDurationdPrintValue(MY_DURATION::FIFTEEN_SEC);
  oled.print(v3); oled.print(" ");
  auto v4 = getDurationdPrintValue(MY_DURATION::THIRTY_SEC);
  oled.print(v4); oled.print(" ");
  auto v5 = getDurationdPrintValue(MY_DURATION::ONE_MINUTE);
  oled.print(v5); oled.print(" ");
  oled.update();
}

void printChosenBox(uint8_t option) {
  if (option <= 2) {
    oled.roundRect(option*40, 20, 39+option*40, 40, OLED_STROKE);  // скруглённый прямоугольник (x0,y0,x1,y1)
  }
  else if (option >2) {
    oled.roundRect((option-3)*43, 43, 37+(option-3)*44, 63, OLED_STROKE);  // скруглённый прямоугольник (x0,y0,x1,y1)
  }
}

int getPeriodMs(MY_PERIOD period) {
  if (period == MY_PERIOD::TEN_SEC)
    return 10*1000;
  if (period == MY_PERIOD::ONE_HOUR)
    return 60*60*1000;
  if (period == MY_PERIOD::ONE_DAY)
    return 24*60*60*1000;
  if (period == MY_PERIOD::TWO_DAYS)
    return 2*24*60*60*1000;
  if (period == MY_PERIOD::ONE_WEEK)
    return 7*24*60*60*1000;
  else return -1;
}

int getDurationMs(MY_DURATION duration) {
  if (duration == MY_DURATION::FIVE_SEC)
    return 5*1000;
  if (duration == MY_DURATION::SEVEN_SEC)
    return 7*1000;
  if (duration == MY_DURATION::TEN_SEC)
    return 10*1000;
  if (duration == MY_DURATION::FIFTEEN_SEC)
    return 15*1000;
  if (duration == MY_DURATION::THIRTY_SEC)
    return 30*1000;
  if (duration == MY_DURATION::ONE_MINUTE)
    return 60*1000;
  else return -1;
}