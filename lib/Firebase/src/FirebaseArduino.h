#ifndef FIREBASE_ARDUINO_H_
#define FIREBASE_ARDUINO_H_

#include "ArduinoJson.h"
#include <HTTPClient.h>
#include <functional>

//インターフェース参考
// https://github.com/firebase/firebase-arduino

class FirebaseArduino {
public:
  FirebaseArduino();
  void begin(const char* host);
  void signin(const char* mail,const char* password,const std::function<void(int,JsonObject&)> callback);
  void post(const char* url,const char* payload,const std::function<void(int,JsonObject&)> callback);
  void get(const char* url,const std::function<void(int,JsonObject&)> callback);

private:
  HTTPClient http_;
  StaticJsonBuffer<JSON_ARRAY_SIZE(200)> jsonBuffer_;

  char host_[255];
  char* auth_token;
  char result_[255];
};

//extern FirebaseArduino Firebase;

#endif //FIREBASE_ARDUINO_H_
