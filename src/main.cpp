#include <Arduino.h>
#include "Gateway.h"

Gateway gw;

void task1(void *pvParameters){
    gw.task1(pvParameters);
}

void interrupt(){
    gw.interrupt();
}

void setup() {
    gw.setup();
    //CPU 1 ループ
    //xTaskCreatePinnedToCore(task1, "task1", 8192, NULL, 1, NULL, ARDUINO_RUNNING_CORE_1);

    //IO 割り込み
    attachInterrupt(digitalPinToInterrupt(SW0), interrupt, CHANGE);
}

void loop() {
  gw.loop();
}
