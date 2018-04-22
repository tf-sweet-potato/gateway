#ifndef BEACON_PAYLOAD_H_
#define BEACON_PAYLOAD_H_

#include <stdint.h>

union Payload {
    uint8_t raw[13];
    struct {
        uint16_t companyID; //企業ID
        uint8_t type;       //ビーコンタイプ
        uint8_t data[10];   //センサーのデータetc. 10byte
    };
};

class BeaconPayload {
public:
  BeaconPayload(const char *name,const char* payload);

  char* getName();
  Payload* getPayload();

private:
  char _name[11];
  Payload _payload[13];
};

#endif
