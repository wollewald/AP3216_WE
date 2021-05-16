# AP3216_WE
Arduino library for the AP3216C module (CJMCU 3216). 

In order to find out how the library works please look into the examples and the list of public functions.

Wiring for Arduino can be found in AP3216_Module_Wiring.png

Further details can be found on

https://wolles-elektronikkiste.de/ap3216-cjmcu-3216 (German)

https://wolles-elektronikkiste.de/en/ap3216-cjmcu-3216-ambient-light-and-proximity-sensor (English)

Update 1.2.0 has limited compatility. In order to make the library work on ESP32 boards I needed to rename the AP3216 modes:
PS -> AP3216_PS, ALS -> AP3216_ALS, etc.

Have fun!
