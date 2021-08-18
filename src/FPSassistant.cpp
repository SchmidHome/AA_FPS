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
#if defined(ESP8266) || defined(ESP32)
    // #pragma message "FPSassistant compiling for ESP"
    Serial.printf("F:%6i K: ", _frames);
    for (uint8_t i = 0; i < 4; i++)
        Serial.print(Assistant::getAssistantKey(i));
    Serial.printf(" RAM:%6u AC: %2u Tmax:%4lums Tges:%5lus\n", ESP.getFreeHeap(), getAssistantCount(), _step_max, millis() / 1000);

#else
    // #pragma message "FPSassistant compiling for AVR boards"
    char buffer[26];
    sprintf(buffer, "F:%6u K: ", _frames);
    Serial.print(buffer);
    for (uint8_t i = 0; i < 4; i++)
        Serial.print(Assistant::getAssistantKey(i));
    sprintf(buffer, " AC: %2u Tmax:%4lums Tges:%5lus", getAssistantCount(), _step_max, millis() / 1000);
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

#if defined(ESP8266) || defined(ESP32)
void FPSassistant::ramCheckStart() {
    _ram = ESP.getFreeHeap();
}

void FPSassistant::ramCheckEnd(String message) {
    int32_t change = (int32_t)ESP.getFreeHeap() - (int32_t)_ram;
    if (change) {
        Serial.print(message);
#if defined(ESP8266) || defined(ESP32)
        Serial.printf("%5d\n", change);
#else
        char buffer[7];
        sprintf(buffer, "%5d\n", change);
        Serial.println(buffer);
#endif
    }
}
#endif