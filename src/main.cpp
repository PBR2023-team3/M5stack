#include <M5StickCPlus.h>
#include <WiFi.h>
#include <WiFiUDP.h>
#include "time.h"

int id = 1; //プレイヤーのID

const char *ssid = "ミィのiPhone";
const char *password = "LIFEischanging";
const char *ntpServer = "ntp.jst.mfeed.ad.jp";
// const char* saddr = "XXX.XXX.XXX.XXX"; //これはおおうち
const char *saddr = "172.20.10.3"; // taeget PC address

const int sport = 12345; //送信先のポート番号
const int kport = 5556;

RTC_TimeTypeDef RTC_TimeStruct;

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;
bool kick = false;
double theta = 0, last_theta = 0;
double phi = 0, last_phi = 0;
double alpha = 0.2;


uint8_t mac3[6];

WiFiUDP Udp;
char packetBuffer[255];

void convertTimeToString(RTC_TimeTypeDef timeStruct, char *TimeStr)
{
  // 例: HH:MM:SS
  sprintf(TimeStr, "%02d:%02d:%02d", timeStruct.Hours, timeStruct.Minutes, timeStruct.Seconds);
}

void setup()
{
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);

  M5.IMU.Init();

  esp_read_mac(mac3, ESP_MAC_WIFI_STA);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(1, 0, 2);
  M5.Lcd.println("Welcome \n If the connection fails, turn off and on the power again.");

  // connect to WiFi
  M5.Lcd.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("CONNECTED");
  //  Serial.println("CONNECTED");
  delay(1000);

  // Set ntp time to local
  configTime(9 * 3600, 0, ntpServer);
  // Get local time
  struct tm timeInfo;
  if (getLocalTime(&timeInfo))
  {
    // Set RTC time
    RTC_TimeTypeDef TimeStruct;
    TimeStruct.Hours = timeInfo.tm_hour;
    TimeStruct.Minutes = timeInfo.tm_min;
    TimeStruct.Seconds = timeInfo.tm_sec;
    M5.Rtc.SetTime(&TimeStruct);
  }
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 1, 2);
  M5.Lcd.print("Connected \nto ");
  M5.Lcd.println(WiFi.localIP());
  uint8_t mac3[6];
  esp_read_mac(mac3, ESP_MAC_WIFI_STA);
  M5.Lcd.printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac3[0], mac3[1], mac3[2], mac3[3], mac3[4], mac3[5]);
  Udp.begin(kport);
}

void loop()
{
  int mil = millis();
  // put your main code here, to run repeatedly:
  M5.Rtc.GetTime(&RTC_TimeStruct);
  M5.Lcd.setCursor(0, 50);
  // M5.Lcd.printf("%04d-%02d-%02d ", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Date);
  //  M5.Lcd.printf("millis: %6d\n",mil);
  M5.Lcd.printf("%02d:%02d:%02d\n", RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
  //  Serial.printf("%04d-%02d-%02d (%s) %02d : %02d : %02d\n", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Date, RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);

  // 加速度データ取得
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  M5.Lcd.printf("%5.2f, %5.2f, %5.2f\n", accX, accY, accZ);
  if ((accX < 1) && (accX > -1))
  {
    theta = asin(-accX) * 57.295;
  }
  if (accZ != 0)
  {
    phi = atan(accY / accZ) * 57.295;
  }
  theta = alpha * theta + (1 - alpha) * last_theta;
  phi = alpha * phi + (1 - alpha) * last_phi;
  last_theta = theta;
  last_phi = phi;

  if (abs(theta) > 30 ){
    kick = true;
  }else{
    kick = false;
  }


    // UDP
    Udp.beginPacket(saddr, sport);
  String IDStr(id);
  String DataTypeStr(1); //M5Stick 
  String KickStr(kick);
  String accXStr(accX);
  String accYStr(accY);
  String accZStr(accZ);

  char TimeStr[9]; // 文字列格納用のバッファ
  convertTimeToString(RTC_TimeStruct, TimeStr);

  String state_str = "{ \"id\":" + IDStr +
                     ", \"type\":" + DataTypeStr +
                     ", \"kick\":" + KickStr + "}";

  Udp.printf("%s", state_str.c_str());
  Udp.endPacket();
  delay(100);
}