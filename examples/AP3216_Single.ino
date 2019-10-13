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
* If you understand German, further information can be found on:
* http://wolles-elektronikkiste.de/
* 
***************************************************************************/
#include <Wire.h>
#include <AP3216_WE.h>

AP3216_WE myAP3216 = AP3216_WE();

void setup() {
  Serial.begin(9600);
  Wire.begin();
	myAP3216.init();
  myAP3216.setLuxRange(RANGE_20661);
  myAP3216.setMode(ALS_PS_ONCE);
  delay(1000);
}

void loop() {
  float als = myAP3216.getAmbientLight();
  unsigned int prox = myAP3216.getProximity();
  Serial.print("Lux: "); Serial.print(als);
  Serial.print("  Proximity: "); Serial.println(prox);
  myAP3216.setMode(ALS_PS_ONCE); // initiates next measurement
  delay(1000); 
}
