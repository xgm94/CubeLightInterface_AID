
#include "AmbientLight.h"
#include "AccelGyro.h"
#include <eHealth.h> 
AmbientLight al;
AccelGyro gy;
int currentState = -1; // -1 = reseted, non data
                      //  0 initialized
                      // [1..10 means graded
float initial_GSR = 0.0f;
float current_GSR = 0.0f;
const float steps_GSR = 4.0f;
int n_light_steps = 7;
  
const int speedThreshold = 100;
float speedIndex = 1;

int speedState = 0; // 0 = slow 1= mid 2 = high 4 = veryHigh 5= Shake!
const int resetState = 4;
const int nResetReps = 6;
int nCurrentResetReps =0;

int conductanceVol = 0;
int nConductanceMesures = 20;
int conductanceMesures [20];
int nCurrentConductanceMesure = 0;
float meanConductanceMesure = 0;

void setup() {
  gy.initialize();
  al.currentColor = al.colors.rojo;
  al.stripe.begin();
  Serial.begin(9600);
  Serial.setTimeout(20);
  Serial.println("****************");
  Serial.println("step: "+(String) steps_GSR);
  Serial.println("light steps: " + (String)n_light_steps);
}

void loop() {
  int color = 0;
  gy.iterate();
  mapAccel2Speed();  
  conductanceVol = analogRead(A0);
  conductanceMesures[nCurrentConductanceMesure] = conductanceVol;
  if(nCurrentConductanceMesure == nConductanceMesures -1){
    int sum = 0;
    for(int i = 0; i < nConductanceMesures; i ++){
      sum += conductanceMesures[i];
    }
    meanConductanceMesure = float(sum)/nConductanceMesures;
  }
  nCurrentConductanceMesure = nCurrentConductanceMesure < nConductanceMesures -1? nCurrentConductanceMesure +1: 0;
  
  manageStates();         

 changeStateFeedback(currentState);
 al.AnimateCube(speedIndex);

}
void mapAccel2Speed(){
   int speedy = (int)gy.computeSpeed(millis());
   speedState = speedy/speedThreshold;  
    checkReset(); 
      
  if(millis()%200 <10){
    Serial.print(speedy);
    Serial.print("\t");
    Serial.print(speedIndex);
    Serial.print("\t");
    Serial.print(speedState);
    Serial.print("\tConductance Mean : ");       
    Serial.println(meanConductanceMesure);
  }
   speedIndex = gy.lerp(speedy,0,200,1,10);  
}

void manageStates(){
  // fake reset
  float incom = meanConductanceMesure;

  if(currentState == -1){
    initial_GSR = current_GSR  = incom;
    currentState = 0;
      Serial.println("****************");
      Serial.println("initializeing");
      Serial.println("initial GSR: " + (String)initial_GSR);
      Serial.println("currentState: " +(String) currentState);
  }else{
      float delta = incom - current_GSR;
      float increment = delta / (float)steps_GSR;
      increment = (int) increment;

      
      if(abs(increment) >= 1){
        Serial.println("****************");
      Serial.println("updating");
      Serial.println("delta: " +(String) delta);
      Serial.println("increment: " +(String) increment);
        Serial.println("increment grather than 1 ");
        current_GSR = incom;
        for(int i = 0; i <abs( increment); i ++){
          if( increment > 0){
            currentState += currentState < n_light_steps -1? 1:0;
          }else if( increment < 0){
             currentState += currentState >1? -1:0;
          }
        }
        Serial.println("currentState: " +(String) currentState);
      }
    }
  }

void checkReset(){
  if(speedState > resetState){
    if(nCurrentResetReps < nResetReps){
      nCurrentResetReps ++;
    }else{
      nCurrentResetReps = 0;
      currentState = -1;
    }
  }
}

void changeStateFeedback(int state){
  al.newColorIndex = currentState;
  if(al.newColorIndex != al.currentColorIndex){
    al.ChangeColors();
  }
}
