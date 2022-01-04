#include <FPSassistant.h>

String f() { return ""; };

FPSassistant::FPSassistant(String (*tick_callback)())
    : _T(1000),
      tick_callback(tick_callback) {}

void FPSassistant::_setup() {
    setState(true);
}
void FPSassistant::_loop() {
    if (_T.isON()) {
        unsigned long d = _STEP.delta();
        _step_max = max(d, _step_max);
        if (d > 1) {
            uint16_t p = 0;
            do d = d >> 1;
            while (d && ++p < 11);
            _frames[p]++;
        }
        _frames_all++;
        _STEP.reset();
        if (_T()) _printFPS();
    }
}

void FPSassistant::_printFPS() {
    //Frames
    char z_buf[11];
    for (size_t i = 0; i < 10; i++) {
        if (_frames[i] == 0) {
            z_buf[i] = ' ';
        } else if (_frames[i] == 1) {
            z_buf[i] = '.';
        } else {
            uint16_t count = _frames[i] * 100 / _frames_all;
            if (count < 18) {
                if (count < 5) {
                    if (count < 2) {
                        z_buf[i] = '-';  // 1
                    } else {
                        z_buf[i] = '~';  // 2
                    }
                } else {
                    if (count < 10) {
                        z_buf[i] = '+';  // 5
                    } else {
                        z_buf[i] = '=';  // 10
                    }
                }
            } else {
                if (count < 50) {
                    if (count < 25) {
                        z_buf[i] = '#';  // 18
                    } else {
                        z_buf[i] = 'D';  // 25
                    }
                } else {
                    if (count < 75) {
                        z_buf[i] = 'E';  // 50
                    } else {
                        z_buf[i] = 'F';  // 75
                    }
                }
            }
        }
        _frames[i] = 0;
    }
    z_buf[10] = '\0';

    //Tmax
    char Tmaxbuff[6];
    if (_step_max < 1000) {
        sprintf(Tmaxbuff, "%3lums", _step_max);
    } else {
        sprintf(Tmaxbuff, "%4lus", _step_max / 1000);
    }
    _step_max = 0;

    //T ges
    unsigned long T = millis() / 1000;
    char Tc;
    switch (Tges_mode) {
        case 0:  //seconds
            if (T >= 59) Tges_mode = 1;
            Tc = 's';
            break;
        case 1:  // minutes
            T = T / 60;
            Tc = 'm';
            if (T >= 59) Tges_mode = 2;
            break;
        default:  // hours
            T = T / 3600;
            Tc = 'h';
            break;
    }

#if defined(ESP8266) || defined(ESP32)
        //todo Serial.printf(" RAM:%6u", ESP.getFreeHeap(),;
#endif

    char buffer[50];
    sprintf(buffer, "F:%6u%s Tmax:%s Tup:%3lu%c AC:%3u ",
            _frames_all, z_buf, Tmaxbuff, T, Tc, getAssistantCount());
    MSG(String(buffer) + tick_callback());

    _frames_all = 0;
}

void FPSassistant::setState(bool ON) {
    _T.turn(ON);
    _STEP.turn(ON);
    if (ON) {
        _frames_all = 0;
        for (size_t i = 0; i < 10; i++) _frames[i] = 0;
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
// todo fix this
// void FPSassistant::ramCheckStart() {
//     _ram = ESP.getFreeHeap();
// }

// void FPSassistant::ramCheckEnd(String message) {
//     int32_t change = (int32_t)ESP.getFreeHeap() - (int32_t)_ram;
//     if (change) {
//         Serial.print(message);
// #if defined(ESP8266) || defined(ESP32)
//         Serial.printf("%5d\n", change);
// #else
//         char buffer[7];
//         sprintf(buffer, "%5d\n", change);
//         Serial.println(buffer);
// #endif
//     }
// }
#endif
