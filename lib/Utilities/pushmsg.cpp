#include "pushmsg.hpp"
#include "display.hpp"
#include "converter.hpp"
#include "menu.hpp"

namespace pushmsg
{
    
menu::Menu *gfFail,
    *canFail,
    *canFail2,
    *fusesFail,
    *fusesFail2,
    *fusesFail3,
    *scFail,
    *implFail,
    *ptr;

CAN::VehicleState *cvs;
int last_next_call = 0;

void init()
{
    cvs = CAN::getVehicleState();

    gfFail = new menu::Menu([] {
        String apps1 = boolStrX(cvs->fail.apps1) + " APPS1",
               apps2 = "APPS2 " + boolStrX(cvs->fail.apps2),
               bps_f = boolStrX(cvs->fail.bps_f) + " BPS F",
               bps_r = "BPS R " + boolStrX(cvs->fail.bps_r),
               bf = "BrakeF " + boolStrX(cvs->fail.brakeforce);
        display::setContent(
            display::fit("General Fail", "1/8", true),
            display::fit(apps1, apps2, true),
            display::fit(bps_f, bps_r, true),
            display::alignRight(bf));
    });

    canFail = new menu::Menu([] {
        String pedals = boolStrX(cvs->fail.can_pedals) + " Pedals",
               reku = "Reku " + boolStrX(cvs->fail.can_reku),
               gps = boolStrX(cvs->fail.can_gps) + " GPS",
               acc = "Acc " + boolStrX(cvs->fail.can_acc),
               rot = boolStrX(cvs->fail.can_rot) + " Rot",
               mag = "Mag " + boolStrX(cvs->fail.can_mag);

        display::setContent(
            display::fit("CAN", "2/8", true),
            display::fit(pedals, reku, true),
            display::fit(gps, acc, true),
            display::fit(rot, mag, true));
    });

    canFail2 = new menu::Menu([] {
        String inv_fail[4];
        for (int i = 0; i < 4; i++)
            if (i%2 == 0)
                inv_fail[i] = "INV " + inverterStr(i) + " " + boolStrX(cvs->inverter[i].can_fail);
            else
                inv_fail[i] = boolStrX(cvs->inverter[i].can_fail) + " INV " + inverterStr(i);

        String rear = boolStrX(cvs->fail.can_rear) + " SSB R",
               ams = "AMS " + boolStrX(cvs->fail.can_ams);

        display::setContent(
            display::fit("CAN", "3/8", true),
            display::fit(inv_fail[1], inv_fail[0], true),
            display::fit(inv_fail[3], inv_fail[2], true),
            display::fit(rear, ams, true));
    });

    fusesFail = new menu::Menu([] {
        String sc_lat = boolStrX(!cvs->fuses.latching) + " SC Latch",
               inv = "Inv " + boolStrX(!cvs->fuses.inv),
               imd = boolStrX(!cvs->fuses.imd) + " IMD",
               vcu = "VCU " + boolStrX(!cvs->fuses.vcu),
               ssb = boolStrX(!cvs->fuses.ssb) + " SSB",
               rtds = "RTDS " + boolStrX(!cvs->fuses.rtds);

        display::setContent(
            display::fit("Fuses", "4/8", true),
            display::fit(sc_lat, inv, true),
            display::fit(imd, vcu, true),
            display::fit(ssb, rtds, true));
    });

    fusesFail2 = new menu::Menu([] {
        String pump_mot = boolStrX(!cvs->fuses.pump_mot) + " PumpMot",
               fan_mot = "FanMot " + boolStrX(!cvs->fuses.fan_mot),
               pump_inv = boolStrX(!cvs->fuses.pump_inv) + " PumpInv",
               fan_inv = "FanInv " + boolStrX(!cvs->fuses.fan_inv),
               drs = boolStrX(!cvs->fuses.drs) + " DRS",
               sc = "SC " + boolStrX(!cvs->fuses.sc);

        display::setContent(
            display::fit("Fuses", "5/8", true),
            display::fit(pump_mot, fan_mot, true),
            display::fit(pump_inv, fan_inv, true),
            display::fit(drs, sc, true));
    });

    fusesFail3 = new menu::Menu([] {
        String gps = boolStrX(!cvs->fuses.gps) + " GPS",
               dis = "DIS " + boolStrX(!cvs->fuses.dis);

        display::setContent(
            display::fit("Fuses", "6/8", true),
            display::fit(gps, dis, true));
    });

    scFail = new menu::Menu([] {
        String bspd = boolStrX(cvs->sc.after_bspd) + " BSPD",
               sb_right = "SB Right " + boolStrX(cvs->sc.after_sb_right),
               bots = boolStrX(cvs->sc.after_bots) + " BOTS",
               tsms = "TSMS " + boolStrX(cvs->sc.after_tsms),
               sc_state = "SC State " + boolStrX(cvs->sc_state);

        display::setContent(
            display::fit("SC", "7/8", true),
            display::fit(bspd, sb_right, true),
            display::fit(bots, tsms, true),
            display::alignRight(sc_state));
    });

    implFail = new menu::Menu([] {
        String pedal = boolStrX(cvs->fail.pedals) + " Pedal",
               apps = "APPS " + boolStrX(cvs->fail.apps),
               steering = boolStrX(cvs->fail.steer_angle) + " Steering",
               rot_z = "Rot Z " + boolStrX(cvs->fail.rot_z),
               acc_x = boolStrX(cvs->fail.acc_x) + " Acc X",
               acc_y = "Acc Y " + boolStrX(cvs->fail.acc_y);

        display::setContent(
            display::fit("Implausibility", "8/8", true),
            display::fit(pedal, apps, true),
            display::fit(steering, rot_z, true),
            display::fit(acc_x, acc_y, true));
    });

    gfFail->addNeighbour(canFail);
    gfFail->addNeighbour(canFail2);
    gfFail->addNeighbour(fusesFail);
    gfFail->addNeighbour(fusesFail2);
    gfFail->addNeighbour(fusesFail3);
    gfFail->addNeighbour(scFail);
    gfFail->addNeighbour(implFail);
    ptr = gfFail;

}

menu::Menu *getFailureMenu()
{
    if (millis() - last_next_call < 4000)
        return ptr;
        
    // ptr has always a neighbour, as the failure menus are closed in a loop
    ptr = ptr->advance();
    last_next_call = millis();
    return ptr;
}

} // namespace pushmsg