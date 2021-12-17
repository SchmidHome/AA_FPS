#ifndef AA_FPSassistant
#define AA_FPSassistant

#include <Arduino.h>
#include <Assistant.h>
#include <Timer.h>

String f();
class FPSassistant : public Assistant {
   public:
    FPSassistant(HardwareSerial serial, uint32_t setupSerial = 0, bool defaultState = true, String (*tick_callback)() = &f);
    void setState(bool ON);
    bool getState();
    void setInterval(unsigned long time);

#if defined(ESP8266) || defined(ESP32)
    void ramCheckStart();
    void ramCheckEnd(String message);
#endif

   private:
    void _setup() override;
    void _loop() override;
    void _printFPS();

    uint32_t _ram;

    HardwareSerial _serial;
    uint32_t _setupSerial;

    Timer _T, _STEP;  //Tick timer (usually 1 sec)
                      //STEP timer (to determine loop time)

    uint16_t _frames[10];
    uint16_t _frames_all;

    String (*tick_callback)();

    uint8_t Tges_mode = 0;  // 0 = seconds, 1 = minutes, 2 = hours

    unsigned long _step_max;

    void _setState(bool state);
};

#endif
