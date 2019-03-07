#include "converter.hpp"

String ams_state[8] = {"Idle",
                       "Precharging",
                       "Drive",
                       "reserved",
                       "PrechFail",
                       "Data Error",
                       "Relays Stuck",
                       "reserved"};

String vehicle_mode[6] = {"Testing",
                          "Scruti",
                          "Accel",
                          "Skidpad",
                          "AutoX",
                          "Endurace" };

String imd_state[7] = {"IMD aus",
                       "PWMfail",
                       "Normal",
                       "Unterspannung",
                       "Good",
                       "Bad",
                       "Fail"};

String vehicle_state[4] = {
    "Idle",
    "Drive",
    "Error",
    "Init"
};

String inverter_state[4] = {
    "FR", "FL", "RR", "RL"};

String fltStr(float val)
{
    return String(val, 2);
}

String boolStr(bool val)
{
    return val ? "1" : "0";
}

String boolStrX(bool val)
{
    return val ? "X" : " ";
}

String inverterStr(uint8_t val)
{
    return inverter_state[val];
}

String intStr(int val)
{
    return String(val);
}

String amsStr(uint8_t state)
{
    return ams_state[state];
}

String imdStr(uint8_t state)
{
    return imd_state[state];
}

String vehicleModeStr(uint8_t mode)
{
    return vehicle_mode[mode];
}

String vehicleStateStr(uint8_t state) {
    return vehicle_state[state];
}

String boolStrOnOff(bool val) {
    return val ? "ON" : "OFF";
}