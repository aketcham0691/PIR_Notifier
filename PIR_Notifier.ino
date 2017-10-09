/*******************************************************************
 *  Push Notificaiton Demo                                         *
 *  Using IFTTT and Telegram to send alerts to your phone          *
 *                                                                 *
 *  Telegram Library:                                              *
 *  https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot *
 *                                                                 *
 *  IFTTT Maker Library:                                           *
 *  https://github.com/witnessmenow/arduino-ifttt-maker            *
 *                                                                 *
 *  By Brian Lough                                                 *
 *******************************************************************/

//Including the two libraries
#include <UniversalTelegramBot.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//------- WiFi Settings -------
char ssid[] = "NETGEAR22";     
char password[] = "modernearth850"; 

#define TELEGRAM_BUTTON_PIN D5

// ------- Telegram config --------
#define BOT_TOKEN "477612027:AAF02yFlCL0JX02jyya88Nm_3NBzB8MtupY" 
#define CHAT_ID "373242419"

// SSL client needed for both libraries
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

String ipAddress = "";

volatile bool telegramButtonPressedFlag = false;

void setup() {

  Serial.begin(115200);

  // Initlaze the buttons
  pinMode(TELEGRAM_BUTTON_PIN, INPUT);

  // NOTE:
  // It is important to use interupts when making network calls in your sketch
  // if you just checked the status of te button in the loop you might
  // miss the button press.
  attachInterrupt(TELEGRAM_BUTTON_PIN, telegramButtonPressed, RISING);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  ipAddress = ip.toString();

}

void telegramButtonPressed() {
  Serial.println("telegramButtonPressed");
  int button = digitalRead(TELEGRAM_BUTTON_PIN);
  if(button == HIGH)
  {
    telegramButtonPressedFlag = true;
  }
  return;
}


void sendTelegramMessage() {
  String message = "I just went out!";
  if(bot.sendMessage(CHAT_ID, message, "Markdown")){
    Serial.println("TELEGRAM Successfully sent");
  }
  telegramButtonPressedFlag = false;
}

void loop() {
  if ( telegramButtonPressedFlag ) {
    sendTelegramMessage();
  }
}
