#include "helper.h"

#include <Arduino.h>

void pin_init() {

    pinMode(SPARE1_ENABLE, OUTPUT);
    pinMode(SPARE2_ENABLE, OUTPUT);
    pinMode(FAN1_ENABLE, OUTPUT); 
    pinMode(FAN1_UNUSED_ENABLE, OUTPUT);
    pinMode(PUMP1_ENABLE, OUTPUT);
    pinMode(PUMP2_ENABLE, OUTPUT);
    pinMode(DEVICE_TRAY_ENABLE, OUTPUT);
    pinMode(DAQ_ENABLE, OUTPUT);
    pinMode(SDC_ENABLE, OUTPUT);
    pinMode(INVERTER_ENABLE, OUTPUT);
    pinMode(ECU_ENABLE, OUTPUT);
    pinMode(SENSORS_ENABLE, OUTPUT);
    pinMode(ECU_FAULT_INDICATOR, OUTPUT);

    pinMode(SPARE1_DIAG, INPUT);
    pinMode(SPARE2_DIAG, INPUT);
    pinMode(FAN1_DIAG, INPUT);
    pinMode(FAN1_UNUSED_DIAG, INPUT);
    pinMode(PUMP1_DIAG , INPUT);
    pinMode(PUMP2_DIAG , INPUT);
    pinMode(DEVICE_TRAY_DIAG, INPUT);
    pinMode(DAQ_DIAG, INPUT);
    pinMode(SDC_DIAG, INPUT);
    pinMode(INVERTER_DIAG, INPUT);
    pinMode(ECU_DIAG, INPUT);
    pinMode(SENSORS_DIAG, INPUT);

    digitalWrite(SPARE1_DIAG, LOW);
    digitalWrite(SPARE2_DIAG, LOW);
    digitalWrite(FAN1_DIAG, LOW);
    digitalWrite(FAN1_UNUSED_DIAG, LOW);
    digitalWrite(PUMP1_DIAG , LOW);
    digitalWrite(PUMP2_DIAG , LOW);
    digitalWrite(DEVICE_TRAY_DIAG, LOW);
    digitalWrite(DAQ_DIAG, LOW);
    digitalWrite(SDC_DIAG, LOW);
    digitalWrite(INVERTER_DIAG, LOW);
    digitalWrite(ECU_DIAG, LOW);
    digitalWrite(SENSORS_DIAG, LOW);
}

bool LV_ON() {
    digitalWrite(DEVICE_TRAY_ENABLE, HIGH);
    digitalWrite(SENSORS_ENABLE, HIGH);
    
    /**
     * This is power to switch side of inverter power realy.
     * i.e. the inverter will not recieve power if this output is on but sdc is open.
     * The inverter only recieves this power when sdc is closed.
     */
    digitalWrite(INVERTER_ENABLE, HIGH);
    
    delay(1000);
    digitalWrite(ECU_ENABLE, HIGH);
    return true;
}

void is_ecu_faulted(bool lv_on, int ecu_diag) {
    if (lv_on == true && ecu_diag == HIGH) {
        digitalWrite(ECU_FAULT_INDICATOR, HIGH);
    } else {
        digitalWrite(ECU_FAULT_INDICATOR, LOW);
    }
}

float current_sense(uint8_t pin, int gain, float shunt_resistance) {
    float adc_resolution = 3.3/8191;
    float input_voltage = analogRead(pin) * adc_resolution;
    float conversion_factor = gain * shunt_resistance;
    return (input_voltage/conversion_factor);
}