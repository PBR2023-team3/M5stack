/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC-Plus sample source code
*                          配套  M5StickC-Plus 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/m5stickc_plus
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/m5stickc_plus
*
* Describe: Wifi scan.  wifi扫描
* Date: 2022/4/3
*******************************************************************************
  http://forum.m5stack.com/topic/58/lesson-3-wi-fi-scanner
  Press M5 to start/re-scan wifi, press up to turn down the page
  按下M5键以开始/重新扫描wifi,按下上键向下翻页
*/

#include <M5StickCPlus.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
// #include <WebServer.h>
// #include <ESPmDNS.h>

const char *ssid = "ミィのiPhone";                       // Enter SSID
const char *passwd = "LIFEischanging";                 // Enter Password
const char *websockets_server_host = "172.20.10.5";    // サーバーの実際のIPアドレスに変更
const uint16_t websockets_server_port = 8765;

WebSocketsClient client;
DynamicJsonDocument doc(1024);

int ID = 1;
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

// //受信したデータを取得する場合
// std::string parseReceivedJson(uint8_t *payload)
// {
//   char *json = (char *)payload;
//   DeserializationError error = deserializeJson(doc, json);
//   if (error)
//   {
//     Serial.print(F("deserializeJson() failed: "));
//     Serial.println(error.c_str());
//     return "none";
//   }
//   JsonObject obj = doc.as<JsonObject>();

//   // You can use a String to get an element of a JsonObject
//   // No duplication is done.
//   return obj[String("color")];
// }

// // 取得したデータから機能を与える場合
// void syncColor(uint8_t *payload)
// {
//   std::string color = parseReceivedJson(payload);

//   Serial.printf("color: %s\n", color.c_str());
//   // M5.Lcd.fillRect(60, 20, 200, 200, colorMap[color]);
//   M5.Lcd.fillScreen(colorMap[color]);
// }

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    M5.Lcd.printf("[WSc] Disconnected!\n");
    // client.sendTXT("DisConnected");
    break;
  case WStype_CONNECTED:
    M5.Lcd.printf("[WSc] Connected to url: %s\n", payload);
    client.sendTXT("Connected");
    break;
  case WStype_TEXT:
    M5.Lcd.printf("[WSc] get text: %s\n", payload);
    // client.sendTXT("get text");
    // syncColor(payload);
    break;
  case WStype_BIN:
  case WStype_ERROR:
  case WStype_FRAGMENT_TEXT_START:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
    break;
  }
}

void setupWiFi()
{
  WiFi.begin(ssid, passwd);

  // Wait some time to connect to wifi
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++)
  {
    M5.Lcd.print(".");
    delay(1000);
  }

  // Check if connected to wifi
  if (WiFi.status() != WL_CONNECTED)
  {
    M5.Lcd.println("No Wifi!");
    return;
  }

  M5.Lcd.println("Connected to Wifi, Connecting to server.");

  client.begin(websockets_server_host, websockets_server_port, "/");

  M5.Lcd.println("Connected!");
  client.onEvent(webSocketEvent);
  client.setReconnectInterval(5000);

  // // use HTTP Basic Authorization this is optional remove if not needed
  // // webSocket.setAuthorization("user", "Password");
}

  // WebServer server(80); // WebServerオブジェクト作成

  void setup()
  {
    M5.begin();
    M5.Imu.Init(); // Init IMU.  初始化IMU
    M5.Lcd.setRotation(3);

    M5.Lcd.setTextSize(1);
    setupWiFi();
  }

  void loop()
  {
    static uint32_t pre_send_time = 0;
    uint32_t time = millis();
    if (time - pre_send_time > 100)
    {
      pre_send_time = time;
      M5.IMU.getAccelData(&accX, &accY, &accZ);
      String IDStr(ID);
      String accXStr(accX);
      String accYStr(accY);
      String accZStr(accZ);

      String state_str = "{ \"ID\":" + IDStr +
                         ", \"accX\":" + accXStr +
                         ", \"accY\":" + accYStr +
                         ", \"accZ\":" + accZStr + "}";
      client.sendTXT(state_str);
      // Serial.print(state_str);
      // Serial.printf("\n");
    }
    client.loop();
    M5.update();
  }