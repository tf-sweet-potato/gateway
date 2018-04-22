#include "BeaconPayload.h"
#include <Arduino.h>

BeaconPayload::BeaconPayload(const char *name,const char* payload){
    memcpy(_name,name,sizeof(_name));
    memcpy((char*)_payload,payload,sizeof(_payload));
}

char* BeaconPayload::getName(){
  return _name;
}

Payload* BeaconPayload::getPayload(){
  return _payload;
}
