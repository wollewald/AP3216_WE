/***************************************************************************
* Example sketch for the AP3216_WE library
*
* Ambient light and proximity are measured continuously 
* Interrupts for ambient light and proximity are defined.  
* 
* The AP3216_Continuous sketch describes settings more in detail
* 
* 
* Attention: Recommended supply voltage is 2.4 to 3.6 Volt, max 4.5 Volt
*
* Further information can be found on:
* https://wolles-elektronikkiste.de/en/ap3216-cjmcu-3216-ambient-light-and-proximity-sensor
* 
***************************************************************************/
#include <Wire.h>
#include <AP3216_WE.h>
byte interruptPin=2;
byte ledPin=10;
volatile bool event = false;

AP3216_WE myAP3216 = AP3216_WE();
// You can also pass a TwoWire object such as wire2:
// AP3216_WE myAP3216 = AP3216_WE(&wire2);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, FALLING);
  Serial.begin(9600);
  Wire.begin();
	myAP3216.init();
  myAP3216.setLuxRange(RANGE_20661);
  
  /*
   * Interrupts can be cleared manually if CLR_INT_MANUALLY (or 1) is chosen
   * Interrupts will be cleared by reading PS or ALS data if CLR_INT_BY_DATA_READ (or 0) is chosen
   */
  myAP3216.setIntClearManner(CLR_INT_MANUALLY);
  myAP3216.setPSThresholds(0, 200); // Outside this window an interrupt is triggered

  /*
   * Outside the following window (lux values) an interrupt is triggered. Ensure that 
   * you don't chose values outside the defined lux range (setLuxRange).
   */
  myAP3216.setALSThresholds(3, 500); // 
  myAP3216.setPSIntegrationTime(8);
  myAP3216.setPSInterruptMode(INT_MODE_ZONE);
  delay(1000);
}

void loop() {
/*
 * Uncomment the following lines if you want to see the measured values
 */
//  float als = myAP3216.getAmbientLight();
//  unsigned int prox = myAP3216.getProximity();
//  Serial.print("Lux: "); Serial.print(als);
//  Serial.print("  Proximity: "); Serial.println(prox);
  if(event){
    interruptAction();
  }
  /*
   * without the following delay you will not detect ALS and PS interrupts together. 
   */
  delay(1000); 
}

void interruptAction(){
  byte intType = NO_INT;
  intType = myAP3216.getIntStatus();
  
  switch(intType){
    case(ALS_INT):
      Serial.println("Ambient Light Interrupt!");
      break;
    case(PS_INT):
      Serial.println("Proximity Interrupt!");
      break;
    case(ALS_PS_INT):
      Serial.println("Ambient Light and Proximity Interrupt!");
      break;
    default:
      Serial.println("Something went wrong...");
      break;      
  }
  
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  
  intType = myAP3216.getIntStatus();
  myAP3216.clearInterrupt(intType);
  event = false;
}

void blink(){
  event = true;
}
