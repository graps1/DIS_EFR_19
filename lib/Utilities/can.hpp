#pragma once

#include "FlexCAN.h"

namespace CAN
{

struct Inverter
{
      bool  sys_ready, 
            quit_hv, 
            quit_rf, 
            warn, 
            can_fail;
      float temp_inv, temp_mot;
      uint32_t diagnosis;
};


union Failure {
      uint32_t buf;
      struct {
            char can_pedals : 1; 
            char can_reku : 1; 
            char can_gps : 1;
            char can_acc : 1;
            char can_rot : 1;
            char can_mag : 1;
            char can_rear : 1;
            char can_ams : 1;
            char can_em : 1;
            char can_pdu : 1;
            char can_dis : 1;
            char impl_pedals : 1;
            char impl_apps : 1;
            char apps1 : 1;
            char apps2 : 1;
            char bps_f : 1;
            char bps_r : 1;
            char brakeforce : 1;
            char steer_angle : 1;
            char acc_x : 1;
            char acc_y : 1;
            char rot_z : 1;
            char gps_v : 1;
      };
};

union SC {
      uint8_t buf;
      struct {
            char state : 1;
            char after_bots : 1;
            char after_tsms : 1;
            char after_sb_right : 1;
            char after_bspd : 1;
      };
};

union Fuses
{
      uint16_t buf;
      struct {
            char sc : 1;
            char inv : 1;
            char imd : 1;
            char vcu : 1;
            char ssb : 1;
            char rtds : 1;
            char drs : 1;
            char pump_mot : 1;
            char fan_mot : 1;
            char pump_inv : 1;
            char fan_inv : 1; 
            char swimming_angle : 1;
            char brakelight : 1;
      };
};



struct VehicleState
{
      String vehicle_state, ams_state, imd_state;
      uint32_t HDOP, qual_fix, sattelites,
          vehicle_mode, reku_brakeforce;
      float soc, accu_vlt, cell_vlt_low,
          cell_vlt_high, cell_temp_low,
          cell_temp_high, voltage,
          temp_mot_high, temp_mot_low,
          temp_inv_high, temp_inv_low,
          temp_ambient, odometer, iso_resistance,
          torque_lim_pos, torque_lim_neg, power_lim,
          rpm_lim, accel_slip, brake_slip;
      bool sc_state, reku_state, config_lock,
          asr, bsr, tv, energy_save, drs;
      Inverter inverter[4]; // fr, fl, rr, rl
      Fuses fuses;
      SC sc;
      Failure fail;
};

void parseSSBFReku(CAN_message_t &receivedMsg, VehicleState &state);
void parseAmsMsg(CAN_message_t &receivedMsg, VehicleState &state);
void parseVCUControlsMsg(CAN_message_t &receivedMsg, VehicleState &state);
void parseGps(CAN_message_t &receivedMsg, VehicleState &state);
void parseEmVlt(CAN_message_t &receivedMsg, VehicleState &state);
void parseSSBR(CAN_message_t &receivedMsg, VehicleState &state);
void parseGpsOdo(CAN_message_t &receivedMsg, VehicleState &state);
void parsePDU(CAN_message_t &receivedMsg, VehicleState &state); //uint8_t *FusestoBit
void parseVCUParam(CAN_message_t &receivedMsg, VehicleState &state);
void parseVCUErrors(CAN_message_t &receivedMsg, VehicleState &state);
void parseVCUINV(CAN_message_t &receivedMsg, VehicleState &state);
void parseVCUTemps(CAN_message_t &receivedMsg, VehicleState &state);

VehicleState check();
void sendStatus();
void init();
bool paramChanged();
void sendParam();

} // namespace CAN