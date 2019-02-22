#pragma once

#include "FlexCAN.h"
#include "descriptable.hpp"

namespace CAN
{


typedef Descriptable<uint32_t> DUINT;
typedef Descriptable<float> DFLOAT;
typedef Descriptable<bool> DBOOL;
typedef Descriptable<int> DINT;

struct VehicleState
{
    DINT    vehicle_state = DINT("Vehicle State");
    DUINT ams_state = DUINT("AMS State"), HDOP = DUINT("HDOP"),
          qual_fix = DUINT("Quality of Fix"), sattelites = DUINT("No. Sattelites"),
          imd_state = DUINT("IMD State"), vehicle_mode = DUINT("Vehicle Mode"), 
          reku_brakeforce = DUINT("Reku Brakeforce"), diagnosis_inv_1 = DUINT("Diagnosis Inv 1"),
          diagnosis_inv_2 = DUINT("Diagnosis Inv 2"), diagnosis_inv_3 = DUINT("Diagnosis Inv 3"), 
          diagnosis_inv_4 = DUINT("Diagnosis Inv 4");
    DFLOAT soc = DFLOAT("SOC"), accu_vlt = DFLOAT("Voltage"), cell_vlt_low = DFLOAT("Voltage Low"),
           cell_vlt_high = DFLOAT("Voltage High"), cell_temp_low = DFLOAT("Cell Temp. Low"),
           cell_temp_high = DFLOAT("Cell Temp. High"), voltage = DFLOAT("Voltage"),
           temp_mot_high = DFLOAT("Temp. Motor High"), temp_mot_low = DFLOAT("Temp. Motor Low"),
           temp_inv_high = DFLOAT("Temp. Inverter High"), temp_inv_low = DFLOAT("Temp. Inverter Low"),
           temp_ambient = DFLOAT("Temp. Ambient"), odometer = DFLOAT("Odometer"), iso_resistance = DFLOAT("Iso. Widerstand"),
           torque_lim_pos = DFLOAT("Torque Limit Pos."), torque_lim_neg = DFLOAT("Torque Limit Neg."), power_lim = DFLOAT("Power Limit"),
           rpm_lim = DFLOAT("RPM Limit"), accel_slip = DFLOAT("Accel Slip"), brake_slip = DFLOAT("Brake Slip"),
           temp_inv_fr = DFLOAT("Temp. Inverter FR"), temp_inv_fl = DFLOAT("Temp. Inverter FL"),
           temp_inv_rr = DFLOAT("Temp. Inverter RR"), temp_inv_rl = DFLOAT("Temp. Inverter RL"),
           temp_mot_fr = DFLOAT("Temp. Mot. FR"), temp_mot_fl = DFLOAT("Temp. Mot. FL"),
           temp_mot_rr = DFLOAT("Temp. Mot. RR"), temp_mot_rl = DFLOAT("Temp. Mot. FR");
    DBOOL sc_state = DBOOL("SC State"), reku_state = DBOOL("Reku State"), config_lock = DBOOL("Config Lock"),
          f_sc = DBOOL("Fuse SC Latching"), f_inv = DBOOL("Fuse Inverter"), f_imd = DBOOL("Fuse IMD"),
          f_vcu = DBOOL("Fuse VCU"), f_ssb = DBOOL("Fuse SSB"), f_rtds = DBOOL("Fuse RTDS"), f_drs = DBOOL("Fuse DRS"), 
          f_pump_mot = DBOOL("Fuse Pump Motor"), f_fan_mot = DBOOL("Fuse Fan Motor"), f_pump_inv = DBOOL("Fuse Pump Inverter"), 
          f_fan_inv = DBOOL("Fuse Fan Inverter"), asr = DBOOL("ASR"), bsr = DBOOL("BSR"), 
          tv = DBOOL("TV"), energy_save = DBOOL("Energy Save"), drs = DBOOL("DRS"),
          can_pedals = DBOOL("CAN Fail Pedals"), can_reku = DBOOL("CAN Fail Reku"), can_gps = DBOOL("CAN Fail GPS"), can_acc = DBOOL("CAN Fail Acc"),
          can_rot = DBOOL("CAN Fail Rot"), can_mag = DBOOL("CAN Fail Mag"), can_rear = DBOOL("CAN Fail Rear"),
          can_ams = DBOOL("CAN Fail AMS"), can_em = DBOOL("CAN Fail EM"), can_pdu = DBOOL("CAN Fail PDU"),
          can_dis = DBOOL("CAN Fail DIS"), can_inv_fr = DBOOL("CAN Fail Inv FR"), can_inv_fl = DBOOL("CAN Fail Inv FL"),
          can_inv_rr = DBOOL("CAN Fail Inv RR"), can_inv_rl = DBOOL("CAN Fail Inv RL"), pedals_impl = DBOOL("Pedal Implausibility"),
          apps_impl = DBOOL("APPS Implausibility"), apps1 = DBOOL("APPS1 Fail"), apps2 = DBOOL("APPS2 Fail"), bps_f = DBOOL("BPS Front Fail"),
          bps_r = DBOOL("BPS Rear Fail"), brakeforce = DBOOL("Brakeforce Fail"), steer_angle = DBOOL("Steering Angle Implausibility"),
          acc_x = DBOOL("Acc X Implausibility"), acc_y = DBOOL("Acc Y Implausibility"), rot_z = DBOOL("Rot Z Implausibility"),
          gps_v = DBOOL("GPS Velocity Implausibility"), sc_after_bots = DBOOL("SC after BOTS"), sc_after_tsms = DBOOL("SC after TSMS"),
          sc_after_sbright = DBOOL("SC after SBright"), sc_after_bspd = DBOOL("SC after BSPD"), f_swimming_angle = DBOOL("Fuse Swimming Angle"),
          f_brakelight = DBOOL("Fuse Brakelight"), sys_ready1 = DBOOL("Sys. Ready 1"), quit_hv1 = DBOOL("Quit HV 1"),
          quit_rf1 = DBOOL("Quit RF 1"), warn_inv1 = DBOOL("Warn Inv 1"), sys_ready2 = DBOOL("Sys. Ready 2"), quit_hv2 = DBOOL("Quit HV 2"),
          quit_rf2 = DBOOL("Quit RF 2"), warn_inv2 = DBOOL("Warn Inv 2"), sys_ready3 = DBOOL("Sys. Ready 3"), quit_hv3 = DBOOL("Quit HV 3"),
          quit_rf3 = DBOOL("Quit RF 3"), warn_inv3 = DBOOL("Warn Inv 3"), sys_ready4 = DBOOL("Sys. Ready 4"), quit_hv4 = DBOOL("Quit HV 4"),
          quit_rf4 = DBOOL("Quit RF 4"), warn_inv4 = DBOOL("Warn Inv 4"); 

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