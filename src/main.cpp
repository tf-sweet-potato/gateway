#include <Arduino.h>
#include "Gateway.h"

Gateway gw;

void task1(void *pvParameters){
    gw.task1(pvParameters);
}

void setup() {
    gw.setup();
    //CPU 1 ループ
    //xTaskCreatePinnedToCore(task1, "task1", 8192, NULL, 1, NULL, ARDUINO_RUNNING_CORE_1);
}

void loop() {
  gw.loop();
}
