#include "AmbientLight.h"


AmbientLight::AmbientLight ()
{
  InitColors();
}

void AmbientLight::ChangeColors(){
    if(currentIteration < n_iterations2Change){
      currentColor.value = colorLerp(currentIteration, 0,n_iterations2Change, getColor(currentColorIndex).value, getColor(newColorIndex).value) ;
      currentColor.secondaryValue = colorLerp(currentIteration, 0,n_iterations2Change, getColor(currentColorIndex).secondaryValue, getColor(newColorIndex).secondaryValue) ;
      currentIteration ++;
    }else{
      Serial.println("change ended ");
      currentColorIndex = newColorIndex;
       currentColor.value = getColor(newColorIndex).value;
       currentColor.secondaryValue = getColor(newColorIndex).secondaryValue;
      currentIteration = 0;
    }
}

void AmbientLight::InitColors () {
  InitColor(colors.blanco,    0xFFFFFF, 0x040404);
  InitColor(colors.rojo,      0xFF7F00, 0xFF007F);
  InitColor(colors.amarillo,  0xFFFF00, 0xFF7F00);
  InitColor(colors.verde,     0x7FFF00, 0x00FF7F);
  InitColor(colors.azulEco,   0x00FFFF, 0x00FF7F);
  InitColor(colors.azul,      0x00FFFF, 0x007FFF);
  InitColor(colors.azulOscuro,0x7f00ff, 0x007fff);
  InitColor(colors.naranja,   0xFF7F00, 0xFF0000);
  InitColor(colors.off,       0x000000, 0x000000);
}

void AmbientLight::InitColor(ALColor &color, uint32_t baseColor,uint32_t secondaryColor) {
  color.value  = baseColor;
  color.secondaryValue = secondaryColor;
}
AmbientLight::ALColor AmbientLight::getColor(int index){
   switch(index){
    case ROJO:
      return colors.rojo;
    break;
    case AMARILLO:
      return colors.amarillo;
    break;
    case NARANJA:
      return colors.naranja;
    break;
    case VERDE:
      return colors.verde;
    break;
    case BLANCO:
      return colors.blanco;
    break;
    case AZULECO:
      return colors.azulEco;
    break;
    case AZULOSCURO:
      return colors.azulOscuro;
    break;
    default:
       return colors.blanco;
  }
}

float AmbientLight::lerp(const float x, const float x0, const float x1, const float y0, const float y1) {
  return y0 + (x - x0) * ((y1 - y0) / (x1 - x0));
}

uint32_t AmbientLight::colorLerp(const float x, const float x0, const float x1, const uint32_t c0, const uint32_t c1) {
  uint8_t r0 = (c0 >> 16) & 0xFF;
  uint8_t g0 = (c0 >> 8) & 0xFF;
  uint8_t b0 = c0 & 0xFF;
  uint8_t r1 = (c1 >> 16) & 0xFF;
  uint8_t g1 = (c1 >> 8) & 0xFF;
  uint8_t b1 = c1 & 0xFF;
  uint32_t r = int(lerp(x, x0, x1, r0, r1));
  uint32_t g = int(lerp(x, x0, x1, g0, g1));
  uint32_t b = int(lerp(x, x0, x1, b0, b1));
  return (r << 16) | (g << 8) | b;
}

float AmbientLight::constrainPosition(const float pos,int numPixels_tira) {
  float result = fmod(pos, numPixels_tira);
  if (result < 0.0) {
    result += numPixels_tira;
  }
  return result;
}

void AmbientLight::animateSine(float pos, float frequency) {
 float phase = 2.0*PI*(constrainPosition(pos,STRIPE));
  for (int i=0; i<STRIPE/2;++i) {
    float x = sin(2.0*PI*frequency*(i/(STRIPE*2.0))+phase);
    stripe.setPixelColor((STRIPE/2)+i, colorLerp(x, -1.0, 1.0, currentColor.value, currentColor.secondaryValue));
    stripe.setPixelColor((STRIPE/2)-i, colorLerp(x, -1.0, 1.0, currentColor.value, currentColor.secondaryValue));
  }
 stripe.show();
}

uint8_t AmbientLight::red(uint32_t c)
{
  return (uint8_t) ((c >> 16) & 0xFF);
}
uint8_t AmbientLight::green(uint32_t c)
{
  return (uint8_t) ((c >> 8) & 0xFF);
}
uint8_t AmbientLight::blue(uint32_t c)
{
  return (uint8_t) ((c >> 0) & 0xFF);
}

void AmbientLight::AnimateCube(float speedIndex){
  sine_index= sine_index <STRIPE? sine_index +0.002*speedIndex: 0;
  animateSine(sine_index,4.0);
}
