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

class Gateway{
public:
  Gateway();
  void setup();
  void loop();

  //
  void task1(void *pvParameters);
  void addBeacons(const char *name,const char* payload);
  void clearBeacons();

private:
  //BLE 関連
  void setupBle();
  void scanBle();

  //Wi-Fi 関連
  void setupWiFi();
  void setWifiPower(bool isOn);

  //firebase 関連
  void setupFirebase(std::function<void(bool)> callback);
  void postBeacons();
  void postFirebase(BeaconPayload* p);

private:
  Status status;

  BLEScan* pBLEScan;
  std::vector<BeaconPayload *> beacons;

  FirebaseArduino firebase;
  char url[255];
  char payload[512];
};

#endif
