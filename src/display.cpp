#include <GyverOLED.h>
#include "display.h"
#include "options.h"

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

void init_display(void) {
  oled.init();           // Инциализация дисплея
  oled.setContrast(100); // Макс. яркость
  oled.setScale(2);
  Wire.setClock(400000L);
}

void update_display(void) {
    oled.update();          // Выводим кадр на дисплей
}

void printMainMenu(Settings &settings) {
  oled.clear();           // Очищаем буфер
  oled.home();            // Курсор в левый верхний угол
  oled.println(F(" SmartPump"));
  String chosen_period_ = getPeriodPrintValue(static_cast<MY_PERIOD>(settings.chosen_period));
  oled.print(F(" Prd:  ")); oled.println(chosen_period_);
  String chosen_duration_ = getDurationdPrintValue(static_cast<MY_DURATION>(settings.chosen_duration));
  oled.print(F(" Drtn: ")); oled.println(chosen_duration_);
}

void printPointer(const uint8_t pointer) {
  oled.setCursor(0, pointer);  // Указатель в начале строки
  oled.print(">");
}

void setup_period_callback_menu(void) {
  oled.clear();
  oled.home();
  oled.println(F("Set period"));
}

void setup_duration_callback_menu(void) {
  oled.clear();
  oled.home();
  oled.println(F("Set duration"));
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

void printChosenBox(const uint8_t option) {
  if (option <= 2) {
    oled.roundRect(option*40, 20, 39+option*40, 40, OLED_STROKE);  // (x0,y0,x1,y1)
  }
  else if (option >2) {
    oled.roundRect((option-3)*43, 43, 37+(option-3)*44, 63, OLED_STROKE);  // (x0,y0,x1,y1)
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
