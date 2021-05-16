/***************************************************************************
* Example sketch for the AP3216_WE library
*
* Ambient light and proximity are measured "single shot"
* Sensor is powered down after each measurement
* 
* Attention: Recommended supply voltage is 2.4 to 3.6 Volt, max 4.5 Volt
* 
* The AP3216_Continuous sketch describes settings more in detail
*
* Further information can be found on:
* https://wolles-elektronikkiste.de/en/ap3216-cjmcu-3216-ambient-light-and-proximity-sensor
* 
***************************************************************************/
#include <Wire.h>
#include <AP3216_WE.h>

AP3216_WE myAP3216 = AP3216_WE();
// You can also pass a TwoWire object such as wire2:
// AP3216_WE myAP3216 = AP3216_WE(&wire2);

void setup() {
  Serial.begin(9600);
  Wire.begin();
	myAP3216.init();
  myAP3216.setLuxRange(RANGE_20661);
  myAP3216.setMode(AP3216_ALS_PS_ONCE);
  delay(1000);
}

void loop() {
  float als = myAP3216.getAmbientLight();
  unsigned int prox = myAP3216.getProximity();
  Serial.print("Lux: "); Serial.print(als);
  Serial.print("  Proximity: "); Serial.println(prox);
  myAP3216.setMode(AP3216_ALS_PS_ONCE); // initiates next measurement
  delay(1000); 
}
