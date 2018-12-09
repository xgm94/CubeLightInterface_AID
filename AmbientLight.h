#include <Adafruit_NeoPixel.h>

#define STRIPE 28
#define STRIPE_PIN 8
class AmbientLight
{
  public:
    Adafruit_NeoPixel stripe = Adafruit_NeoPixel(STRIPE  , STRIPE_PIN, NEO_GRB + NEO_KHZ800);
    typedef struct {
          byte r;
          byte g;
          byte b;
      } Components;

    typedef struct {
      Components components;
      uint32_t value;
      uint32_t secondaryValue;
    } ALColor;

    //VARIABLES
    struct {
      ALColor rojo ;
      ALColor amarillo ;
      ALColor naranja;
      ALColor blanco ;
      ALColor azulEco ;
      ALColor azul ;
      ALColor azulOscuro ;
      ALColor verde ;
      ALColor off ;
    } colors;

    enum incommingColor{
    BLANCO = 0,
    ROJO = 1,
    AMARILLO = 2,
    NARANJA = 3,
    VERDE = 4,
    AZULECO = 5,
    AZUL = 6,
    AZULOSCURO = 7,
    OFF = 8
   };
    enum lightMode {STATIC, PULSE};
    int currentLightMode = 0;
    float pulse_speed = 0.1;
    //sin vars ++++
    int dynamic_id = 0;
    int bug_index= 0;
    bool canPrint = true;
    float sine_index = 0;
    //++++++
    // change colors vars ++++
    int n_iterations2Change = 50;
    int currentIteration = 0;
    int currentColorIndex = AMARILLO;
    int newColorIndex = AMARILLO;
    ALColor currentColor;

  public:
    AmbientLight ();
    void AnimateCube(float speedIndex);
    void ChangeColors();
    void InitColors();
    void InitColor(ALColor &color, uint32_t baseColor,uint32_t secondaryColor);
    ALColor getColor(int index);
    void lightIteration();

    float lerp(const float x, const float x0, const float x1, const float y0, const float y1);
    uint32_t colorLerp(const float x, const float x0, const float x1, const uint32_t c0, const uint32_t c1);
    float constrainPosition(const float pos,int numPixels_tira);
    void animateSine(float pos, float frequency);
    uint8_t red(uint32_t c);
    uint8_t green(uint32_t c);
    uint8_t blue(uint32_t c);
};
