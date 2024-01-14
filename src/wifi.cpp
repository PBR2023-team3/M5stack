// /*
// *******************************************************************************
// * Copyright (c) 2021 by M5Stack
// *                  Equipped with M5StickC-Plus sample source code
// *                          配套  M5StickC-Plus 示例源代码
// * Visit for more information: https://docs.m5stack.com/en/core/m5stickc_plus
// * 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/m5stickc_plus
// *
// * Describe: Wifi scan.  wifi扫描
// * Date: 2022/4/3
// *******************************************************************************
//   http://forum.m5stack.com/topic/58/lesson-3-wi-fi-scanner
//   Press M5 to start/re-scan wifi, press up to turn down the page
//   按下M5键以开始/重新扫描wifi,按下上键向下翻页
// */

// #include <M5StickCPlus.h>

// #include <WebServer.h>
// #include <ESPmDNS.h>
// // #include <WiFi.h>

// const char *ssid = "ミィのiPhone";
// const char *password = "LIFEischanging";

// WebServer server(80); // WebServerオブジェクト作成

// void handleIMU();
// void handleNotFound();
// void WiFiConnect();

// float accX = 0.0F;
// float accY = 0.0F;
// float accZ = 0.0F;

// void WiFiConnect()
// {
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     M5.Lcd.print(".");
//   }
//   M5.Lcd.print("\r\nWiFi connected\r\nIP address: ");
//   M5.Lcd.println(WiFi.localIP());
// }

// void setup()
// {
//   M5.begin();
//   M5.Imu.Init(); // Init IMU.  初始化IMU
//   M5.Lcd.setTextSize(2);

//   WiFiConnect();
//   // マルチキャストDNSにホスト名を登録
//   if (MDNS.begin("m5stack"))
//   {
//     M5.Lcd.println("MDNS responder started");
//   }
//   server.on("/", handleIMU);        // URLにアクセスされた際の動作を登録
//   server.onNotFound(handleNotFound); // server.onで登録されていないURLにアクセスされた際の動作を登録
//   server.begin();                    // クライアントからの接続応答待ちを開始
//   M5.Lcd.println("HTTP server started");
// }

// void loop()
// {
//   server.handleClient();
// }

// void handleIMU()
// {
//   char buf[400];
//   M5.IMU.getAccelData(&accX, &accY, &accZ);

//   sprintf(buf,
//           "<html>\
//      <head>\
//         <title>Temperature Measurement</title>\
//      </head>\
//      <body>\
//         <h1>M5Stack ADT7410 Temperature Measurement</h1>\
//         <p>accX: %.2f 'C</p>\
//         <p>accY: %.2f 'C</p>\
//         <p>accZ: %.2f 'C</p>\
//      </body>\
//      </html>",
//           accX, accY, accZ);
//   server.send(200, "text/html", buf);
//   M5.Lcd.println("accessed on \"/\"");
// }

// void handleNotFound()
// {
//   server.send(404, "text/plain", "File Not Found\n\n");
//   M5.Lcd.println("File Not Found");
// }