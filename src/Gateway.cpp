#include "Gateway.h"

class BeaconAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
public:
  BeaconAdvertisedDeviceCallbacks(Gateway* gw){
    _gw = gw;
  }

  void onResult(BLEAdvertisedDevice advertisedDevice){
    //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    // 製造者固有データ
    //| Flags  | Length | AdType | companyID | Type | Data     |
    //| ------ | ------ | ------ | --------- | ---- | -------- |
    //| 020106 | XX     | FF     | FFFF      | 0000 | 0x00...  |

    // デバイス名 8文字 SP-XXXXX の場合だけPOST対象
    if(advertisedDevice.getName().find(BEACON_NAME_PREFIX) == std::string::npos){
      return;
    }

    std::string manufacturerData = advertisedDevice.getManufacturerData();
    /*
    Serial.printf("%s size: %d ManufacturerData %s size: %d \n",
              advertisedDevice.getName().data(),
              advertisedDevice.getName().length(),
              manufacturerData.data(),
              manufacturerData.length());
    */
    _gw->addBeacons(advertisedDevice.getName().data(), manufacturerData.data());
  }

private:
  Gateway* _gw;
};

//コンストラクタ
Gateway::Gateway(){

}

void Gateway::setup(){
  Serial.begin(115200);

  //Wi-Fi 初期化
  setupWiFi();

  //Wifiが繋がったので Firebase初期設定
  std::function<void(bool)> fnc = [=](bool){
    //Wi-FiをOFFにする
    setWifiPower(false);

    //BLE初期設定
    setupBle();
  };
  setupFirebase(fnc);
}

void Gateway::loop(){
  //BLE 検索
  scanBle();

  //firebaseにビーコン情報を送信
  postBeacons();

  delay(1000);
}

void Gateway::task1(void *pvParameters){
  /*
  //Serial.println(F("loopTask1"));
  portTickType delaySec = 5*1000 / portTICK_RATE_MS;

  while(1) {
    //Serial.printf("loopTask1 coreID %d\n", xPortGetCoreID());
    vTaskDelay(delaySec);
  }
  */
}

//BLE 初期化
void Gateway::setupBle(){
  Serial.println(F("setupBle"));
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new BeaconAdvertisedDeviceCallbacks(this));
  pBLEScan->setActiveScan(true);
}

//スキャン開始 〜 停止
void Gateway::scanBle(){
  Serial.println(F("scanBle"));
  //esp_bt_controller_enable(ESP_BT_MODE_BLE);
  BLEScanResults foundDevices = pBLEScan->start(BEACON_SCAN_TIME_SEC);
  Serial.printf("Devices found: %d \n",foundDevices.getCount());
  //esp_bt_controller_enable(ESP_BT_MODE_IDLE);
}

void Gateway::setupWiFi(){
  Serial.println(F("setupWiFi"));
  WiFi.mode(WIFI_STA);
  WiFi.printDiag(Serial);
  WiFi.begin("tukuddo-guest-2.4","monotukuddo");

  Serial.print(F("Connecting to WiFi .."));
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(F("."));
  }
  Serial.println();
}

void Gateway::setWifiPower(bool isOn){
  if(isOn == true){
    //WiFi接続する
    Serial.print(F("Connecting to WiFi .."));
    //WiFi.printDiag(Serial);
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(F("."));
    }
    Serial.println();
  }
  else{
    //Wi-FiをOFFにする
    Serial.println(F("WIFI_OFF"));
    WiFi.mode(WIFI_OFF);
  }
  delay(3000);
}

//ビーコン一覧に追加
void Gateway::addBeacons(const char *name,const char* payload){
  Serial.printf("addBeacons %s %s", name, payload);
  beacons.push_back( new BeaconPayload(name,payload) );
}

void Gateway::clearBeacons(){
  Serial.println(F("clearBeacons"));
  beacons.clear();
  beacons.shrink_to_fit();
}

//firebase へ送信
void Gateway::postBeacons(){
  Serial.print(F("postBeacons "));Serial.println(beacons.size());

  if(beacons.size() == 0){
    return;
  }

  //Wi-Fi on
  setWifiPower(true);

  //firebase に送信
  for(auto b : beacons) {
    postFirebase(b);
  }
  clearBeacons();

  //Wi-Fi off
  setWifiPower(false);
}

void Gateway::postFirebase(BeaconPayload* b){
    Serial.printf("postFirebase\n %s payload %s\n", b->getName(), b->getPayload()->raw);

    //POST先URL作成
    memset(url,0,sizeof(url));
    sprintf(url,"%s/api/gw?uid=%s&be=%s",FIREBASE_HOST, GW_UID, b->getName());

    //POST するデータを作成
    memset(payload,0,sizeof(payload));
    sprintf(payload,"[");
    if(strlen(payload) != 1){
      sprintf(payload,"%s,", payload);
    }
    //製造者固有データ CompanyID 以降 を 16進数文字列に変換する
    char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*) b->getPayload()->raw, sizeof(b->getPayload()->raw));
    sprintf(payload,"%s{\"data\":\"%s\"}", payload, pHex);
    sprintf(payload,"%s]",payload);

    std::function<void(int,const JsonObject&)> callback = [=](int,const JsonObject& root){
      //TODO BeaconType によってはビーコンへ接続し、レスポンスを元に何かしらのACTIONを実効する
      //b->getPayload()->type
    };
    firebase.post(url, payload, callback);
}

void Gateway::setupFirebase(std::function<void(bool)> callback){
  Serial.println(F("setupFirebase"));

  firebase.begin(FIREBASE_HOST);
  std::function<void(int,const JsonObject&)> fnc = [=](int,const JsonObject& root){
    // uid 取得に成功
    bool result = false;
    if(root.success()){
      status.setUid(root["uid"]);
      status.setCustomToken(root["custom_token"]);
      //Serial.printf("uid %s\n",status.getUid());
      //Serial.printf("custom_token %s\n",status.getCustomToken());
      result = true;
    }
    callback(result);
  };
  firebase.signin(FIREBASE_EMAIL,FIREBASE_PASSWORD,fnc);
}
