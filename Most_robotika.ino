// Naming:
// BL: B = bridge, L = left
// TLG:  = traffic light, G = green

#define TLG 8
#define TLR 7
#define BL_0 A0
#define BL_1 A1
#define BL_2 A2
#define BR_2 A3
#define BR_1 A4
#define BR_0 A5

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
 * Traffic lights control.
 *
 * 0 - all off
 * 1 - green
 * 2 - red
 */
void traffic_ligths(byte mode) {
	switch (mode) {
		case 0:
			digitalWrite(TLR, LOW);
			digitalWrite(TLG, LOW);
			break;
		case 1:
			digitalWrite(TLR, LOW);
			digitalWrite(TLG, HIGH);
			break;
		case 2:
			digitalWrite(TLR, HIGH);
			digitalWrite(TLG, LOW);
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
			digitalWrite(BL_0, LOW);
			digitalWrite(BL_1, LOW);
			digitalWrite(BL_2, LOW);
			digitalWrite(BR_0, LOW);
			digitalWrite(BR_1, LOW);
			digitalWrite(BR_2, LOW);
			break;
		case 1:
			digitalWrite(BL_0, HIGH);
			digitalWrite(BL_1, LOW);
			digitalWrite(BL_2, LOW);
			digitalWrite(BR_0, HIGH);
			digitalWrite(BR_1, LOW);
			digitalWrite(BR_2, LOW);
			break;
		case 2:
			digitalWrite(BL_0, LOW);
			digitalWrite(BL_1, HIGH);
			digitalWrite(BL_2, LOW);
			digitalWrite(BR_0, LOW);
			digitalWrite(BR_1, HIGH);
			digitalWrite(BR_2, LOW);
			break;
		case 3:
			digitalWrite(BL_0, LOW);
			digitalWrite(BL_1, LOW);
			digitalWrite(BL_2, HIGH);
			digitalWrite(BR_0, LOW);
			digitalWrite(BR_1, LOW);
			digitalWrite(BR_2, HIGH);
			break;
		case 4:
			digitalWrite(BL_0, HIGH);
			digitalWrite(BL_1, HIGH);
			digitalWrite(BL_2, HIGH);
			digitalWrite(BR_0, HIGH);
			digitalWrite(BR_1, HIGH);
			digitalWrite(BR_2, HIGH);

	}
}

void setup() {
	pinMode(TLG, OUTPUT);
	pinMode(TLR, OUTPUT);
	pinMode(BR_2, OUTPUT);
	pinMode(BR_1, OUTPUT);
	pinMode(BR_0, OUTPUT);
	pinMode(BL_2, OUTPUT);
	pinMode(BL_1, OUTPUT);
	pinMode(BL_0, OUTPUT);
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
