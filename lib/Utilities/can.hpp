#pragma once

#include "FlexCAN.h"
#include "descriptable.hpp"

namespace CAN
{


typedef Descriptable<uint8_t> DUINT;
typedef Descriptable<float> DFLOAT;
typedef Descriptable<bool> DBOOL;
typedef Descriptable<int> DINT;

struct VehicleState
{
    Descriptable<int> vehicle_state = Descriptable<int>("Vehicle State");
    DUINT ams_state = DUINT("AMS State"), HDOP = DUINT("HDOP"),
          qual_fix = DUINT("Quality of Fix"), sattelites = DUINT("No. Sattelites"),
          imd_state = DUINT("IMD State"), vehicle_mode = DUINT("Vehicle Mode");
    DFLOAT soc = DFLOAT("SOC"), accu_vlt = DFLOAT("Voltage"), cell_vlt_low = DFLOAT("Voltage Low"),
           cell_vlt_high = DFLOAT("Voltage High"), cell_temp_low = DFLOAT("Cell Temp. Low"),
           cell_temp_high = DFLOAT("Cell Temp. High"), voltage = DFLOAT("Voltage"),
           temp_mot_high = DFLOAT("Temp. Motor High"), temp_mot_low = DFLOAT("Temp. Motor Low"),
           temp_inv_high = DFLOAT("Temp. Inverter High"), temp_inv_low = DFLOAT("Temp. Inverter Low"),
           temp_ambient = DFLOAT("Temp. Ambient"), odometer = DFLOAT("Odometer"), iso_resistance = DFLOAT("Iso. Widerstand"),
           torque_lim_pos = DFLOAT("Torque Limit Pos."), torque_lim_neg = DFLOAT("Torque Limit Neg."), power_lim = DFLOAT("Power Limit"),
           rpm_lim = DFLOAT("RPM Limit"), accel_slip = DFLOAT("Accel Slip"), brake_slip = DFLOAT("Brake Slip"),
           diagnosis_inv_1 = DFLOAT("Diagnosis Inverter 1"), diagnosis_inv_2 = DFLOAT("Diagnosis Inverter 2"),
           diagnosis_inv_3 = DFLOAT("Diagnosis Inverter 3"), diagnosis_inv_4 = DFLOAT("Diagnosis Inverter 4"),
           temp_inv_fr = DFLOAT("Temp. Inverter FR"), temp_inv_fl = DFLOAT("Temp. Inverter FL"),
           temp_inv_rr = DFLOAT("Temp. Inverter RR"), temp_inv_rl = DFLOAT("Temp. Inverter RL");
    DBOOL sc_state = DBOOL("SC State"), reku_state = DBOOL("Reku State"), config_lock = DBOOL("Config Lock"),
          f_sc = DBOOL("Fuse SC"), f_inv = DBOOL("Fuse Inverter"), f_imd = DBOOL("Fuse IMD"),
          f_vcu = DBOOL("Fuse VCU"), f_ssbr = DBOOL("Fuse SSB R"), f_ssbf = DBOOL("Fuse SSB F"),
          f_rtds = DBOOL("Fuse RTDS"), f_drs = DBOOL("Fuse DRS"), f_pump_mot_f = DBOOL("Fuse Pump Motor F"),
          f_pump_mot_r = DBOOL("Fuse Pump Motor R"), f_fan_mot = DBOOL("Fuse Fan Motor"),
          f_pump_inv = DBOOL("Fuse Pump Inverter"), f_fan_inv = DBOOL("Fuse Fan Inverter"),
          asr = DBOOL("ASR"), bsr = DBOOL("BSR"), tv = DBOOL("TV"), energy_save = DBOOL("Energy Save"), drs = DBOOL("DRS"),
          can_pedals = DBOOL("CAN Fail Pedals"), can_reku = DBOOL("CAN Fail Reku"), can_gps = DBOOL("CAN Fail GPS"), can_acc = DBOOL("CAN Fail Acc"),
          can_rot = DBOOL("CAN Fail Rot"), can_mag = DBOOL("CAN Fail Mag"), can_rear = DBOOL("CAN Fail Rear"),
          can_ams = DBOOL("CAN Fail AMS"), can_em = DBOOL("CAN Fail EM"), can_pdu = DBOOL("CAN Fail PDU"),
          can_dis = DBOOL("CAN Fail DIS"), can_inv_fr = DBOOL("CAN Fail Inv FR"), can_inv_fl = DBOOL("CAN Fail Inv FL"),
          can_inv_rr = DBOOL("CAN Fail Inv RR"), can_inv_rl = DBOOL("CAN Fail Inv RL"), pedals_impl = DBOOL("Pedal Implausibility"),
          apps_impl = DBOOL("APPS Implausibility"), apps1 = DBOOL("APPS1 Fail"), apps2 = DBOOL("APPS2 Fail"), bps_f = DBOOL("BPS Front Fail"),
          bps_r = DBOOL("BPS Rear Fail"), brakeforce = DBOOL("Brakeforce Fail"), steer_angle = DBOOL("Steering Angle Implausibility"),
          acc_x = DBOOL("Acc X Implausibility"), acc_y = DBOOL("Acc Y Implausibility"), rot_z = DBOOL("Rot Z Implausibility"),
          gps_v = DBOOL("GPS Velocity Implausibility");

} state;

FlexCAN can = FlexCAN(1000000);

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

VehicleState interpret(CAN_message_t receivedMsg);

} // namespace CAN