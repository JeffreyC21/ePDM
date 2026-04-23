#ifndef HELPER
#define HELPER

#include <Arduino.h>

// Device enable pin assignment
#define SPARE1_ENABLE 0
#define SPARE2_ENABLE 7
#define FAN1_ENABLE 8
#define FAN1_UNUSED_ENABLE 11
#define PUMP1_ENABLE 12
#define PUMP2_ENABLE 26
#define DEVICE_TRAY_ENABLE 27
#define DAQ_ENABLE 30
#define INVERTER_ENABLE 14
#define SDC_ENABLE 37 // This is power to switch side of inverter relay. Power to inverter is switched by SDC
#define SENSORS_ENABLE 36
#define ECU_ENABLE 33

#define ECU_FAULT_INDICATOR 13

// Device diagnostics pin assignment
#define SPARE1_DIAG 1
#define SPARE2_DIAG 6
#define FAN1_DIAG 9
#define FAN1_UNUSED_DIAG 10
#define PUMP1_DIAG 24
#define PUMP2_DIAG 25
#define DEVICE_TRAY_DIAG 28
#define DAQ_DIAG 29
#define INVERTER_DIAG 39
#define SDC_DIAG 38
#define SENSORS_DIAG 35
#define ECU_DIAG 34

// Device current sense pin assignment
#define SPARE1_CURR_SENSE PIN_A8
#define SPARE2_CURR_SENSE PIN_A9
#define FAN1_CURR_SENSE PIN_A7
#define FAN1_UNUSED_CURR_SENSE PIN_A6
#define PUMP1_CURR_SENSE PIN_A4
#define PUMP2_CURR_SENSE PIN_A5
#define DEVICE_TRAY_CURR_SENSE PIN_A3
#define DAQ_CURR_SENSE PIN_A2
#define INVERTER_CURR_SENSE PIN_A21
#define SDC_CURR_SENSE PIN_A13
#define SENSORS_CURR_SENSE PIN_A1
#define ECU_CURR_SENSE PIN_A12
#define LV_BAT_CURR_SENSE PIN_A22

/**
 * @brief This function intialises the state of all pins on the teensy 3.5 
 * to what's required for 2023 ePDM.
 */
void pin_init();

/**
 *@brief This function turns on power to all LV devices that cannot be toggled
 * over CAN.
 * @return returns true if the function was run.
 */
bool LV_ON();

/**
 * @brief This function determines if the ecu output has faulted.
 */
void is_ecu_faulted(bool lv_on, int ecu_diag);

/**
 * @brief This function converts the recieved analogue input from high side 
 * switch modules into a current value
 * 
 * @param pin This is the anlogue pin that needs to be read
 * @param gain This is the gain of the amplifier used
 * @param shunt_resistance This is the equivalent resistances of the shunt resistors
 * used on the high side switch module. In the case of lv battery current sense this
 * represents the detection sensitivity threshold of the current transducer.
 * @return A floating point number that represents the current being sensed
 */
float current_sense(uint8_t pin, int gain, float shunt_resistance);

#endif