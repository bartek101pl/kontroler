#ifndef ANALOG
#define ANALOG

#include <Arduino.h>

struct config
{
  int axcen;
  int axmax;
  int axmin;
  int aycen;
  int aymax;
  int aymin;
  int azcen;
  int azmax;
  int azmin;
};

class analog
{
private:
    uint8_t pinCodeX,pinCodeY,pinCodeZ;
    config configData;
     int blind = 18;
    const int blindZ = 28;
public:
    analog(uint8_t pinCodeX,uint8_t pinCodeY, uint8_t pinCodeZ,int blind);
    void init(config data);
    int getX();
    int getY();
    int getZ();
    int getXAxis();
    int getYAxis();
    int getZAxis();
    ~analog();
};

#endif