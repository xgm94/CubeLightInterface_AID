#include <Wire.h>


class AccelGyro
{
  public:
    const int MPU_addr=0x68;
    int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
    int16_t last_AcX = 0;
    long lastTime = 0.0f;

  public:
    AccelGyro ();
    void iterate();
    void initialize();
    float computeSpeed(long millis);
    float lerp(const float x, const float x0, const float x1, const float y0, const float y1);
};
