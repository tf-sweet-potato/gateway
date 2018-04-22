#ifndef STATUS_H_
#define STATUS_H_

class Status {
public:
  Status();
  void setUid(const char* src);
  char* getUid();

  void setCustomToken(const char* src);
  char* getCustomToken();

private:
  char uid[28];
  char customToken[804];
};

#endif
