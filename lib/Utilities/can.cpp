#include "can.hpp"

namespace CAN
{

VehicleState interpret(CAN_message_t receivedMsg)
{
    if (!can.available())
        return state;

    can.read(receivedMsg);

    switch (receivedMsg.id)
    {
    case 0x101:
        parseAmsMsg(receivedMsg, state);
        break;
    case 0x51:
        parseVCUControlsMsg(receivedMsg, state);
        break;
    case 0x33:
        parseGps(receivedMsg, state);
        break;
    case 0x112:
        parseEmVlt(receivedMsg, state);
        break;
    case 0x132:
        parseSSBR(receivedMsg, state); // not yet implemented
        break;
    case 0x143:
        parseGpsOdo(receivedMsg, state);
        break;
    case 0x201:
        parsePDU(receivedMsg, state); // not yet implemented
        break;
    case 0x210:
        parseVCUParam(receivedMsg, state);
        break;
    case 0x214:
        parseVCUErrors(receivedMsg, state);
        break;
    case 0x218:
        parseVCUINV(receivedMsg, state);
        break;
    case 0x21C:
        parseVCUTemps(receivedMsg, state);
        break;
    default:
        break;
    }

    return state;
}

void parseAmsMsg(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.ams_state = (receivedMsg.buf[0] & 0b11100000) >> 5;
    state.sc_state = (receivedMsg.buf[0] & 0b00010000) >> 4;
    state.accu_vlt = 0.2 * (((receivedMsg.buf[0] & 0b00001111) << 8) + (receivedMsg.buf[1]));
    state.soc = 0.392157 * (receivedMsg.buf[2]);
    state.cell_vlt_high = 0.001 * ((receivedMsg.buf[3] << 4) + ((receivedMsg.buf[4] & 0b11110000) >> 4)) + 1;
    state.cell_vlt_low = 0.001 * (((receivedMsg.buf[4] & 0b00001111) << 8) + receivedMsg.buf[5]) + 1;
    state.cell_temp_high = 0.5 * receivedMsg.buf[6];
    state.cell_temp_low = 0.5 * receivedMsg.buf[7];
}

void parseVCUControlsMsg(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.reku_state = (receivedMsg.buf[1] & 0b00100000) >> 5;
    state.vehicle_state = (receivedMsg.buf[1] & 0b11000000) >> 6;
    state.config_lock = (receivedMsg.buf[1] & 0b00010000) >> 4;
}

void parseGps(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.HDOP = receivedMsg.buf[4];
    state.qual_fix = (receivedMsg.buf[5] & 0b11110000) >> 4;
    state.sattelites = receivedMsg.buf[5] & 0b00001111;
}

void parseEmVlt(CAN_message_t &receivedMsg, VehicleState &state)
{
    int32_t voltageBuffer = (receivedMsg.buf[2] << 24) + (receivedMsg.buf[3] << 16) + (receivedMsg.buf[4] << 8) + receivedMsg.buf[5];
    state.voltage = voltageBuffer / 1000;
}

void parseGpsOdo(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.odometer = 0.1 * (receivedMsg.buf[0] + receivedMsg.buf[1]);
}

void parseVCUParam(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.torque_lim_pos = 0.1 * receivedMsg.buf[0];
    state.torque_lim_neg = 0.1 * receivedMsg.buf[1];
    state.power_lim = receivedMsg.buf[2];
    state.rpm_lim = (receivedMsg.buf[3] << 8) + receivedMsg.buf[4];
    state.accel_slip = receivedMsg.buf[5];
    state.brake_slip = receivedMsg.buf[6];
    state.vehicle_mode = (receivedMsg.buf[7] & 0b11100000) >> 5;
    state.asr = (receivedMsg.buf[7] & 0b00010000) >> 4;
    state.bsr = (receivedMsg.buf[7] & 0b00001000) >> 3;
    state.tv = (receivedMsg.buf[7] & 0b00000100) >> 2;
    state.energy_save = (receivedMsg.buf[7] & 0b00000010) >> 1;
    state.drs = receivedMsg.buf[7] & 0b00000001;
}

void parseVCUErrors(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.can_pedals = (receivedMsg.buf[0] & 0b10000000) >> 7;
    state.can_reku = (receivedMsg.buf[0] & 0b01000000) >> 6;
    state.can_gps = (receivedMsg.buf[0] & 0b00100000) >> 5;
    state.can_acc = (receivedMsg.buf[0] & 0b00010000) >> 4;
    state.can_rot = (receivedMsg.buf[0] & 0b00001000) >> 3;
    state.can_mag = (receivedMsg.buf[0] & 0b00000100) >> 2;
    state.can_rear = (receivedMsg.buf[0] & 0b00000010) >> 1;
    state.can_ams = receivedMsg.buf[0] & 0b00000001;
    state.can_em = (receivedMsg.buf[1] & 0b10000000) >> 7;
    state.can_pdu = (receivedMsg.buf[1] & 0b01000000) >> 6;
    state.can_dis = (receivedMsg.buf[1] & 0b00100000) >> 5;
    state.can_inv_fr = (receivedMsg.buf[1] & 0b00010000) >> 4;
    state.can_inv_fl = (receivedMsg.buf[1] & 0b00001000) >> 3;
    state.can_inv_rr = (receivedMsg.buf[1] & 0b00000100) >> 2;
    state.can_inv_rl = (receivedMsg.buf[1] & 0b00000010) >> 1;
    state.pedals_impl = (receivedMsg.buf[2] & 0b10000000) >> 7;
    state.apps_impl = (receivedMsg.buf[2] & 0b01000000) >> 6;
    state.apps1 = (receivedMsg.buf[2] & 0b00100000) >> 5;
    state.apps2 = (receivedMsg.buf[2] & 0b00010000) >> 4;
    state.bps_f = (receivedMsg.buf[2] & 0b00001000) >> 3;
    state.bps_r = (receivedMsg.buf[2] & 0b00000100) >> 2;
    state.brakeforce = (receivedMsg.buf[2] & 0b00000010) >> 1;
    state.steer_angle = receivedMsg.buf[2] & 0b00000001;
    state.acc_x = (receivedMsg.buf[3] & 0b10000000) >> 7;
    state.acc_y = (receivedMsg.buf[3] & 0b01000000) >> 6;
    state.rot_z = (receivedMsg.buf[3] & 0b00100000) >> 5;
    state.gps_v = (receivedMsg.buf[3] & 0b00010000) >> 4;
}

} // namespace CAN