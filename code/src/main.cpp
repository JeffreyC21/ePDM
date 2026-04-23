#include <FlexCAN.h>

#include "helper.h"

#define BAUD_RATE 1000000
#define LOW_CURRENT_SHUNT 0.011
#define HIGH_CURRENT_SHUNT 0.0055
#define HSS_CURR_SENSE_GAIN 50
#define LV_BAT_CURR_SENSE_GAIN 2
#define DETECTION_THRESHOLD 0.016 // This parameter is for LV bat current sense

CAN_message_t deviceEnable;
CAN_message_t diagCurrSense;
CAN_message_t currSense;

bool is_lv_on = false;

void setup() {

  pin_init();
  is_lv_on = LV_ON();
  Can0.begin(BAUD_RATE);
  analogReadResolution(13);

  deviceEnable.id = 0x700;
  deviceEnable.len = 1;
  deviceEnable.buf[0] = 0;

  diagCurrSense.id = 0x702;
  diagCurrSense.len = 8;
  diagCurrSense.buf[0] = 0;
  diagCurrSense.buf[1] = 0;
  diagCurrSense.buf[2] = 0;
  diagCurrSense.buf[3] = 0;
  diagCurrSense.buf[4] = 0;
  diagCurrSense.buf[5] = 0;
  diagCurrSense.buf[6] = 0;
  diagCurrSense.buf[7] = 0;

  currSense.id = 0x701;
  currSense.len = 8;
  currSense.buf[0] = 0;
  currSense.buf[1] = 0;
  currSense.buf[2] = 0;
  currSense.buf[3] = 0;
  currSense.buf[4] = 0;
  currSense.buf[5] = 0;
  currSense.buf[6] = 0;
  currSense.buf[7] = 0;
}

void loop() {
  // cooling fans have 14 amp trainseint response and settles at around 6 amps

  is_ecu_faulted(is_lv_on, digitalRead(ECU_DIAG));

  // Devices enabled by ECU over CAN
  while (Can0.available()) {
    Can0.read(deviceEnable);
    // Mask 0x01 used to be for pumps but that will be switched directly by ECU due to PDM hardware shortcomings
    digitalWrite(PUMP1_ENABLE, deviceEnable.buf[0] & 0x02);
    digitalWrite(PUMP2_ENABLE, deviceEnable.buf[0] & 0x04);
    digitalWrite(SDC_ENABLE, deviceEnable.buf[0] & 0x08);
    digitalWrite(DAQ_ENABLE, deviceEnable.buf[0] & 0x10);
  }

  diagCurrSense.buf[0] = (uint8_t)(current_sense(SDC_CURR_SENSE, HSS_CURR_SENSE_GAIN, LOW_CURRENT_SHUNT) * 10);
  diagCurrSense.buf[1] = (uint8_t)(current_sense(INVERTER_CURR_SENSE, HSS_CURR_SENSE_GAIN, LOW_CURRENT_SHUNT) * 10);
  diagCurrSense.buf[2] = (uint8_t)(current_sense(ECU_CURR_SENSE, HSS_CURR_SENSE_GAIN, LOW_CURRENT_SHUNT) * 10);
  diagCurrSense.buf[3] = (uint8_t)(current_sense(SENSORS_CURR_SENSE, HSS_CURR_SENSE_GAIN, LOW_CURRENT_SHUNT) * 10);

  uint16_t lv_bat_curr = (uint16_t)(current_sense(LV_BAT_CURR_SENSE, LV_BAT_CURR_SENSE_GAIN, DETECTION_THRESHOLD) * 10);
  diagCurrSense.buf[4] = (uint8_t)(lv_bat_curr & 0x00FF);
  diagCurrSense.buf[5] = (uint8_t)(lv_bat_curr >> 16);

  Serial.println(analogRead(LV_BAT_CURR_SENSE));

  uint8_t diagnostics = 0;
  diagnostics = (digitalRead(SENSORS_DIAG) | diagnostics) << 1;
  diagnostics = (digitalRead(ECU_DIAG) | diagnostics) << 1; 
  diagnostics = (digitalRead(INVERTER_DIAG) | diagnostics) << 1; 
  diagnostics = (digitalRead(SDC_DIAG) | diagnostics) << 1; 
  diagnostics = (digitalRead(DAQ_DIAG) | diagnostics) << 1; 
  diagnostics = (digitalRead(DEVICE_TRAY_DIAG) | diagnostics) << 1; 
  diagnostics = (digitalRead(PUMP2_DIAG) | diagnostics) << 1; 
  diagnostics = (digitalRead(PUMP1_DIAG) | diagnostics); 

  currSense.buf[0] = diagnostics;
  currSense.buf[1] = (uint8_t)(current_sense(PUMP1_CURR_SENSE, HSS_CURR_SENSE_GAIN, LOW_CURRENT_SHUNT) * 10);
  currSense.buf[2] = (uint8_t)(current_sense(PUMP2_CURR_SENSE, HSS_CURR_SENSE_GAIN, LOW_CURRENT_SHUNT) * 10);
  currSense.buf[3] = (uint8_t)(current_sense(DEVICE_TRAY_CURR_SENSE, HSS_CURR_SENSE_GAIN, LOW_CURRENT_SHUNT) * 10);
  currSense.buf[4] = (uint8_t)(current_sense(DAQ_CURR_SENSE, HSS_CURR_SENSE_GAIN, LOW_CURRENT_SHUNT) * 10);
  currSense.buf[5] = is_lv_on ? 0x0F : 0;

  Can0.write(diagCurrSense);
  delay(100);
  Can0.write(currSense);
  delay(100);
}
