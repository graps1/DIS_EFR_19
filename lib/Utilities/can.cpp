#include "can.hpp"
#include "buttons.hpp"

namespace CAN
{

FlexCAN can = FlexCAN(1000000);
VehicleState state;
CAN_filter_t ams_f,
    VCUcontrol_f,
    Gps_f,
    EmVlt_f,
    SSBR_f,
    GpsOdo_f,
    PDU_f,
    VCUtoDIS_f,
    mask;

CAN_message_t dis,
    dis_parameter;

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
                          "Endurance"};

String imd_state[7] = {"IMD aus",
                       "PWMfail",
                       "Normal",
                       "Unterspannung",
                       "Good",
                       "Bad",
                       "Fail"};

void init()
{
    // CAN Filter setzen
    mask.id = 0xFF3;

    // Nachrichten Filter
    ams_f.id = 0x101;
    VCUcontrol_f.id = 0x51;
    Gps_f.id = 0x33;
    EmVlt_f.id = 0x112;
    SSBR_f.id = 0x132;
    GpsOdo_f.id = 0x143;
    PDU_f.id = 0x201;
    VCUtoDIS_f.id = 0x210;

    dis.id = 0x41;
    dis.len = 4;
    dis.timeout = 5;

    dis_parameter.id = 0x301;
    dis_parameter.len = 8;
    dis_parameter.timeout = 100;

    can.begin(mask);

    // CanFilter setzen (0-7)
    can.setFilter(ams_f, 0);
    can.setFilter(VCUcontrol_f, 1);
    can.setFilter(Gps_f, 2);
    can.setFilter(EmVlt_f, 3);
    can.setFilter(SSBR_f, 4);
    can.setFilter(GpsOdo_f, 5);
    can.setFilter(PDU_f, 6);
    can.setFilter(VCUtoDIS_f, 7);
}

VehicleState check()
{
    CAN_message_t receivedMsg;
    if (!can.available())
        return state;

    can.read(receivedMsg);

    switch (receivedMsg.id)
    {
    case 0x22:
        parseSSBFReku(receivedMsg, state);
        break;
    case 0x101:
        parseAmsMsg(receivedMsg, state);
        break;
    case 0x51:
        parseVCUControlsMsg(receivedMsg, state); // wurde angepasst
        break;
    case 0x33:
        parseGps(receivedMsg, state);
        break;
    case 0x112:
        parseEmVlt(receivedMsg, state);
        break;
    case 0x132:
        parseSSBR(receivedMsg, state);
        break;
    case 0x143:
        parseGpsOdo(receivedMsg, state);
        break;
    case 0x201:
        parsePDU(receivedMsg, state);
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

bool paramChanged()
{
    // todo
    return false;
}

void sendParam()
{
    // todo
}

void sendStatus()
{
    buttons::ButtonsToBit current_button_state = buttons::getCurrentState();
    dis.buf[0] = buttons::getPoti(2);
    dis.buf[1] = buttons::getPoti(1);
    dis.buf[2] = current_button_state.buf >> 1;
    dis.buf[3] = current_button_state.buf << 7;
    can.write(dis);
}

#pragma region MESSAGE_INTERPRETATION

void parseVCUINV(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.sys_ready1 = (receivedMsg.buf[0] & 0b10000000) >> 7;
    state.quit_hv1 = (receivedMsg.buf[0] & 0b01000000) >> 6;
    state.quit_rf1 = (receivedMsg.buf[0] & 0b00100000) >> 5;
    state.warn_inv1 = (receivedMsg.buf[0] & 0b00010000) >> 4;
    state.sys_ready2 = (receivedMsg.buf[0] & 0b00001000) >> 3;
    state.quit_hv2 = (receivedMsg.buf[0] & 0b00000100) >> 2;
    state.quit_rf2 = (receivedMsg.buf[0] & 0b00000010) >> 1;
    state.warn_inv2 = receivedMsg.buf[0] & 0b00000001;
    state.sys_ready3 = (receivedMsg.buf[1] & 0b10000000) >> 7;
    state.quit_hv3 = (receivedMsg.buf[1] & 0b01000000) >> 6;
    state.quit_rf3 = (receivedMsg.buf[1] & 0b00100000) >> 5;
    state.warn_inv3 = (receivedMsg.buf[1] & 0b00010000) >> 4;
    state.sys_ready4 = (receivedMsg.buf[1] & 0b00001000) >> 3;
    state.quit_hv4 = (receivedMsg.buf[1] & 0b00000100) >> 2;
    state.quit_rf4 = (receivedMsg.buf[1] & 0b00000010) >> 1;
    state.warn_inv4 = receivedMsg.buf[1] & 0b00000001;
    state.diagnosis_inv_1 = (receivedMsg.buf[2] << 4) + ((receivedMsg.buf[3] & 0b11110000) >> 4);
    state.diagnosis_inv_2 = ((receivedMsg.buf[3] & 0b00001111) << 8) + receivedMsg.buf[4];
    state.diagnosis_inv_3 = (receivedMsg.buf[5] << 4) + ((receivedMsg.buf[6] & 0b11110000) >> 4);
    state.diagnosis_inv_4 = ((receivedMsg.buf[6] & 0b00001111) << 8) + receivedMsg.buf[7];
}

void parseVCUTemps(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.temp_mot_fr = receivedMsg.buf[0] - 20;
    state.temp_mot_fl = receivedMsg.buf[1] - 20;
    state.temp_mot_rr = receivedMsg.buf[2] - 20;
    state.temp_mot_rl = receivedMsg.buf[3] - 20;
    state.temp_inv_fr = receivedMsg.buf[4] - 20;
    state.temp_inv_fl = receivedMsg.buf[5] - 20;
    state.temp_inv_rr = receivedMsg.buf[6] - 20;
    state.temp_inv_rl = receivedMsg.buf[7] - 20;
}

void parseVCUParam(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.torque_lim_pos = 0.1 * receivedMsg.buf[0];
    state.torque_lim_neg = 0.1 * receivedMsg.buf[1];
    state.power_lim = receivedMsg.buf[2];
    state.rpm_lim = (receivedMsg.buf[3] << 8) + receivedMsg.buf[4];
    state.accel_slip = receivedMsg.buf[5];
    state.brake_slip = receivedMsg.buf[6];
    state.vehicle_mode = vehicle_mode[(receivedMsg.buf[7] & 0b11100000) >> 5];
    state.asr = (receivedMsg.buf[7] & 0b00010000) >> 4;
    state.bsr = (receivedMsg.buf[7] & 0b00001000) >> 3;
    state.tv = (receivedMsg.buf[7] & 0b00000100) >> 2;
    state.energy_save = (receivedMsg.buf[7] & 0b00000010) >> 1;
    state.drs = receivedMsg.buf[7] & 0b00000001;
}

void parsePDU(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.f_fan_inv = (receivedMsg.buf[2] & 0b1) || ((receivedMsg.buf[2] & 0b10) >> 1);
    state.f_drs = (receivedMsg.buf[2] & 0b01000000) >> 6;
    state.f_fan_mot = (receivedMsg.buf[2] & 0b10000000) >> 7;
    state.f_swimming_angle = (receivedMsg.buf[1] & 0b1);
    state.f_rtds = (receivedMsg.buf[1] & 0b10000) >> 4;
    state.f_brakelight = (receivedMsg.buf[1] & 0b100000) >> 5;
    state.f_pump_mot = (receivedMsg.buf[0] & 0b1);
    state.f_ssb = (receivedMsg.buf[0] & 0b10) >> 1;
    state.f_imd = (receivedMsg.buf[0] & 0b100) >> 2;
    state.f_inv = (receivedMsg.buf[0] & 0b1000) >> 3;
    state.f_vcu = (receivedMsg.buf[0] & 0b100000) >> 5;
    state.f_sc = (receivedMsg.buf[0] & 0b1000000) >> 6;
    state.imd_state = imd_state[(receivedMsg.buf[4] & 0b11100000) >> 5];
    state.iso_resistance = (receivedMsg.buf[5] << 8) + receivedMsg.buf[6];
}

void parseSSBFReku(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.sc_after_bots = receivedMsg.buf[2] & 0b00000001;
    state.reku_brakeforce = (receivedMsg.buf[0] << 8) + receivedMsg.buf[1]; // Kalibrierung fehlt!
}

void parseSSBR(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.temp_inv_high = (receivedMsg.buf[0] << 4) + (receivedMsg.buf[1] >> 4);        // Kalibrierung fehlt!
    state.temp_inv_low = ((receivedMsg.buf[1] & 0b00001111) << 4) + receivedMsg.buf[2]; // Kalibrierung fehlt!
    // Bei den folgenden Drei werden nur boolsche Werte ausgelesen,
    // allerdings unklar welches bit jetzt wo ist, da in der CAN-Übersicht
    // das komplette Byte als "Inhalt" markiert ist.
    state.sc_after_tsms = receivedMsg.buf[3];
    state.sc_after_bspd = receivedMsg.buf[4];
    state.sc_after_sbright = receivedMsg.buf[5];
    state.temp_ambient = (receivedMsg.buf[6] << 8) + receivedMsg.buf[7]; // Kalibrierung fehlt!
}

void parseAmsMsg(CAN_message_t &receivedMsg, VehicleState &state)
{
    state.ams_state = ams_state[(receivedMsg.buf[0] & 0b11100000) >> 5];
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
    state.reku_state = (receivedMsg.buf[1] & 0b00000010) >> 1;
    state.vehicle_state = (receivedMsg.buf[1] & 0b00001100) >> 2;
    state.config_lock = (receivedMsg.buf[1] & 0b00000001);
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

#pragma endregion

} // namespace CAN