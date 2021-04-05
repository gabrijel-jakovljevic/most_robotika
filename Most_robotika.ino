// primjer_0 - MLVC: M = most, L = lijevo, V = velika, C = crvena
// primjer_1 - SZ:  = semafor, Z = zeleno

#define SZ 8
#define SC 7
#define MLVC_0 A0
#define MLVC_1 A1
#define MLVC_2 A2
#define MDVC_2 A3
#define MDVC_1 A4
#define MDVC_0 A5
#define SDMC 12
#define SDMZ 13

#define ENTRY_SENSOR 2
#define EXIST_SENSOR 4

#define RED 2
#define GREEN 1
#define OFF 0
#define BRIDGEALLON 4

long lastrun;
byte traffic_state;
byte entry_sensor_laststate;
byte entry_sensor_currentstate;
byte bridge_effect_state;
boolean is_bridge_blinking = true;

/*
 * Semafor kontrol funkcija.
 *
 * 0 - ugasi sve
 * 1 - zeleno
 * 2 - crveno
 */
void traffic_ligths(byte mode) {
	switch (mode) {
		case 0:
			digitalWrite(SC, LOW);
			digitalWrite(SZ, LOW);
			break;
		case 1:
			digitalWrite(SC, LOW);
			digitalWrite(SZ, HIGH);
			break;
		case 2:
			digitalWrite(SC, HIGH);
			digitalWrite(SZ, LOW);
			break;
	}
}

/*
 * Function to control lights on the bridge
 * 0 - all OFF
 * 1 - closest to the base of the bridge
 * 2 - in the middle of the bridge
 * 3 - farthest form base of the bridge
 */
void bridge_effect(byte mode) {
	switch (mode) {
		case 0:
			digitalWrite(MLVC_0, LOW);
			digitalWrite(MLVC_1, LOW);
			digitalWrite(MLVC_2, LOW);
			digitalWrite(MDVC_0, LOW);
			digitalWrite(MDVC_1, LOW);
			digitalWrite(MDVC_2, LOW);
			break;
		case 1:
			digitalWrite(MLVC_0, HIGH);
			digitalWrite(MLVC_1, LOW);
			digitalWrite(MLVC_2, LOW);
			digitalWrite(MDVC_0, HIGH);
			digitalWrite(MDVC_1, LOW);
			digitalWrite(MDVC_2, LOW);
			break;
		case 2:
			digitalWrite(MLVC_0, LOW);
			digitalWrite(MLVC_1, HIGH);
			digitalWrite(MLVC_2, LOW);
			digitalWrite(MDVC_0, LOW);
			digitalWrite(MDVC_1, HIGH);
			digitalWrite(MDVC_2, LOW);
			break;
		case 3:
			digitalWrite(MLVC_0, LOW);
			digitalWrite(MLVC_1, LOW);
			digitalWrite(MLVC_2, HIGH);
			digitalWrite(MDVC_0, LOW);
			digitalWrite(MDVC_1, LOW);
			digitalWrite(MDVC_2, HIGH);
			break;
		case 4:
			digitalWrite(MLVC_0, HIGH);
			digitalWrite(MLVC_1, HIGH);
			digitalWrite(MLVC_2, HIGH);
			digitalWrite(MDVC_0, HIGH);
			digitalWrite(MDVC_1, HIGH);
			digitalWrite(MDVC_2, HIGH);

	}
}

void setup() {
	pinMode(SDMZ, OUTPUT);
	pinMode(SDMC, OUTPUT);
	pinMode(SZ, OUTPUT);
	pinMode(SC, OUTPUT);
	pinMode(MDVC_2, OUTPUT);
	pinMode(MDVC_1, OUTPUT);
	pinMode(MDVC_0, OUTPUT);
	pinMode(MLVC_2, OUTPUT);
	pinMode(MLVC_1, OUTPUT);
	pinMode(MLVC_0, OUTPUT);
	pinMode(ENTRY_SENSOR, INPUT);
	pinMode(EXIST_SENSOR, OUTPUT);
	lastrun = millis();
	traffic_ligths(GREEN);
	bridge_effect(OFF);
}

void loop() {
	/*	if (millis() - lastrun > 1000) {
	 if (traffic_state == 1) {
	 traffic_state = 2;
	 } else {
	 traffic_state = 1;
	 }
	 traffic_ligths(traffic_state);
	 lastrun = millis();
	 }
	 */
	delay(10);

	// read and debounce sensor 0
	if (digitalRead(ENTRY_SENSOR) == 1) {
		//debounce
		delay(10);
		if (digitalRead(ENTRY_SENSOR) == 1) {
			entry_sensor_currentstate = 1;
		}
	} else {
		delay(10);
		if (digitalRead(ENTRY_SENSOR) == 0) {
			entry_sensor_currentstate = 0;
		}
	}

	// detect entry sensor state change
	if (entry_sensor_laststate != entry_sensor_currentstate) {
		entry_sensor_laststate = entry_sensor_currentstate;
		if (entry_sensor_currentstate == 1) {
			traffic_ligths(RED);
			is_bridge_blinking = false;
			bridge_effect(BRIDGEALLON);
		} else {
			traffic_ligths(GREEN);
			is_bridge_blinking = true;
			// bridge_effect(OFF);
		}
	}

	// lowered state blinking
	if (is_bridge_blinking && (millis() - lastrun > 1000)) {
		if (bridge_effect_state == 1) {
			bridge_effect_state = 2;
		} else if (bridge_effect_state == 2) {
			bridge_effect_state = 3;
		} else {
			bridge_effect_state = 1;
		}
		bridge_effect(bridge_effect_state);
		lastrun = millis();
	}
	if (!is_bridge_blinking && (millis() - lastrun > 1000)) {
		if (bridge_effect_state == OFF) {
			bridge_effect_state = BRIDGEALLON;
		} else {
			bridge_effect_state = OFF;
		}
		bridge_effect(bridge_effect_state);
		lastrun = millis();
	}
}
