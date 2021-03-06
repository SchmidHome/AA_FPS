#include <Arduino.h>
#include <FPSassistant.h>

FPSassistant FPS{Serial,  //define Serial to be used
                 115200,  //define Serial speed for Serial.begin() (0 = manual begin())
                 true};   //default state of FPS timer

void setup() {
    FPS.setup(); //setup FPS timer
}

void loop() {
    FPS.loop(); //loop FPS timer
}
