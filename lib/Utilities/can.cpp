#include "can.hpp"
#include "buttons.hpp"

namespace CAN
{

FlexCAN can = FlexCAN(1000000);
VehicleState* state;
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
    state = new VehicleState();
}


void check()
{
    CAN_message_t receivedMsg;
    if (!can.available())
        return;

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
}

bool paramChanged()
{
    return  !state->torque_lim_neg.upToDate() ||
            !state->torque_lim_pos.upToDate() ||
            !state->power_lim.upToDate() ||
            !state->rpm_lim.upToDate() ||
            !state->accel_slip.upToDate()||
            !state->brake_slip.upToDate();
}

void sendParam()
{
    dis_parameter.buf[0] = (uint8_t) (10.0*state->torque_lim_pos.getNewVal());
    dis_parameter.buf[1] = (uint8_t) (10.0*state->torque_lim_neg.getNewVal());
    dis_parameter.buf[2] = state->power_lim.getNewVal();
    dis_parameter.buf[3] = state->rpm_lim.getNewVal() >> 8;
    dis_parameter.buf[4] = state->rpm_lim.getNewVal();
    dis_parameter.buf[5] = state->accel_slip.getNewVal();
    dis_parameter.buf[6] = state->brake_slip.getNewVal();
    dis_parameter.buf[7] =    (state->vehicle_mode.getNewVal() << 5)
                            + (state->asr.getNewVal() << 4)
                            + (state->bsr.getNewVal() << 3) 
                            + (state->tv.getNewVal() << 2) 
                            + (state->energy_save.getNewVal() << 1) 
                            + state->drs.getNewVal();
    can.write(dis_parameter);
}

void sendStatus()
{
    buttons::ButtonsToBitStamped current_button_state;
    buttons::getButtonStates(&current_button_state);
    dis.buf[0] = buttons::getPoti(2);
    dis.buf[1] = buttons::getPoti(1);
    dis.buf[2] = current_button_state.btns.buf >> 1;
    dis.buf[3] = current_button_state.btns.buf << 7;
    can.write(dis);
}

#pragma region MESSAGE_INTERPRETATION

VehicleState* getVehicleState() {
    return state;
}

void parseVCUINV(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->inverter[0].sys_ready = (receivedMsg.buf[0] & 0b10000000) >> 7;
    state->inverter[0].quit_hv = (receivedMsg.buf[0] & 0b01000000) >> 6;
    state->inverter[0].quit_rf = (receivedMsg.buf[0] & 0b00100000) >> 5;
    state->inverter[0].warn = (receivedMsg.buf[0] & 0b00010000) >> 4;
    
    state->inverter[1].sys_ready = (receivedMsg.buf[0] & 0b00001000) >> 3;
    state->inverter[1].quit_hv = (receivedMsg.buf[0] & 0b00000100) >> 2;
    state->inverter[1].quit_rf = (receivedMsg.buf[0] & 0b00000010) >> 1;
    state->inverter[1].warn = receivedMsg.buf[0] & 0b00000001;
    
    state->inverter[2].sys_ready = (receivedMsg.buf[1] & 0b10000000) >> 7;
    state->inverter[2].quit_hv = (receivedMsg.buf[1] & 0b01000000) >> 6;
    state->inverter[2].quit_rf = (receivedMsg.buf[1] & 0b00100000) >> 5;
    state->inverter[2].warn = (receivedMsg.buf[1] & 0b00010000) >> 4;
    
    state->inverter[3].sys_ready = (receivedMsg.buf[1] & 0b00001000) >> 3;
    state->inverter[3].quit_hv = (receivedMsg.buf[1] & 0b00000100) >> 2;
    state->inverter[3].quit_rf = (receivedMsg.buf[1] & 0b00000010) >> 1;
    state->inverter[3].warn = receivedMsg.buf[1] & 0b00000001;
    
    state->inverter[0].diagnosis = (receivedMsg.buf[2] << 4) + ((receivedMsg.buf[3] & 0b11110000) >> 4);
    state->inverter[1].diagnosis = ((receivedMsg.buf[3] & 0b00001111) << 8) + receivedMsg.buf[4];
    state->inverter[2].diagnosis = (receivedMsg.buf[5] << 4) + ((receivedMsg.buf[6] & 0b11110000) >> 4);
    state->inverter[3].diagnosis = ((receivedMsg.buf[6] & 0b00001111) << 8) + receivedMsg.buf[7];
}

void parseVCUTemps(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->inverter[0].temp_mot = receivedMsg.buf[0] - 20;
    state->inverter[1].temp_mot = receivedMsg.buf[1] - 20;
    state->inverter[2].temp_mot = receivedMsg.buf[2] - 20;
    state->inverter[3].temp_mot = receivedMsg.buf[3] - 20;
    state->inverter[0].temp_inv = receivedMsg.buf[4] - 20;
    state->inverter[1].temp_inv= receivedMsg.buf[5] - 20;
    state->inverter[2].temp_inv= receivedMsg.buf[6] - 20;
    state->inverter[3].temp_inv= receivedMsg.buf[7] - 20;
    
    state->temp_mot_high = -20;
    state->temp_mot_low = 1000;
    state->temp_inv_high = -20;
    state->temp_inv_low = 1000;
    for (int i=0; i<4; i++) {
        if (state->temp_inv_high < state->inverter[i].temp_inv)
            state->temp_inv_high = state->inverter[i].temp_inv;
        if (state->temp_inv_low > state->inverter[i].temp_inv)
            state->temp_inv_low = state->inverter[i].temp_inv;

        if (state->temp_mot_high < state->inverter[i].temp_mot)
            state->temp_mot_high = state->inverter[i].temp_mot;
        if (state->temp_mot_low > state->inverter[i].temp_mot)
            state->temp_mot_low = state->inverter[i].temp_mot;
    } 
}

void parseVCUParam(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->torque_lim_pos.setCurrentVal( 0.1 * receivedMsg.buf[0] );
    state->torque_lim_neg.setCurrentVal( 0.1 * receivedMsg.buf[1] );
    state->power_lim.setCurrentVal( receivedMsg.buf[2] );
    state->rpm_lim.setCurrentVal( (receivedMsg.buf[3] << 8) + receivedMsg.buf[4] );
    state->accel_slip.setCurrentVal( receivedMsg.buf[5] );
    state->brake_slip.setCurrentVal( receivedMsg.buf[6] );
    state->vehicle_mode.setCurrentVal( (receivedMsg.buf[7] & 0b11100000) >> 5 );
    
    // Serial.println(state->vehicle_mode.getCurrentVal());
    state->asr.setCurrentVal( (receivedMsg.buf[7] & 0b00010000) >> 4 );
    state->bsr.setCurrentVal( (receivedMsg.buf[7] & 0b00001000) >> 3 );
    state->tv.setCurrentVal( (receivedMsg.buf[7] & 0b00000100) >> 2 );
    state->energy_save.setCurrentVal( (receivedMsg.buf[7] & 0b00000010) >> 1 );
    state->drs.setCurrentVal( receivedMsg.buf[7] & 0b00000001 );
    state->initial_parameters_received = true;
}

void parsePDU(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->fuses.fan_inv = (receivedMsg.buf[2] & 0b1) || ((receivedMsg.buf[2] & 0b10) >> 1);
    state->fuses.drs = (receivedMsg.buf[2] & 0b01000000) >> 6;
    state->fuses.sc = (receivedMsg.buf[2] & 0b10000000) >> 7;
    state->fuses.fan_mot = (receivedMsg.buf[2] & 0b10000000) >> 7;
    state->fuses.swimming_angle = (receivedMsg.buf[1] & 0b1);
    state->fuses.rtds = (receivedMsg.buf[1] & 0b10000) >> 4;
    state->fuses.brakelight = (receivedMsg.buf[1] & 0b100000) >> 5;
    state->fuses.pump_mot = (receivedMsg.buf[0] & 0b1);
    state->fuses.ssb = (receivedMsg.buf[0] & 0b10) >> 1;
    state->fuses.imd = (receivedMsg.buf[0] & 0b100) >> 2;
    state->fuses.inv = (receivedMsg.buf[0] & 0b1000) >> 3;
    state->fuses.gps = (receivedMsg.buf[0] & 0b10000) >> 4;
    state->fuses.vcu = (receivedMsg.buf[0] & 0b100000) >> 5;
    state->fuses.latching = (receivedMsg.buf[0] & 0b1000000) >> 6;
    state->fuses.dis = (receivedMsg.buf[0] & 0b10000000) >> 7;
    state->imd_state = (receivedMsg.buf[4] & 0b11100000) >> 5;
    state->iso_resistance = (receivedMsg.buf[5] << 8) + receivedMsg.buf[6];
}

void parseSSBFReku(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->sc.after_bots = receivedMsg.buf[2] & 0b00000001;
    state->reku_brakeforce = (receivedMsg.buf[0] << 8) + receivedMsg.buf[1]; // Kalibrierung fehlt!
}

void parseSSBR(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->temp_inv_high = (receivedMsg.buf[0] << 4) + (receivedMsg.buf[1] >> 4);        // Kalibrierung fehlt!
    state->temp_inv_low = ((receivedMsg.buf[1] & 0b00001111) << 4) + receivedMsg.buf[2]; // Kalibrierung fehlt!
    // Bei den folgenden Drei werden nur boolsche Werte ausgelesen,
    // allerdings unklar welches bit jetzt wo ist, da in der CAN-Übersicht
    // das komplette Byte als "Inhalt" markiert ist.
    state->sc.after_tsms = receivedMsg.buf[3];
    state->sc.after_bspd = receivedMsg.buf[4];
    state->sc.after_sb_right = receivedMsg.buf[5];
    state->temp_ambient = (receivedMsg.buf[6] << 8) + receivedMsg.buf[7]; // Kalibrierung fehlt!
}

void parseAmsMsg(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->ams_state = (receivedMsg.buf[0] & 0b11100000) >> 5;
    state->sc.state = (receivedMsg.buf[0] & 0b00010000) >> 4;
    state->accu_vlt = 0.2 * (((receivedMsg.buf[0] & 0b00001111) << 8) + (receivedMsg.buf[1]));
    state->soc = 0.392157 * (receivedMsg.buf[2]);
    state->cell_vlt_high = 0.001 * ((receivedMsg.buf[3] << 4) + ((receivedMsg.buf[4] & 0b11110000) >> 4)) + 1;
    state->cell_vlt_low = 0.001 * (((receivedMsg.buf[4] & 0b00001111) << 8) + receivedMsg.buf[5]) + 1;
    state->cell_temp_high = 0.5 * receivedMsg.buf[6];
    state->cell_temp_low = 0.5 * receivedMsg.buf[7];
}

void parseVCUControlsMsg(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->reku_state = (receivedMsg.buf[1] & 0b00000010) >> 1;
    state->vehicle_state = (receivedMsg.buf[1] & 0b00001100) >> 2;
    Serial.println(state->vehicle_state);
    state->config_lock = (receivedMsg.buf[1] & 0b00000001);
}

void parseGps(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->HDOP = receivedMsg.buf[4];
    state->qual_fix = (receivedMsg.buf[5] & 0b11110000) >> 4;
    state->sattelites = receivedMsg.buf[5] & 0b00001111;
}

void parseEmVlt(CAN_message_t &receivedMsg, VehicleState *state)
{
    int32_t voltageBuffer = (receivedMsg.buf[2] << 24) + (receivedMsg.buf[3] << 16) + (receivedMsg.buf[4] << 8) + receivedMsg.buf[5];
    state->voltage = voltageBuffer / 1000;
}

void parseGpsOdo(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->odometer = 0.1 * (receivedMsg.buf[0] + receivedMsg.buf[1]);
}

void parseVCUErrors(CAN_message_t &receivedMsg, VehicleState *state)
{
    state->fail.can_pedals = (receivedMsg.buf[0] & 0b10000000) >> 7;
    state->fail.can_reku = (receivedMsg.buf[0] & 0b01000000) >> 6;
    state->fail.can_gps = (receivedMsg.buf[0] & 0b00100000) >> 5;
    state->fail.can_acc = (receivedMsg.buf[0] & 0b00010000) >> 4;
    state->fail.can_rot = (receivedMsg.buf[0] & 0b00001000) >> 3;
    state->fail.can_mag = (receivedMsg.buf[0] & 0b00000100) >> 2;
    state->fail.can_rear = (receivedMsg.buf[0] & 0b00000010) >> 1;
    state->fail.can_ams = receivedMsg.buf[0] & 0b00000001;
    state->fail.can_em = (receivedMsg.buf[1] & 0b10000000) >> 7;
    state->fail.can_pdu = (receivedMsg.buf[1] & 0b01000000) >> 6;
    state->fail.can_dis = (receivedMsg.buf[1] & 0b00100000) >> 5;
    state->inverter[0].can_fail = (receivedMsg.buf[1] & 0b00010000) >> 4;
    state->inverter[1].can_fail = (receivedMsg.buf[1] & 0b00001000) >> 3;
    state->inverter[2].can_fail = (receivedMsg.buf[1] & 0b00000100) >> 2;
    state->inverter[3].can_fail = (receivedMsg.buf[1] & 0b00000010) >> 1;
    state->fail.pedals = (receivedMsg.buf[2] & 0b10000000) >> 7;
    state->fail.apps = (receivedMsg.buf[2] & 0b01000000) >> 6;
    state->fail.apps1 = (receivedMsg.buf[2] & 0b00100000) >> 5;
    state->fail.apps2 = (receivedMsg.buf[2] & 0b00010000) >> 4;
    state->fail.bps_f = (receivedMsg.buf[2] & 0b00001000) >> 3;
    state->fail.bps_r = (receivedMsg.buf[2] & 0b00000100) >> 2;
    state->fail.brakeforce = (receivedMsg.buf[2] & 0b00000010) >> 1;
    state->fail.steer_angle = receivedMsg.buf[2] & 0b00000001;
    state->fail.acc_x = (receivedMsg.buf[3] & 0b10000000) >> 7;
    state->fail.acc_y = (receivedMsg.buf[3] & 0b01000000) >> 6;
    state->fail.rot_z = (receivedMsg.buf[3] & 0b00100000) >> 5;
    state->fail.gps_v = (receivedMsg.buf[3] & 0b00010000) >> 4;
}

#pragma endregion

} // namespace CAN