#include "Arduino.h"
#include "Metro.h"
#include "can.hpp"
#include "menu.hpp"
#include "display.hpp"
#include "buttons.hpp"
#include "converter.hpp"
#include "setable.hpp"
#include "TLC59116.h"
#include "pushmsg.hpp"

#define ADDR_LEDs 96

Metro timer_display = Metro(100),
	  timer_dis_can = Metro(5),
	  timer_dis_param_can = Metro(100),
	  set_leds = Metro(30),
	  timer_push = Metro(2000);

menu::Menu *currentMenu;
TLC59116 leds = TLC59116(ADDR_LEDs);
CAN::VehicleState *cvs;

#pragma region MENU_DEFINITION

menu::Menu menuMain([] {
	String soc = "SOC " + fltStr(cvs->soc) + "%",
		   voltage = "U " + fltStr(cvs->voltage) + "V",
		   vm = "VM " + vehicleModeStr(cvs->vehicle_mode.getCurrentVal()),
		   vs = "VS " + vehicleStateStr(cvs->vehicle_state);

	display::setContent(
		display::fit("Main", vs, true),
		vm,
		display::fit("AMS " + amsStr(cvs->ams_state),"SC " + boolStrOnOff(cvs->sc.state)),
		display::fit(soc, voltage));
});

menu::Menu motorMain([] {
	String temp_high = "Max " + String(cvs->temp_mot_high) + "C",
		   temp_low = "Min " + String(cvs->temp_mot_low) + "C",
		   fr = "FR " + String(cvs->inverter[0].temp_mot) + "C",
		   fl = "FL " + String(cvs->inverter[1].temp_mot) + "C",
		   rr = "RR " + String(cvs->inverter[2].temp_mot) + "C",
		   rl = "RL " + String(cvs->inverter[3].temp_mot) + "C";
	display::setContent(
		"Motor",
		display::fit(temp_high, temp_low),
		display::fit(fl, fr),
		display::fit(rl, rr));
});

menu::Menu inverterMain([] {
	String temp_high = "Max " + String(cvs->temp_inv_high) + "C",
		   temp_low = "Min " + String(cvs->temp_inv_low) + "C",
		   fr = "FR " + String(cvs->inverter[0].temp_inv) + "C",
		   fl = "FL " + String(cvs->inverter[1].temp_inv) + "C",
		   rr = "RR " + String(cvs->inverter[2].temp_inv) + "C",
		   rl = "RL " + String(cvs->inverter[3].temp_inv) + "C";
	display::setContent(
		"Inverter",
		display::fit(temp_high, temp_low),
		display::fit(fl, fr),
		display::fit(rl, rr));
});

menu::Menu *inverterDetail[4];

void produceInverterDetailFunction(int i)
{
	String inv = "Inverter " + inverterStr(i),
		   quit_hv = "Q HV " + boolStr(cvs->inverter[i].quit_hv),
		   quit_rf = "Q RF " + boolStr(cvs->inverter[i].quit_rf),
		   sys_rdy = "SYS RDY " + boolStr(cvs->inverter[i].sys_ready),
		   warn = "! " + boolStr(cvs->inverter[i].warn),
		   diag = "Diag " + intStr(cvs->inverter[i].diagnosis);
	display::setContent(
		inv,
		display::fit(quit_hv, quit_rf, true),
		display::fit(sys_rdy, warn, true),
		diag);
}

void createInverterDetailMenus()
{
	inverterDetail[0] = new menu::Menu([] { produceInverterDetailFunction(0); }, &inverterMain);
	inverterDetail[1] = new menu::Menu([] { produceInverterDetailFunction(1); }, &inverterMain);
	inverterDetail[2] = new menu::Menu([] { produceInverterDetailFunction(2); }, &inverterMain);
	inverterDetail[3] = new menu::Menu([] { produceInverterDetailFunction(3); }, &inverterMain);
}

menu::Menu accuMain([] {
	String soc_p = "SOC " + fltStr(cvs->soc) + "%",
		   soc_v = "U " + fltStr(cvs->accu_vlt) + "V",
		   cvh = "CVH " + fltStr(cvs->cell_vlt_high) + "V",
		   cvl = "CVL " + fltStr(cvs->cell_vlt_low) + "V",
		   cth = "CTH " + fltStr(cvs->cell_temp_high) + "C",
		   ctl = "CTL " + fltStr(cvs->cell_temp_low) + "C";
	display::setContent(
		"Accumulator",
		display::fit(soc_p, soc_v),
		display::fit(cvh, cth),
		display::fit(cvl, ctl));
});

menu::Menu imdMain([] {
	String imd_state = "IMD " + imdStr(cvs->imd_state),
		   imd_res = "IsoR " + fltStr(cvs->iso_resistance);

	display::setContent(
		"IMD",
		imd_state,
		imd_res);
});

menu::Menu configMain([] {
	String torque_lim_pos = "TQ+ " + fltStr(cvs->torque_lim_pos.getCurrentVal()),
		   torque_lim_neg = "TQ- " + fltStr(cvs->torque_lim_neg.getCurrentVal()),
		   power_lim_pos = "PWRL " + fltStr(cvs->power_lim.getCurrentVal()),
		   rpm_lim = "RPML " + intStr(cvs->rpm_lim.getCurrentVal()),
		   accel_slip = "ASL " + intStr(cvs->accel_slip.getCurrentVal()) + "%",
		   brake_slip = "BSL " + intStr(cvs->brake_slip.getCurrentVal()) + "%",
		   locked_str = "[LOCKED " + boolStr(cvs->config_lock) + "]";

	display::setContent(
		display::fit("Config", locked_str, true),
		display::fit(torque_lim_pos, torque_lim_neg),
		display::fit(power_lim_pos, rpm_lim),
		display::fit(accel_slip, brake_slip)); }, NULL, [] {
	if (cvs->config_lock)
		return menu::SelectType::Default;
	else
		return menu::SelectType::FirstChild; });

template <typename T>
void callConfigFunction(
	String header,
	setable::Setable<T> *sb,
	String unit = "",
	String (*interpreter)(T) = [](T t) { return String(t); })
{
	buttons::ButtonsToBitStamped btns, btns_down;
	buttons::getButtonStates(&btns, NULL, &btns_down);
	int neg = buttons::getNegBtn(btns_down.btns);
	int pos = buttons::getPosBtn(btns_down.btns);

	// check if the buttons are still pressed
	if (millis() - btns_down.stamp_millis > 1500)
	{
		neg = buttons::getNegBtn(btns.btns);
		pos = buttons::getPosBtn(btns.btns);
	}

	if (neg)
		sb->subNewVal();
	else if (pos)
		sb->addNewVal();

	String last_val = "Cur " + interpreter(sb->getCurrentVal()) + " " + unit,
		   new_val = "New " + interpreter(sb->getNewVal()) + " " + unit;

	display::setContent(
		header,
		last_val,
		new_val,
		display::alignRight("[+-" + String(sb->getIncr()) + " " + unit + "]"));
}

menu::Menu configVehicleMode([] { callConfigFunction<uint8_t>("Vehicle Mode", &cvs->vehicle_mode, "", vehicleModeStr); }, &menuMain);
menu::Menu configTorqueLimPos([] { callConfigFunction<double>("Torque Lim+", &cvs->torque_lim_pos, "Nm"); }, &configMain);
menu::Menu configTorqueLimNeg([] { callConfigFunction<double>("Torque Lim-", &cvs->torque_lim_neg, "Nm"); }, &configMain);
menu::Menu configPowerLim([] { callConfigFunction<uint8_t>("Power Lim", &cvs->power_lim, "kW"); }, &configMain);
menu::Menu configRPMLim([] { callConfigFunction<uint16_t>("RPM Lim", &cvs->rpm_lim, "1/min"); }, &configMain);
menu::Menu configAccelSlip([] { callConfigFunction<uint8_t>("Accel Slip", &cvs->accel_slip, "%"); }, &configMain);
menu::Menu configBrakeSlip([] { callConfigFunction<uint8_t>("Brake Slip", &cvs->brake_slip, "%"); }, &configMain);
menu::Menu configASR([] { callConfigFunction<bool>("ASR", &cvs->asr, "", boolStrOnOff); }, &configMain);
menu::Menu configBSR([] { callConfigFunction<bool>("BSR", &cvs->bsr, "", boolStrOnOff); }, &configMain);
menu::Menu configTV([] { callConfigFunction<bool>("TV", &cvs->tv, "", boolStrOnOff); }, &configMain);
menu::Menu configES([] { callConfigFunction<bool>("Energy Save", &cvs->energy_save, "", boolStrOnOff); }, &configMain);
menu::Menu configDRS([] { callConfigFunction<bool>("DRS", &cvs->drs, "", boolStrOnOff); }, &configMain);

menu::Menu disMain([] {
	buttons::ButtonsToBitStamped btns;
	buttons::getButtonStates(&btns);

	String buttons_bin = String(btns.btns.buf, BIN);
	for ( int i=buttons_bin.length(); i<9; i++)
		buttons_bin = "0"+buttons_bin;

	String poti_1 = "PT1 " + String(buttons::getPoti(1)),
		   poti_2 = "PT2 " + String(buttons::getPoti(2)),
		   buttons_str = "BTN " + buttons_bin;


	display::setContent(
		"DIS",
		display::fit(poti_1, poti_2),
		buttons_str);
});

menu::Menu moreInfoDIS([] {
	String odom = "Odom " + fltStr(cvs->odometer) + "m",
		   hdop = "HDOP " + String(cvs->HDOP),
		   satt = "Sat " + String(cvs->sattelites),
		   qual_fix = "QualFix " + String(cvs->qual_fix),
		   bf = "BrakeF " + fltStr(cvs->reku_brakeforce),
		   reku_state = "Reku State " + boolStr(cvs->reku_state),
		   temp_amb = "Temp Amb " + fltStr(cvs->temp_ambient) + "C";

	display::setContent(
		display::fit(odom, hdop),
		display::fit(satt, qual_fix),
		display::fit(reku_state, temp_amb),
		display::alignRight(bf));
},
					   &disMain);

#pragma endregion

void updateLEDs()
{
	String ams = amsStr(cvs->ams_state);
	String vs = vehicleStateStr(cvs->vehicle_state);

	uint8_t drive_led = 0;
	uint8_t vcu_led = vehicleStateStr(cvs->vehicle_state) == "Error";
	uint8_t reku_led = cvs->reku_state;

	if (ams == "Drive")
		if (vs == "Drive")
			drive_led = 0b01;
		else if (vs != "Error")
			drive_led = 0b11; // second bit -> blinking

	leds.setStates(drive_led, reku_led, vcu_led /*funktioniert bei einem display nicht*/, 1000);
}

void setup()
{
	Serial.begin(9600);

	createInverterDetailMenus();
	menuMain.addNeighbour(&disMain);
	menuMain.addNeighbour(&imdMain);
	menuMain.addNeighbour(&accuMain);
	menuMain.addNeighbour(&inverterMain);
	menuMain.addNeighbour(&motorMain);
	menuMain.addNeighbour(&configMain);
	currentMenu = &menuMain;

	buttons::initButtons();

	display::init();
	CAN::init();
	leds.init();
	pushmsg::init();
	cvs = CAN::getVehicleState();

	leds.setStates(ON, ON, ON, 1);
	delay(1000);
	leds.setStates(OFF, OFF, OFF, 1);
}

void loop()
{
	CAN::check();

	if (timer_display.check())
	{
		buttons::updateButtonStates();
		buttons::ButtonsToBitStamped btns;
		buttons::getButtonStates(&btns);

		// LR6 == Toggle Failure Menu
		if (buttons::getFailBtn(btns.btns))
		{
			pushmsg::getFailureMenu()->display();
		}
		else
		{
			// change the menu if a specific button was pressed
			currentMenu = currentMenu->update();
			// update the displays content from the selected menu
			currentMenu->display();
		}

		// write new content to screen
		display::updateScreen();
	}

	if (timer_dis_param_can.check() && CAN::getVehicleState()->initial_parameters_received /* && CAN::paramChanged() */)
		CAN::sendParam();
	if (timer_dis_can.check())
		CAN::sendStatus();

	if (set_leds.check())
		updateLEDs();
}
