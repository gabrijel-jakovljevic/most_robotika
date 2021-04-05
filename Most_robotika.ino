// primjer_0 - MLVC: M = most, L = lijevo, V = velika, C = crvena
// primjer_1 - SZ:  = semafor, Z = zeleno

#define SZ 8
#define SC 7
#define MLVC_0 A0
#define MLVC_1 A1
#define MLVC_2 A2
#define MDVC_0 A3
#define MDVC_1 A4
#define MDVC_2 A5
#define SDMC 12
#define SDMZ 13

#define ENTRY_SENSOR 2
#define EXIST_SENSOR 4

#define RED 2
#define GREEN 1
#define OFF 0

long lastrun;
byte traffic_state;
byte entry_sensor_laststate;
byte entry_sensor_currentstate;

/**
 * Semfor kontrol funkcija.
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

void bridge_effect() {

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
		} else {
			traffic_ligths(GREEN);
		}
	}
}
