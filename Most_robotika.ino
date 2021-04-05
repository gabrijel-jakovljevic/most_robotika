// primjer - SLMZ: S = semafor, L = lijevo, M = mala, Z = zelena

#define SLMZ 8
#define SLMC 7
#define MLVC_0 A0
#define MLVC_1 A1
#define MLVC_2 A2
#define MDVC_0 A3
#define MDVC_1 A4
#define MDVC_2 A5
#define SDMC 12
#define SDMZ 13

void setup() {
	pinMode(SDMZ, OUTPUT);
	pinMode(SDMC, OUTPUT);
	pinMode(SLMZ, OUTPUT);
	pinMode(SLMC, OUTPUT);
	pinMode(MDVC_2, OUTPUT);
	pinMode(MDVC_1, OUTPUT);
	pinMode(MDVC_0, OUTPUT);
	pinMode(MLVC_2, OUTPUT);
	pinMode(MLVC_1, OUTPUT);
	pinMode(MLVC_0, OUTPUT);
}

void loop() {
	digitalWrite(SDMZ, HIGH);
	digitalWrite(SDMC, HIGH);
	digitalWrite(SLMZ, HIGH);
	digitalWrite(SLMC, HIGH);
	digitalWrite(MDVC_2, HIGH);
	digitalWrite(MDVC_1, HIGH);
	digitalWrite(MDVC_0, HIGH);
	digitalWrite(MLVC_2, HIGH);
	digitalWrite(MLVC_1, HIGH);
	digitalWrite(MLVC_0, HIGH);
}
