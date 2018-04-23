#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>

//ユニークな 5文字 初期登録時に書き込む or 販売時に書き込む
#define GW_UID "Qb9Bh"

//Firebase 初期設定
#define FIREBASE_EMAIL "tfuru27+a1@gmail.com"
#define FIREBASE_PASSWORD "Ri7XmyY6"
#define FIREBASE_HOST "https://sweetpotatoserver.firebaseapp.com"

//BLE ビーコン 初期設定
#define BEACON_NAME_PREFIX "SP-"
#define BEACON_SCAN_TIME_SEC 30

#endif
