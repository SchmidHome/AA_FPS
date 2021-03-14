#include <FPSassistant.h>

FPSassistant::FPSassistant(HardwareSerial serial, uint32_t setupSerial = 0, bool defaultState = true)
    : _serial(serial),
      _setupSerial(setupSerial),
      _T(1000) {
    _T.turn(defaultState);
}
void FPSassistant::_setup() {
    if (_setupSerial) {
        Serial.begin(_setupSerial);
        while (!Serial) delay(0);
    }
    setState(_T.isON());
}
void FPSassistant::_loop() {
    if (_T.isON()) {
        _step_max = max(_STEP.delta(), _step_max);
        _STEP.reset();
        _frames++;
        if (_T()) _printFPS();
    }
}

void FPSassistant::_printFPS() {
#if defined(ESP8266)
// #pragma message "FPSassistant compiling for ESP8266"
    Serial.printf("F:%6i K: ", _frames);
    for (uint8_t i = 0; i < 4; i++)
        Serial.print(Assistant::getAssistantKey(i));
    Serial.printf(" AC: %2u Tmax:%4lums\n", getAssistantCount(), _step_max);
#elif defined(ESP32)
// #pragma message "FPSassistant compiling for ESP32"
#else
// #pragma message "FPSassistant compiling for AVR boards"
    char buffer[26];
    sprintf(buffer, "F:%6u K: ", _frames);
    Serial.print(buffer);
    for (uint8_t i = 0; i < 4; i++)
        Serial.print(Assistant::getAssistantKey(i));
    sprintf(buffer, " AC: %2u Tmax:%4lums", getAssistantCount(), _step_max);
    Serial.println(buffer);
#endif
    _frames = 0;
    _step_max = 0;
}

void FPSassistant::setState(bool ON) {
    _T.turn(ON);
    _STEP.turn(ON);
    if (ON) {
        _frames = 0;
        _step_max = 0;
    }
}
bool FPSassistant::getState() {
    return _T.isON();
}
void FPSassistant::setInterval(unsigned long time) {
    _T.setInterval(time);
}
