#include "Status.h"
#include <string.h>

Status::Status():uid(""),customToken(""){

}

void Status::setUid(const char* src){
  strcpy(uid, src);
}

char* Status::getUid(){
  return uid;
}

void Status::setCustomToken(const char* src){
  strcpy(customToken, src);
}

char* Status::getCustomToken(){
  return customToken;
}
