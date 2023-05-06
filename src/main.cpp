/*
    Прием отчета с Serial и отправка на почту.
    201905-26
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Mail.h"
#include <ESP8266Ping.h>

const char *const staSSID[] = {"link", "Report", "dz", "Press", "Home"};
const char *const staPass[] = {"dkfgf#*12091997", "vtormetall2000", "metprofit2000", "99887766554", "998877**"};
// const char *const staSSID = "Report";
// const char *const staPass = "vtormetall2000";
// const char *const staSSID = "dz";
// const char *const staPass = "metprofit2000";
// const char *const staSSID = "Press";
// const char *const staPass = "99887766554";

const char *remote_host = "www.yandex.ru";

const char *const smtpHost[] = {"smtp.yandex.ru", "smtp.mail.ru", "smtp.gmail.com"};
const uint16_t smtpPort = 465;
const char *const smtpUser[] = {"selecat@ya.ru", "vlapa1@mail.ru", "vlapa1973@gmail.com"};
const char *const smtpPass[] = {"dnjhvtnfkk2000", "Dkfgf#*12091997", "sqdlhsckqscivhxx"};
const char *const mailTo = {"redbuilder@ya.ru"};
const char *const mailSubject = "Отчет за смену.";

String Data = "";
String a = "";
uint8_t number = 1;   // количество почтовых ящиков на которые отправлять

//*******************************************************************
void MailData()
{
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to: "));

  uint8_t i = 0;
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("\"");
    Serial.print(staSSID[i]);
    Serial.print('"');
    WiFi.begin(staSSID[i], staPass[i]);
    delay(5000);
    Serial.println(i);
    Serial.println(WiFi.localIP());
    i++;
    if (i == sizeof(staSSID))
    {
      i = 0;
    }
  }

  Serial.print("Ping host ");
  Serial.println(remote_host);
  bool x = true;
  while (x)
  {
    if (Ping.ping(remote_host))
    {
      Serial.println("Ping success !");
      x = false;
    }
    else
    {
      Serial.println("Ping error !");
    }
    Serial.println("");
  }
  delay(100);
  Serial.println("Ready");

  bool sendFlag = false;
  while (!sendFlag)
  {
    while (Serial.available())
    {
      a += (char)Serial.read();
      sendFlag = true;
      delay(2);
    }
  }

  for (int8_t i = 0; i < number; ++i)
  {
    Serial.println();
    if (sendMail(smtpHost[i], smtpPort, smtpUser[i], smtpPass[i], mailTo, mailSubject, a))
    {
      Serial.print(F("Mail sended "));
      Serial.println(smtpHost[i]);
    }
    else
    {
      Serial.print(F("Error sending mail"));
      Serial.print(smtpHost[i]);
      Serial.println('!');
    }
  }

  WiFi.disconnect();
  a = "";
}

//*******************************************************************
void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  digitalWrite(LED_BUILTIN, LOW);
  MailData();
  digitalWrite(LED_BUILTIN, HIGH);

  ESP.deepSleep(0);
}

void loop() {}