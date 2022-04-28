#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "telegram_bot.h"
#include "options.h"
#include "display.h"

// Wifi network station credentials
#define WIFI_SSID "Mikail Shanson"
#define WIFI_PASSWORD "mika2012"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "5325018828:AAHVMde5YaxBqi15Tbxliu2w7oWSpia0Caw"
#define CHAT_ID "62985"

const int ledPin = 2;
bool ledState = HIGH;

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;          
int ledStatus = 0;

// Checks for new messages every 1 second.
const unsigned int botRequestDelay = 5000; // mean time between scan messages
unsigned long lastTimeBotRan; // last time messages' scan has been done

void handleNewMessages(int numNewMessages, Settings &settings) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++)
  {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/ledon")
    {
      digitalWrite(ledPin, LOW); // turn the LED on (HIGH is the voltage level)
      ledStatus = 1;
      bot.sendMessage(chat_id, "Led is ON", "");
    }

    if (text == "/ledoff")
    {
      ledStatus = 0;
      digitalWrite(ledPin, HIGH); // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Led is OFF", "");
    }

    if (text == "set_period")
    {
      String keyboardJson = "[[\"10s\", \"1h\", \"1d\"],[\"2d\", \"1w\"],[\"get_period\", \"/options\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
    }
    if (text == "10s")
    {
      settings.chosen_period = 0;
      bot.sendMessage(chat_id, "10s period is set", "");
    }
    if (text == "2d")
    {
      settings.chosen_period = 3;
      bot.sendMessage(chat_id, "2d period is set", "");
    }
    if (text == "get_period")
    {
      String message = getPeriodPrintValue(static_cast<MY_PERIOD>(settings.chosen_period));
      bot.sendMessage(chat_id, message, "");
    }
    if (text == "get_duration")
    {
      String message = getDurationdPrintValue(static_cast<MY_DURATION>(settings.chosen_duration));
      bot.sendMessage(chat_id, message, "");
    }
    if (text == "/status")
    {
      if (ledStatus)
      {
        bot.sendMessage(chat_id, "Led is ON", "");
      }
      else
      {
        bot.sendMessage(chat_id, "Led is OFF", "");
      }
    }

    if (text == "/options")
    {
      String keyboardJson = "[[\"/ledon\", \"/ledoff\"],[\"set_period\", \"set_duration\"],[\"/status\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
    }

    if (text == "/start")
    {
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "This is Reply Keyboard Markup example.\n\n";
      welcome += "/ledon : to switch the Led ON\n";
      welcome += "/ledoff : to switch the Led OFF\n";
      welcome += "/set_period : to set period\n";
      welcome += "/status : Returns current status of LED\n";
      welcome += "/options : returns the reply keyboard\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void connect_wifi(void)
{
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  Serial.print(" ");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();

  Serial.print("WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(ledPin, ledState); // initialize pin as off
}

void run_bot(Settings &settings) {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    // was "while"
    // removed because of blocking main thread
    if(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages, settings);
      printMainMenu(settings);
      update_display();
      // numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}
