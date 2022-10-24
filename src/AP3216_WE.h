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

typedef enum AP3216IntStatus : uint8_t {
    NO_INT =    0,
    ALS_INT,
    PS_INT,
    ALS_PS_INT
}ap3216_int_status;

typedef enum AP3216Mode : uint8_t{
    AP3216_POWER_DOWN =     0b00000000,   //CHM: Continuously H-Resolution Mode
    AP3216_ALS =            0b00000001,     
    AP3216_PS =             0b00000010,
    AP3216_ALS_PS =         0b00000011,
    AP3216_RESET =          0b00000100,
    AP3216_ALS_ONCE =       0b00000101,
    AP3216_PS_ONCE =        0b00000110,
    AP3216_ALS_PS_ONCE =    0b00000111
} ap3216_mode;

typedef enum AP3216LuxRange : uint8_t{
    RANGE_20661 = 0b00000000,
    RANGE_5162  = 0b00010000,
    RANGE_1291  = 0b00100000,
    RANGE_323   = 0b00110000
} ap3216_lux_range;

    /****************************************
    * Definitions
    ****************************************/
    static constexpr uint8_t I2C_ADDR               {0x1E};
    static constexpr uint8_t CLR_INT_BY_DATA_READ   {0x00};
    static constexpr uint8_t CLR_INT_MANUALLY       {0x01};
    static constexpr uint8_t LED_100                {0x03};
    static constexpr uint8_t LED_66_7               {0x02};
    static constexpr uint8_t LED_33_3               {0x01};
    static constexpr uint8_t LED_16_7               {0x00};
    static constexpr uint8_t INT_MODE_ZONE          {0x00};
    static constexpr uint8_t INT_MODE_HYSTERESIS    {0x01};
    static constexpr uint8_t PS_MEAN_TIME_12_5      {0x00};
    static constexpr uint8_t PS_MEAN_TIME_25        {0x01};
    static constexpr uint8_t PS_MEAN_TIME_37_5      {0x02};
    static constexpr uint8_t PS_MEAN_TIME_50        {0x03};

class AP3216_WE{
    public:
        /***************************************
        * AP3216 Registers
        ***************************************/
        /* System Registers */
        static constexpr uint8_t SYSTEM_CONFIGURATION_REG    {0x00};
        static constexpr uint8_t INTERRUPT_STATUS_REG        {0x01};
        static constexpr uint8_t INT_CLEAR_MANNER_REG        {0x02};
        static constexpr uint8_t IR_DATA_LOW_REG             {0x0A};
        static constexpr uint8_t IR_DATA_HIGH_REG            {0x0B};
        static constexpr uint8_t ALS_DATA_LOW_REG            {0x0C};
        static constexpr uint8_t ALS_DATA_HIGH_REG           {0x0D};
        static constexpr uint8_t PS_DATA_LOW_REG             {0x0E};
        static constexpr uint8_t PS_DATA_HIGH_REG            {0x0F};

        /* ALS Registers */

        static constexpr uint8_t ALS_CONFIG_REG              {0x10};
        static constexpr uint8_t ALS_CALIBRATION_REG         {0x19};
        static constexpr uint8_t ALS_LOW_THRESHOLD_LOW_REG   {0x1A};
        static constexpr uint8_t ALS_LOW_THRESHOLD_HIGH_REG  {0x1B};
        static constexpr uint8_t ALS_HIGH_THRESHOLD_LOW_REG  {0x1C};
        static constexpr uint8_t ALS_HIGH_THRESHOLD_HIGH_REG {0x1D};

        /* PS Registers */

        static constexpr uint8_t PS_CONFIGURATION_REG        {0x20};
        static constexpr uint8_t PS_LED_DRIVER_REG           {0x21};
        static constexpr uint8_t PS_INT_FORM_REG             {0x22};
        static constexpr uint8_t PS_MEAN_TIME_REG            {0x23};
        static constexpr uint8_t PS_LED_WAITING_TIME_REG     {0x24};
        static constexpr uint8_t PS_CALIBRATION_L_REG        {0x28};
        static constexpr uint8_t PS_CALIBRATION_H_REG        {0x29};
        static constexpr uint8_t PS_LOW_THRESHOLD_LOW_REG    {0x2A};
        static constexpr uint8_t PS_LOW_THRESHOLD_HIGH_REG   {0x2B};
        static constexpr uint8_t PS_HIGH_THRESHOLD_LOW_REG   {0x2C};
        static constexpr uint8_t PS_HIGH_THRESHOLD_HIGH_REG  {0x2D};

        /* Constructors */      
        AP3216_WE() : _wire{&Wire} {}
        AP3216_WE(TwoWire *w) : _wire{w} {}
        
        void init();
        void setMode(AP3216Mode);
        ap3216_int_status getIntStatus();
        void clearInterrupt(uint8_t);
        void setIntClearManner(uint8_t);        // 0,1 (CLR_INT_BY_DATA_READ, CLR_INT_MANUALLY)
        uint8_t getIntClearManner();
        uint16_t getIRData();
        bool irDataIsOverflowed();
        float getAmbientLight();
        uint16_t getProximity();
        bool proximityIsValid();
        bool objectIsNear();
        void setLuxRange(ap3216_lux_range);
        void setALSIntAfterNConversions(uint8_t);   // 1,4,8,12,16,20,....,60
        void setALSCalibrationFactor(float); 
        void setALSThresholds(float, float);    // Ensure threshold/(lux range factor) does not exceed 65535
        void setPSIntegrationTime(uint8_t);     // 1,2,3,4,....,15,16
        void setPSGain(uint8_t);                // 1,2,4,8
        void setPSIntAfterNConversions(uint8_t);        // 1,2,4,8
        void setNumberOfLEDPulses(uint8_t);     // 0,1,2,3 (0 makes no sense)
        void setLEDCurrent(uint8_t);            // 0,1,2,3 (LED_16_7, LED_33_3, LED_66_7, LED_100)
        void setPSInterruptMode(uint8_t);       // 0,1 (INT_MODE_ZONE, INT_MODE_HYSTERESIS) 
        void setPSMeanTime(uint8_t);            // 0,1,2,3 (PS_MEAN_TIME_12_5, PS_MEAN_TIME_25, PS_MEAN_TIME_37_5,PS_MEAN_TIME_50)
        void setLEDWaitingTime(uint8_t);        // 0 - 63
        uint8_t getLEDWaitingTime();
        void setPSCalibration(uint16_t);
        void setPSThresholds(uint16_t, uint16_t);
                    
    protected:
        uint16_t getALSData();
        void setALSLowThreshold(uint16_t);
        void setALSHighThreshold(uint16_t);
        void setPSLowThreshold(uint16_t);
        void setPSHighThreshold(uint16_t); 
        
        void writeReg(uint8_t, uint8_t);
        uint8_t readReg(uint8_t);   
        
        TwoWire *_wire;
        ap3216_int_status intStatus;
        AP3216Mode deviceMode;
        AP3216LuxRange luxRange;
            
};

#endif

