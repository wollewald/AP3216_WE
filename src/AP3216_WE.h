/*****************************************
This is a library for the AP3216 ambient light and proximity sensor module

You'll find an example which should enable you to use the library. 

You are free to use it, change it or build on it. In case you like it, it would be cool 
if you give it a star.

If you find bugs, please inform me!

Written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de

*******************************************/

#ifndef AP3216_WE_H
#define AP3216_WE_H

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <Wire.h>

#define I2C_ADDR 0x1E

/***************************************
* AP3216 Registers
***************************************/
/* System Registers */
#define SYSTEM_CONFIGURATION_REG 	0x00
#define INTERRUPT_STATUS_REG 		0x01
#define INT_CLEAR_MANNER_REG		0x02
#define IR_DATA_LOW_REG				0x0A
#define	IR_DATA_HIGH_REG			0x0B
#define ALS_DATA_LOW_REG			0x0C
#define ALS_DATA_HIGH_REG			0x0D
#define	PS_DATA_LOW_REG				0x0E
#define PS_DATA_HIGH_REG			0x0F

/* ALS Registers */

#define ALS_CONFIG_REG 				0x10
#define ALS_CALIBRATION_REG			0x19
#define	ALS_LOW_THRESHOLD_LOW_REG 	0x1A
#define ALS_LOW_THRESHOLD_HIGH_REG	0x1B
#define	ALS_HIGH_THRESHOLD_LOW_REG	0x1C
#define	ALS_HIGH_THRESHOLD_HIGH_REG	0x1D

/* PS Registers */

#define PS_CONFIGURATION_REG 		0x20
#define PS_LED_DRIVER_REG 			0x21
#define PS_INT_FORM_REG 			0x22
#define PS_MEAN_TIME_REG 			0x23
#define PS_LED_WAITING_TIME_REG 	0x24
#define PS_CALIBRATION_L_REG 		0x28
#define PS_CALIBRATION_H_REG 		0x29
#define	PS_LOW_THRESHOLD_LOW_REG 	0x2A
#define PS_LOW_THRESHOLD_HIGH_REG 	0x2B
#define PS_HIGH_THRESHOLD_LOW_REG 	0x2C
#define PS_HIGH_THRESHOLD_HIGH_REG 	0x2D

/****************************************
* Other definitions
****************************************/

#define CLR_INT_BY_DATA_READ 	0
#define CLR_INT_MANUALLY 		1
#define LED_100 				3
#define LED_66_7 				2
#define LED_33_3 				1
#define LED_16_7 				0
#define INT_MODE_ZONE			0
#define INT_MODE_HYSTERESIS		1
#define PS_MEAN_TIME_12_5		0
#define PS_MEAN_TIME_25			1
#define PS_MEAN_TIME_37_5 		2
#define PS_MEAN_TIME_50			3

enum AP3216IntStatus {
	NO_INT = 	0,
	ALS_INT,
	PS_INT,
	ALS_PS_INT
};

typedef enum AP3216Mode {
	POWER_DOWN = 	0b00000000,   //CHM: Continuously H-Resolution Mode
	ALS = 			0b00000001,		
	PS = 			0b00000010,
	ALS_PS =		0b00000011,
	RESET = 		0b00000100,
	ALS_ONCE = 		0b00000101,
	PS_ONCE =		0b00000110,
	ALS_PS_ONCE = 	0b00000111
} mode;

enum AP3216LuxRange{
	RANGE_20661	= 0b00000000,
	RANGE_5162  = 0b00010000,
	RANGE_1291	= 0b00100000,
	RANGE_323	= 0b00110000
};

class AP3216_WE{
	public:
		AP3216_WE();
		void init();
		void setMode(AP3216Mode);
		uint8_t getIntStatus();
		void clearInterrupt(uint8_t);
		void setIntClearManner(uint8_t);		// 0,1 (CLR_INT_BY_DATA_READ, CLR_INT_MANUALLY)
		uint8_t getIntClearManner();
		uint16_t getIRData();
		bool irDataIsOverflowed();
		float getAmbientLight();
		uint16_t getProximity();
		bool proximityIsValid();
		bool objectIsNear();
		void setLuxRange(AP3216LuxRange);
		void setALSIntAfterNConversions(uint8_t); 	// 1,4,8,12,16,20,....,60
		void setALSCalibrationFactor(float); 
		void setALSThresholds(float, float);	// Ensure threshold/(lux range factor) does not exceed 65535
		void setPSIntegrationTime(uint8_t); 	// 1,2,3,4,....,15,16
		void setPSGain(uint8_t); 				// 1,2,4,8
		void setPSIntAfterNConversions(uint8_t);		// 1,2,4,8
		void setNumberOfLEDPulses(uint8_t);		// 0,1,2,3 (0 makes no sense)
		void setLEDCurrent(uint8_t);			// 0,1,2,3 (LED_16_7, LED_33_3, LED_66_7, LED_100)
		void setPSInterruptMode(uint8_t);		// 0,1 (INT_MODE_ZONE, INT_MODE_HYSTERESIS)	
		void setPSMeanTime(uint8_t);			// 0,1,2,3 (PS_MEAN_TIME_12_5, PS_MEAN_TIME_25, PS_MEAN_TIME_37_5,PS_MEAN_TIME_50)
		void setLEDWaitingTime(uint8_t);		// 0 - 63
		uint8_t getLEDWaitingTime();
		void setPSCalibration(uint16_t);
		void setPSThresholds(uint16_t, uint16_t);
					
	private:
		uint16_t getALSData();
		void setALSLowThreshold(uint16_t);
		void setALSHighThreshold(uint16_t);
		void setPSLowThreshold(uint16_t);
		void setPSHighThreshold(uint16_t); 
		
		void writeReg(uint8_t, uint8_t);
		uint8_t readReg(uint8_t);	
		
		uint8_t intStatus;
		AP3216Mode deviceMode;
		AP3216LuxRange luxRange;
			
};

#endif

