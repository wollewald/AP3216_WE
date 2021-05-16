/*****************************************
This is a library for the AP3216 ambient light and proximity sensor module

You'll find an example which should enable you to use the library. 

You are free to use it, change it or build on it. In case you like it, it would be cool 
if you give it a star.

If you find bugs, please inform me!

Written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de

*******************************************/

#include "AP3216_WE.h"

AP3216_WE::AP3216_WE(){
    _wire = &Wire;
}

AP3216_WE::AP3216_WE(TwoWire *w){
    _wire = w;
}

void AP3216_WE::init(){
    setMode(AP3216_RESET);
    setMode(AP3216_ALS_PS);
    setLuxRange(RANGE_20661);
}

void AP3216_WE::setMode(AP3216Mode d_mode){
    deviceMode = d_mode;
    writeReg(SYSTEM_CONFIGURATION_REG, deviceMode);
}

uint8_t AP3216_WE::getIntStatus(){
    intStatus = readReg(INTERRUPT_STATUS_REG) & 0b00000011;
    return(intStatus);
}

void AP3216_WE::clearInterrupt(byte intType){
    writeReg(INTERRUPT_STATUS_REG, intType);
}

void AP3216_WE::setIntClearManner(uint8_t intClearManner){
    writeReg(INT_CLEAR_MANNER_REG, intClearManner);
}
    
uint8_t AP3216_WE::getIntClearManner(){
    uint8_t intClearManner = readReg(INT_CLEAR_MANNER_REG) & 0b00000001;
    return intClearManner;
}

uint16_t AP3216_WE::getIRData(){
    uint8_t lowByte, highByte;
    uint16_t irData = 0x0000;
    lowByte = readReg(IR_DATA_LOW_REG) & 0b00000011;
    highByte = readReg(IR_DATA_HIGH_REG);
    irData = (highByte<<2) + lowByte;
    return irData;  
}

bool AP3216_WE::irDataIsOverflowed(){
    if((readReg(IR_DATA_LOW_REG) & 0b10000000) == 0){
        return false;
    }
    else return true; 
}

float AP3216_WE::getAmbientLight(){
    uint16_t rawALS = 0;
    rawALS = getALSData();
    float als=0;
    
    switch(luxRange){
        case RANGE_20661:
            als = 0.35 * rawALS;    
            break;
        case RANGE_5162:
            als = 0.0788 * rawALS;  
            break;
        case RANGE_1291:
            als = 0.0197 * rawALS;  
            break;
        case RANGE_323:
            als = 0.0049 * rawALS;  
            break;
        default:
            als = 0.35 * rawALS;    
            break;
    }
    
    return als;
}   

uint16_t AP3216_WE::getProximity(){
    uint8_t lowByte, highByte;
    uint16_t proximity = 0x0000;
    lowByte = readReg(PS_DATA_LOW_REG) & 0b00001111;
    highByte = readReg(PS_DATA_HIGH_REG) & 0b00111111;
    proximity = (highByte<<4) + lowByte;
    return proximity;   
}

bool AP3216_WE::proximityIsValid(){
    if((readReg(PS_DATA_LOW_REG) & 0b01000000) == 0){
        return true;
    }
    else return false; 
}

bool AP3216_WE::objectIsNear(){
    if((readReg(PS_DATA_HIGH_REG)>>7) == 1){
        return true;
    }
    else return false; 
}   

void AP3216_WE::setLuxRange(AP3216LuxRange lr){
    uint8_t alsConfigReg;
    luxRange = lr;
    alsConfigReg = readReg(ALS_CONFIG_REG);
    alsConfigReg &= 0b11001111;
    alsConfigReg |= lr; 
    writeReg(ALS_CONFIG_REG, alsConfigReg); 
}

void AP3216_WE::setALSIntAfterNConversions(uint8_t conv){
    uint8_t convVal = conv;
    convVal /= 4;
    if(convVal == 0){
        convVal++;
    }
    else if (convVal >= 16){
        convVal = 16;
    }
    uint8_t alsConfigReg = readReg(ALS_CONFIG_REG);
    alsConfigReg &= 0b11110000;
    alsConfigReg |= convVal; 
    writeReg(ALS_CONFIG_REG, alsConfigReg);
}
    
void AP3216_WE::setALSCalibrationFactor(float clf){
    uint8_t calVal = round(64*clf);
    writeReg(ALS_CALIBRATION_REG, calVal);
}

void AP3216_WE::setALSThresholds(float alsLowThresInLux, float alsHighThresInLux){
    uint16_t alsLowThres, alsHighThres;
    float factor = 0;
    
    switch(luxRange){
        case RANGE_20661:
            factor = 0.35; 
            break;
        case RANGE_5162:
            factor = 0.0788 ;
            break;
        case RANGE_1291:
            factor = 0.0197;
            break;
        case RANGE_323:
            factor = 0.0049; 
            break;
        default: 
            factor = 0.35;
            break; 
    }
    
    alsLowThres = (uint16_t)(round(alsLowThresInLux / factor));
    alsHighThres = (uint16_t)(round(alsHighThresInLux / factor));
    
    setALSLowThreshold(alsLowThres);
    setALSHighThreshold(alsHighThres); 
}

void AP3216_WE::setPSIntegrationTime(uint8_t integTime){
    uint8_t psConfVal;
    if((integTime<1) || (integTime>16)){    
        integTime = 1;
    }
    integTime--;
    psConfVal = readReg(PS_CONFIGURATION_REG);
    psConfVal &= 0b00001111;
    psConfVal |= (integTime<<4);
    writeReg(PS_CONFIGURATION_REG, psConfVal);
}

void AP3216_WE::setPSGain(uint8_t gain){
    uint8_t gainVal, psConfVal;
    switch (gain){
        case 1: gainVal = 0b00000000; break;
        case 2: gainVal = 0b00000100; break; 
        case 4: gainVal = 0b00001000; break;
        case 8: gainVal = 0b00001100; break;
        default: gainVal = 0b00000000; break; // to avoid invalid values
    }
    psConfVal = readReg(PS_CONFIGURATION_REG);
    psConfVal &= 0b11110011;
    psConfVal |= gainVal;
    writeReg(PS_CONFIGURATION_REG, psConfVal);
}
    
void AP3216_WE::setPSIntAfterNConversions(uint8_t intFilt){
    uint8_t intFiltVal, psConfVal;
    switch (intFilt){
        case 1: intFiltVal = 0b00000000; break;
        case 2: intFiltVal = 0b00000001; break; 
        case 4: intFiltVal = 0b00000010; break;
        case 8: intFiltVal = 0b00000011; break;
        default: intFiltVal = 0b00000001; break; 
    }
    psConfVal = readReg(PS_CONFIGURATION_REG);
    psConfVal &= 0b11111100;
    psConfVal |= intFiltVal;
    writeReg(PS_CONFIGURATION_REG, psConfVal);
}

void AP3216_WE::setNumberOfLEDPulses(uint8_t ledPulses){
    uint8_t psLEDDriverVal;
    if(ledPulses > 3){          // safety check 
        ledPulses = 1;
    }
    psLEDDriverVal = readReg(PS_LED_DRIVER_REG);
    psLEDDriverVal &= 0b11001111;
    psLEDDriverVal |= (ledPulses<<4);
    writeReg(PS_LED_DRIVER_REG, psLEDDriverVal);
}

void AP3216_WE::setLEDCurrent(uint8_t ledCurrent){
    uint8_t psLEDDriverVal;
    if(ledCurrent > 3){
        ledCurrent = 3;
    }
    psLEDDriverVal = readReg(PS_LED_DRIVER_REG);
    psLEDDriverVal &= 0b11111100;
    psLEDDriverVal |= ledCurrent;
    writeReg(PS_LED_DRIVER_REG, psLEDDriverVal);
}

void AP3216_WE::setPSInterruptMode(uint8_t intMode){
    if(intMode > 1){
        intMode = 1;            // avoid invalid values
    }
    writeReg(PS_INT_FORM_REG, intMode);
}

void AP3216_WE::setPSMeanTime(uint8_t psMeanTime){
    if(psMeanTime > 3){         // avoid invalid values
        psMeanTime = 1;
    }
    writeReg(PS_MEAN_TIME_REG, psMeanTime);
}

void AP3216_WE::setLEDWaitingTime(uint8_t wTime){
    if(wTime > 63){         // avoid invalid values ******************************
        wTime = 0;
    }
    writeReg(PS_LED_WAITING_TIME_REG, wTime);
}

uint8_t AP3216_WE::getLEDWaitingTime(){
    uint8_t wTime = readReg(PS_LED_WAITING_TIME_REG);
    return wTime;
}

void AP3216_WE::setPSCalibration(uint16_t calibVal){
    uint8_t lowByte, highByte;
    lowByte = (calibVal & 0b00000001);
    highByte = (calibVal >> 1);
    writeReg(PS_CALIBRATION_L_REG, lowByte);
    writeReg(PS_CALIBRATION_H_REG, highByte);
}

void AP3216_WE::setPSThresholds(uint16_t lowThres, uint16_t highThres){
    setPSLowThreshold(lowThres);
    setPSHighThreshold(highThres);
}

/************************************************ 
    private functions
*************************************************/

uint16_t AP3216_WE::getALSData(){
    uint8_t lowByte, highByte;
    uint16_t als = 0x0000;
    lowByte = readReg(ALS_DATA_LOW_REG);
    highByte = readReg(ALS_DATA_HIGH_REG);
    als = (highByte<<8) + lowByte;
    return als; 
}

void AP3216_WE::setALSLowThreshold(uint16_t alsLowThres){
    uint8_t lowByte, highByte;
    lowByte = alsLowThres & 0b11111111;
    highByte = alsLowThres>>8;
    writeReg(ALS_LOW_THRESHOLD_LOW_REG, lowByte);
    writeReg(ALS_LOW_THRESHOLD_HIGH_REG, highByte);
}

void AP3216_WE::setALSHighThreshold(uint16_t alsHighThres){
    uint8_t lowByte, highByte;
    lowByte = alsHighThres & 0b11111111;
    highByte = alsHighThres>>8;
    writeReg(ALS_HIGH_THRESHOLD_LOW_REG, lowByte);
    writeReg(ALS_HIGH_THRESHOLD_HIGH_REG, highByte);
}   

void AP3216_WE::setPSLowThreshold(uint16_t lowThres){
    uint8_t lowByte, highByte;
    lowByte = (lowThres & 0b00000011);
    highByte = (lowThres >> 2);
    writeReg(PS_LOW_THRESHOLD_LOW_REG, lowByte);
    writeReg(PS_LOW_THRESHOLD_HIGH_REG, highByte);
}

void AP3216_WE::setPSHighThreshold(uint16_t highThres){
    uint8_t lowByte, highByte;
    lowByte = (highThres & 0b00000011);
    highByte = (highThres >> 2);
    writeReg(PS_HIGH_THRESHOLD_LOW_REG, lowByte);
    writeReg(PS_HIGH_THRESHOLD_HIGH_REG, highByte);
}

void AP3216_WE::writeReg(uint8_t reg, uint8_t val){
    _wire->beginTransmission(I2C_ADDR);
    _wire->write(reg);
    _wire->write(val);
    _wire->endTransmission();
}

uint8_t AP3216_WE::readReg(uint8_t reg){
    byte regVal;
    _wire->beginTransmission(I2C_ADDR);
    _wire->write(reg);
    _wire->endTransmission();
    _wire->requestFrom(I2C_ADDR, 1);
    regVal = _wire->read();
    return regVal;
}

    


