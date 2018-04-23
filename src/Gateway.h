#ifndef GATEWAY_H_
#define GATEWAY_H_

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#include "Config.h"
#include "BeaconPayload.h"
#include "Status.h"

#include <Firebase.h>

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include <WiFi.h>

//SmartConfig へ移行するまでの時間 sec
#define WiFI_CONFIG_TIMEOUT 15

//割り込みに利用する IO0
#define SW0 0

#define LED1 2

#define LEDC_CHANNEL_R 0
#define LEDC_CHANNEL_G 1
#define LEDC_CHANNEL_B 2
#define LEDC_TIMER_8_BIT  8
#define LEDC_BASE_FREQ     5000
#define LED2_R 23
#define LED2_G 22
#define LED2_B 21

class Gateway{
public:
  Gateway();
  void setup();
  void loop();

  //別CPU処理やIO割り込み
  void task1(void *pvParameters);
  void interrupt();

  void addBeacons(const char *name,const char* payload);
  void clearBeacons();

private:
  //BLE 関連
  void setupBle();
  void scanBle();

  //Wi-Fi 関連
  void setupWiFi();
  void wifiSmartConfig(int cnt);
  void setWifiPower(bool isOn);

  //firebase 関連
  void setupFirebase(std::function<void(bool)> callback);
  void postBeacons();
  void postFirebase(BeaconPayload* p);

  //GPIO 関連
  void setupGpio();
  void ledRGB(int r,int g,int b);
  void led1(bool isOn);

private:
  Status status;

  BLEScan* pBLEScan;
  std::vector<BeaconPayload *> beacons;

  FirebaseArduino firebase;
  char url[255];
  char payload[512];
};

#endif
