/***************************************************************************
* Example sketch for the AP3216_WE library
*
* Ambient light, proximity, ambient infrared light are measured continuously
* "Object is near" function is used. 
* 
* Attention: Recommended supply voltage is 2.4 to 3.6 Volt, max 4.5 Volt
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

 /*
  * Choose between the modes:
  * AP3216_ALS: ambient light continuous
  * AP3216_PS: proximity sensor continous
  * AP3216_ALS_PS: ambient Light and Proximity Sensor continuous (default)
  * AP3216_ALS_ONCE: ambient light single 
  * AP3216_PS_ONCE: proximity sensor single 
  * AP3216_ALS_PS_ONCE: ambient light and proximity sensor single
  * AP3216_POWER_DOWN: switch off
  * AP3216_RESET: reset
  */
  myAP3216.setMode(AP3216_ALS_PS); // Uncomment and adjust if needed 
  
   /*
   * Choose between 4 Lux Ranges: 
   * RANGE_20661 (default), RANGE_  RANGE_5162, RANGE_1291, RANGE_323
   * RANGE_x means range is from 0 to x Lux. Smaller range = higher resolution.
   */
  myAP3216.setLuxRange(RANGE_20661);
 
   /*
   * Choose between 4 gain values: 1, 2(default), 4, 8 
   * higher gain increases sensitivity as well as noise - 2 is good.
   */
  myAP3216.setPSGain(2);

  /*
   * Choose between 1(default),2,3,4 pulses of LED IR for proximity
   * more pulses increase (slightly) the max. distance
   */
  myAP3216.setNumberOfLEDPulses(1);
  
  /*
   * Choose meantime for proximity measurement: PS_MEAN_TIME_50, PS_MEAN_TIME_37_5, PS_MEAN_TIME_25, PS_MEAN_TIME_12_5
   * ==> 50, 37.5, 25 or 12.5 milliseconds; longer meantime provides less spreaded values
   */
  myAP3216.setPSMeanTime(PS_MEAN_TIME_50);
  
  /*
   * Choose lower and upper proximity threshold for "objectIsNear" function, not needed if function is not used
   */
  myAP3216.setPSThresholds(300, 800);
  
  /*
   * If you want to increase the max distance, this function helps. Choose value between 1 and 16.
   * 
   */
  myAP3216.setPSIntegrationTime(1);
  
  /*
   * the PS calibration value is substracted from the measured proximity value. Choose value between 0 and 512. 
   */
  //myAP3216.setPSCalibration(150);
  
  /*
   * Choose between INT_MODE_HYSTERESIS (default) and INT_MODE_ZONE. The "objectIsNear" function only works with this mode. 
   */
  myAP3216.setPSInterruptMode(INT_MODE_HYSTERESIS);

  /* 
   * ALS calibration is necessary when the sensor is installed behind a window. E.g. if an ALS value is 80% behind a window
   * then select 100/80 = 1.25 as calibration factor. 
   */
  //myAP3216.setALSCalibrationFactor(1.25); // uncomment if needed
  
  delay(1000); // without delay the first values will be zero
}

void loop() {
  float als = myAP3216.getAmbientLight();
  unsigned int prox = myAP3216.getProximity();
  unsigned int ir = myAP3216.getIRData(); // Ambient IR light
  bool isNear = myAP3216.objectIsNear();
  bool irIsValid = !myAP3216.irDataIsOverflowed();
  
  Serial.print("Lux: "); Serial.print(als);
  Serial.print("  Proximity: "); Serial.print(prox);
  Serial.print("  Object near? ");
  
  if(isNear){
    Serial.print("Yes");
  }
  else{
    Serial.print("No");
  }
  Serial.print("  Infrared: "); Serial.print(ir);
  Serial.print("  Valid: "); Serial.println(irIsValid); 

  delay(1000);
}
