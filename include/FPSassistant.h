#ifndef AA_FPSassistant
#define AA_FPSassistant

#include <Arduino.h>
#include <Assistant.h>
#include <Timer.h>

class FPSassistant : public Assistant {
   public:
    FPSassistant(HardwareSerial serial, uint32_t setupSerial, bool defaultState);
    void setState(bool ON);
    bool getState();
    void setInterval(unsigned long time);

   private:
    void _setup() override;
    void _loop() override;
    void _printFPS();

    HardwareSerial _serial;
    uint32_t _setupSerial;

    Timer _T, _STEP;  //Tick timer (usually 1 sec)
                      //STEP timer (to determine loop time)
    uint16_t _frames = 0;
    unsigned long _step_max;

    void _setState(bool state);
};

#endif
