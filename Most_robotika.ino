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
#define EXIT_SENSOR 4
#define RISING_EDGE 1
#define FALLING_EDGE 2
#define EDGE_RESET 0

#define RED 2
#define GREEN 1
#define OFF 0
#define BRIDGEALLON 4

// bridge
unsigned long lastrun_bridge;
byte bridge_effect_state;
boolean is_bridge_blinking = false;

// entry sensor variables
boolean entry_sensor_laststate = 1;
boolean entry_sensor_currentstate = 1;
byte entry_sensor_trigger = 0;

// exit sensor variables
boolean exit_sensor_laststate = 1;
boolean exit_sensor_currentstate = 1;
byte exit_sensor_trigger = 0;

// traffic lights
boolean is_traffic_lights_blinking = false;
byte traffic_lights_effect_state;
unsigned long lastrun_traffic_lights;

// misc
unsigned long alarm;

// motors
byte bridge_motor_tilt = 45;
byte traffic_lights_motor_tilt;
unsigned long bridge_motor_lastrun;
unsigned long traffic_lights_motor_lastrun;


/*
 * Control variable responsable for state of off all system.
 *
 * 10 - sensors 0 and 1 not interrupted, bridge lowered, bridge LEDs in 3-2-1 effect, traffic lights green					<-----
 * 20 - sensor 0 interrupted, bidge lowered, bridge LEDs in all-blink effect, traffic lights in alternative blinking (5s)		 |
 * 30 - state of sensors irrelevant, bridge rising, bridge LEDs in all-blink effect, traffic lights red (until risen)			 |
 * 40 - sensor states irrelevant, bridge risen, bidge LEDs in all-blink effect, traffic lights red (indefinitev)				 |
 * 50 - when both sensors reconnect, bidge lowering, bidge LEDs in all-blink effect, traffic lights red						------
 */
byte control = 10;

boolean read_sensor(byte sensor) {
	// read and debounce sensor
	while (true) {
		if (digitalRead(sensor) == 1) {
			//debounce
			delay(10);
			if (digitalRead(sensor) == 1) {
				return true;
			}
		} else {
			delay(10);
			if (digitalRead(sensor) == 0) {
				return false;
			}
		}
	}
}

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
	Serial.begin(9600);
	pinMode(TLG, OUTPUT);
	pinMode(TLR, OUTPUT);
	pinMode(BR_2, OUTPUT);
	pinMode(BR_1, OUTPUT);
	pinMode(BR_0, OUTPUT);
	pinMode(BL_2, OUTPUT);
	pinMode(BL_1, OUTPUT);
	pinMode(BL_0, OUTPUT);
	pinMode(ENTRY_SENSOR, INPUT);
	pinMode(EXIT_SENSOR, OUTPUT);
	lastrun_bridge = millis();
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


	// read entry sensor
	entry_sensor_currentstate = read_sensor(ENTRY_SENSOR);

	// detect entry sensor state change
	if (entry_sensor_laststate != entry_sensor_currentstate) {
		entry_sensor_laststate = entry_sensor_currentstate;
		if (entry_sensor_currentstate == true) {
			entry_sensor_trigger = RISING_EDGE;
		} else {
			entry_sensor_trigger = FALLING_EDGE;
		}
	}


	// read exit sensor
	exit_sensor_currentstate = read_sensor(EXIT_SENSOR);

	// detect exit sensor state change
	if (exit_sensor_laststate != exit_sensor_currentstate) {
		exit_sensor_laststate = exit_sensor_currentstate;
		if (exit_sensor_currentstate == true) {
			exit_sensor_trigger = RISING_EDGE;
		} else {
			exit_sensor_trigger = FALLING_EDGE;
		}
	}

	// ------------------------------------------------
	// Logic for state changing
	// ------------------------------------------------

	if ((control == 10) && ((entry_sensor_trigger == RISING_EDGE) || (exit_sensor_trigger == RISING_EDGE))) {
		// sensor 0 or 1 interrupted, bidge lowered -> from state 10 to state 20
		control = 20;	// bridge LEDs in all-blink effect, traffic lights in alternative blinking (5s)
		is_bridge_blinking = true;
		is_traffic_lights_blinking = true;
		entry_sensor_trigger = EDGE_RESET;
		exit_sensor_trigger = EDGE_RESET;
		alarm = millis() + 5000;
		Serial.println("State No. 20");
	}

	if ((control == 20) && (millis() > alarm)) {
		// bridge rising, bridge LEDs in all-blink effect, traffic lights red (until risen)
		control = 30;
		bridge_effect_state = BRIDGEALLON;
		is_traffic_lights_blinking = false;
		traffic_ligths(RED);
		bridge_motor_lastrun = millis();
		Serial.println("State No. 30");
	}

	if ((control == 30) && (bridge_motor_tilt <= 90) && (millis() - bridge_motor_lastrun > 100)) {
		Serial.println(bridge_motor_tilt);
		bridge_motor_tilt++;
		bridge_motor_lastrun = millis();
	}

	// ------------------------------------------------
	// Logic for blinking
	// ------------------------------------------------

	// bridge blinking
	if (!is_bridge_blinking && (millis() - lastrun_bridge > 1000)) {
		if (bridge_effect_state == 1) {
			bridge_effect_state = 2;
		} else if (bridge_effect_state == 2) {
			bridge_effect_state = 3;
		} else {
			bridge_effect_state = 1;
		}
		bridge_effect(bridge_effect_state);
		lastrun_bridge = millis();
	}
	if (is_bridge_blinking && (millis() - lastrun_bridge > 1000)) {
		if (bridge_effect_state == OFF) {
			bridge_effect_state = BRIDGEALLON;
		} else {
			bridge_effect_state = OFF;
		}
		bridge_effect(bridge_effect_state);
		lastrun_bridge = millis();
	}

	// traffic lights blinking
	if ((is_traffic_lights_blinking) && (millis() - lastrun_traffic_lights > 750)) {
		if (traffic_lights_effect_state == 1) {
			traffic_lights_effect_state = 2;
		} else {
			traffic_lights_effect_state = 1;
		}
		traffic_ligths(traffic_lights_effect_state);
		lastrun_traffic_lights = millis();
	}
}
