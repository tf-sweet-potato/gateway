#include "FirebaseArduino.h"

#define ARRAY_SIZE 200

//コンストラクタ
FirebaseArduino::FirebaseArduino(){

}

void FirebaseArduino::begin(const char* host){
  sprintf(host_,"%s",host);
}

/* cURLサンプル
curl -H 'Content-Type:application/json'
    -d '{"email":"tfuru27+a1@gmail.com","password":"Ri7XmyY6"}'
    https://sweetpotatoserver.firebaseapp.com/api/signin
*/
void FirebaseArduino::signin(const char* email,const char* password,const std::function<void(int,JsonObject&)> callback){
  char url[255];
  sprintf(url,"%s/api/signin",host_);
  char payload[255];
  sprintf(payload,"{\"email\":\"%s\",\"password\":\"%s\"}",email,password);

  Serial.print(F("url "));Serial.println(url);
  Serial.print(F("payload "));Serial.println(payload);

  http_.begin(url);
  http_.addHeader("Content-Type", "application/json");
  int httpCode = http_.POST(payload);

  memset(result_,0,sizeof(result_));
  sprintf(result_,"%s",http_.getString().c_str());

  //Serial.print("httpCode ");Serial.println(httpCode);
  //Serial.print("result ");Serial.println(result_);

  jsonBuffer_.clear();
  JsonObject& root = jsonBuffer_.parseObject(result_);
  http_.end();

  callback(httpCode,root);
}

/* cURLサンプル
curl -H 'Content-Type:application/json'
    -d '{"data":"ABCDEF"}'
    https://sweetpotatoserver.firebaseapp.com/api/device?uid=XXXXX
*/
void FirebaseArduino::post(const char* url,const char* payload,const std::function<void(int,JsonObject&)> callback){
  Serial.print(F("url "));Serial.println(url);
  Serial.print(F("payload "));Serial.println(payload);

  http_.begin(url);
  http_.addHeader("Content-Type", "application/json");
  int httpCode = http_.POST(payload);

  memset(result_,0,sizeof(result_));
  sprintf(result_,"%s",http_.getString().c_str());

  //Serial.print("httpCode ");Serial.println(httpCode);
  //Serial.print("result ");Serial.println(result_);

  jsonBuffer_.clear();
  JsonObject& root = jsonBuffer_.parseObject(result_);
  http_.end();

  callback(httpCode,root);
}

  void FirebaseArduino::get(const char* url,const std::function<void(int,JsonObject&)> callback){
    //Serial.print("url ");Serial.println(url);
    //Serial.print("payload ");Serial.println(payload);
    http_.begin(url);
    http_.addHeader("Content-Type", "application/json");
    int httpCode = http_.GET();

    memset(result_,0,sizeof(result_));
    sprintf(result_,"%s",http_.getString().c_str());

    //Serial.print("httpCode ");Serial.println(httpCode);
    //Serial.print("result ");Serial.println(result_);
    jsonBuffer_.clear();
    JsonObject& root = jsonBuffer_.parseObject(result_);
    http_.end();

    callback(httpCode,root);
  }
