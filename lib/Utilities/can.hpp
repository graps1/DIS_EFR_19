#pragma once

#include "setable.hpp"
#include "FlexCAN.h"

namespace CAN
{

struct Inverter
{
      bool sys_ready,
          quit_hv,
          quit_rf,
          warn,
          can_fail;
      float temp_inv, temp_mot;
      uint32_t diagnosis;
};

union Failure {
      uint32_t buf;
      struct
      {
            union {
                  uint16_t can_buf;
                  struct
                  {
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
                  };
            };

            union {
                  uint16_t impl_buf;
                  struct
                  {
                        char pedals : 1;
                        char apps : 1;
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
      };
};

union SC {
      uint8_t buf;
      struct
      {
            char state : 1;
            char after_bots : 1;
            char after_tsms : 1;
            char after_sb_right : 1;
            char after_bspd : 1;
      };
};

union Fuses {
      uint16_t buf;
      // 1 = funktioniert
      // 0 = broken
      struct
      {
            char sc : 1;
            char latching : 1;
            char gps : 1;
            char dis : 1;
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

typedef setable::Setable<double> SD;
typedef setable::Setable<uint8_t> SU8;
typedef setable::Setable<uint16_t> SU16;
typedef setable::Setable<bool> SB;

struct VehicleState
{
      uint32_t HDOP, qual_fix, sattelites, reku_brakeforce, ams_state, imd_state, vehicle_state;
      float soc, accu_vlt, cell_vlt_low,
          cell_vlt_high, cell_temp_low,
          cell_temp_high, voltage,
          temp_mot_high, temp_mot_low,
          temp_inv_high, temp_inv_low,
          temp_ambient, odometer, iso_resistance;
      SB    asr = SB(0, 1, 1, 0),
            bsr = SB(0, 1, 1, 0),
            tv = SB(0, 1, 1, 0),
            energy_save = SB(0, 1, 1, 0),
            drs = SB(0, 1, 1, 0);
      SU16  rpm_lim = SU16(0, 100, 20000, 0);
      SU8   power_lim = SU8(0, 1, 30, 0),
            accel_slip = SU8(0, 1, 100, 0), 
            brake_slip = SU8(0, 1, 100, 0),
            vehicle_mode = SU8(0, 1, 5, 0);
      SD    torque_lim_pos = SD(0, 0.2, 21, 0), 
            torque_lim_neg = SD(0, 0.2, 21, 0);
      bool sc_state, reku_state, config_lock;

      Inverter inverter[4]; // fr, fl, rr, rl
      Fuses fuses;
      SC sc;
      Failure fail;
};

void parseSSBFReku(CAN_message_t &receivedMsg, VehicleState *state);
void parseAmsMsg(CAN_message_t &receivedMsg, VehicleState *state);
void parseVCUControlsMsg(CAN_message_t &receivedMsg, VehicleState *state);
void parseGps(CAN_message_t &receivedMsg, VehicleState *state);
void parseEmVlt(CAN_message_t &receivedMsg, VehicleState *state);
void parseSSBR(CAN_message_t &receivedMsg, VehicleState *state);
void parseGpsOdo(CAN_message_t &receivedMsg, VehicleState *state);
void parsePDU(CAN_message_t &receivedMsg, VehicleState *state); //uint8_t *FusestoBit
void parseVCUParam(CAN_message_t &receivedMsg, VehicleState *state);
void parseVCUErrors(CAN_message_t &receivedMsg, VehicleState *state);
void parseVCUINV(CAN_message_t &receivedMsg, VehicleState *state);
void parseVCUTemps(CAN_message_t &receivedMsg, VehicleState *state);
VehicleState* getVehicleState();

void check();
void sendStatus();
void init();
bool paramChanged();
void sendParam();

} // namespace CAN