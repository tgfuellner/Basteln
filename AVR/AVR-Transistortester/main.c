#include <avr/io.h>
#include "lcd-routines.h"
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include <string.h>
#include <avr/wdt.h>

//Strings im EEPROM
unsigned char TestRunning[] EEMEM = "Test láuft...";
unsigned char Bat[] EEMEM = "Batterie ";
unsigned char BatWeak[] EEMEM = "schwach";
unsigned char BatEmpty[] EEMEM = "leer!";
unsigned char TestFailed1[] EEMEM = "Kein,unbek. oder";
unsigned char TestFailed2[] EEMEM = "defektes ";
unsigned char Bauteil[] EEMEM = "Bauteil";
unsigned char Unknown[] EEMEM = " unbek.";
unsigned char OrBroken[] EEMEM = "oder defekt";
unsigned char Diode[] EEMEM = "Diode: ";
unsigned char DualDiode[] EEMEM = "Doppeldiode ";
unsigned char TwoDiodes[] EEMEM = "2 Dioden";
unsigned char Antiparallel[] EEMEM = "antiparallel";
unsigned char InSeries[] EEMEM = "in Serie A=";
unsigned char mosfet[] EEMEM = "-MOS";
unsigned char emode[] EEMEM = "-E";
unsigned char dmode[] EEMEM = "-D";
unsigned char jfet[] EEMEM = "-JFET";
unsigned char Thyristor[] EEMEM = "Thyristor";
unsigned char Triac[] EEMEM = "Triac";
unsigned char A1[] EEMEM = ";A1=";
unsigned char A2[] EEMEM = ";A2=";
unsigned char hfestr[] EEMEM ="hFE = ";
unsigned char NPN[] EEMEM = "NPN";
unsigned char PNP[] EEMEM = "PNP";
unsigned char bstr[] EEMEM = " B=";
unsigned char cstr[] EEMEM = ";C=";
unsigned char estr[] EEMEM = ";E=";
unsigned char gds[] EEMEM = "GDS=";
unsigned char Uf[] EEMEM = "Uf = ";
unsigned char vt[] EEMEM = "Vt=";
unsigned char mV[] EEMEM = "mV";
unsigned char Resistor[] EEMEM = "Widerstand: ";
unsigned char TestTimedOut[] EEMEM = "Timeout!";
unsigned char DiodeIcon[] EEMEM = {4,31,31,14,14,4,31,4,0};	//Dioden-Icon

struct Diode {
	uint8_t Anode;
	uint8_t Cathode;
	int Voltage;
};

void CheckPins(uint8_t HighPin, uint8_t LowPin, uint8_t TristatePin);
void DischargePin(uint8_t PinToDischarge, uint8_t DischargeDirection);
unsigned int ReadADC(uint8_t mux);
void GetGateThresholdVoltage(void);


#define R_DDR DDRB
#define R_PORT PORTB

/* Port für die Testwiderstände
	Die Widerstände müssen an die unteren 6 Pins des Ports angeschlossen werden,
	und zwar in folgender Reihenfolge:
	RLx = 680R-Widerstand für Test-Pin x
	RHx = 470k-Widerstand für Test-Pin x

	RL1 an Pin 0
	RH1 an Pin 1
	RL2 an Pin 2
	RH2 an Pin 3
	RL3 an Pin 4
	RH3 an Pin 5


*/


#define ADC_PORT PORTC
#define ADC_DDR DDRC
#define ADC_PIN PINC
#define TP1 PC0
#define TP2 PC1
#define TP3 PC2

/* Port für die Test-Pins
	Dieser Port muss über einen ADC verfügen (beim Mega8 also PORTC).
	Für die Test-Pins müssen die unteren 3 Pins dieses Ports benutzt werden.
	Bitte die Tefinitionen für TP1, TP2 und TP3 nicht ändern!
*/

#define ON_DDR DDRD
#define ON_PORT PORTD
#define ON_PIN_REG PIND
#define ON_PIN PD6	//Pin, der auf high gezogen werden muss, um Schaltung in Betrieb zu halten
#define RST_PIN PD7	//Pin, der auf low gezogen wird, wenn der Einschalt-Taster gedrückt wird

//Bauteile
#define PART_NONE 0
#define PART_DIODE 1
#define PART_TRANSISTOR 2
#define PART_FET 3
#define PART_TRIAC 4
#define PART_THYRISTOR 5
#define PART_RESISTOR 6
//Ende (Bauteile)
//Spezielle Definitionen für Bauteile
#define PART_MODE_N_E_MOS 1
#define PART_MODE_P_E_MOS 2
#define PART_MODE_N_D_MOS 3
#define PART_MODE_P_D_MOS 4
#define PART_MODE_N_JFET 5
#define PART_MODE_P_JFET 6

#define PART_MODE_NPN 1
#define PART_MODE_PNP 2

//Zeilen für LCD
#define Line1() lcd_command(128)
#define Line2() lcd_command(192)

struct Diode diodes[3];
uint8_t NumOfDiodes;

uint8_t b,c,e;		//Anschlüsse des Transistors
unsigned long hfe;	//Verstärkungsfaktor
uint8_t PartReady;	//Bauteil fertig erkannt
unsigned int hfe1, hfe2;	//Verstärkungsfaktoren
unsigned int uBE1, uBE2;
uint8_t PartMode;
uint8_t ra, rb;			//Widerstands-Pins
unsigned int rv1, rv2;	//Spannungsabfall am Widerstand

uint8_t PartFound;	//das gefundene Bauteil
char outval[6];

int main(void) {
	//Einschalten
	ON_DDR |= (1<<ON_PIN);
	ON_PORT |= (1<<ON_PIN) | (1<<RST_PIN);	//Strom an und Pullup für Reset-Pin
	uint8_t tmp;
	//ADC-Init
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);	//Vorteiler=8

	lcd_init();

	if(MCUCSR & (1<<WDRF)) {	
		/*
		Überprüfen auf Watchdog-Reset 
		Das tritt ein, wenn der Watchdog 2s nicht zurückgesetzt wurde
		Kann vorkommen, wenn sich das Programm in einer Endlosschleife "verheddert" hat.
		*/
		lcd_eep_string(TestTimedOut);	//Timeout-Meldung
		_delay_ms(3000);
		ON_PORT = 0;	//Abschalten!
		ON_DDR = 0;
		return 0;
	}
	lcd_command(0x40);	//Custom-Zeichen
	//Diodensymbol in LCD laden
	lcd_eep_string(DiodeIcon);
	Line1();	//1. Zeile
	//Einsprungspunkt, wenn Start-Taste im Betrieb erneut gedrückt wird
	start:
	wdt_enable(WDTO_2S);	//Watchdog an
	PartFound = PART_NONE;
	NumOfDiodes = 0;
	PartReady = 0;
	PartMode = 0;
	lcd_clear();
	//Versorgungsspannung messen
	ReadADC(5 | (1<<REFS1));	//Dummy-Readout
	hfe1 = ReadADC(5 | (1<<REFS1)); 	//mit interner Referenz
	if (hfe1 < 650) {			//Vcc < 7,6V; Warnung anzeigen
		lcd_eep_string(Bat);		//Anzeige: "Batterie"
		if(hfe1 < 600) {					//Vcc <7,15V; zuverlässiger Betrieb nicht mehr möglich
			lcd_eep_string(BatEmpty);		//Batterie leer!
			_delay_ms(1000);
			PORTD = 0;	//abschalten
			return 0;
		}
		lcd_eep_string(BatWeak);		//Batterie schwach
		Line2();
	}
	//Test beginnen
	lcd_eep_string(TestRunning);	//String: Test läuft
	//Alle 6 Kombinationsmöglichkeiten für die 3 Pins prüfen
	CheckPins(TP1, TP2, TP3);
	CheckPins(TP1, TP3, TP2);
	CheckPins(TP2, TP1, TP3);
	CheckPins(TP2, TP3, TP1);
	CheckPins(TP3, TP2, TP1);
	CheckPins(TP3, TP1, TP2);
	//Fertig, jetzt folgt die Auswertung
	GetGateThresholdVoltage();
	lcd_clear();
	if((PartFound == PART_DIODE) && (NumOfDiodes == 1)) {
		//Standard-Diode
		lcd_eep_string(Diode);	//"Diode: "
		lcd_string("A=");
		lcd_data(diodes[0].Anode + 49);
		lcd_string(";K=");
		lcd_data(diodes[0].Cathode + 49);
		Line2();	//2. Zeile
		lcd_eep_string(Uf);	//"Uf = "
		lcd_string(itoa(diodes[0].Voltage, outval, 10));
		lcd_eep_string(mV);
		goto end;
	} else if((PartFound == PART_DIODE) && (NumOfDiodes == 2)) {
		//Doppeldiode
		if(diodes[0].Anode == diodes[1].Anode) {
			//Common Anode
			lcd_eep_string(DualDiode);	//Doppeldiode
			lcd_string("CA");
			Line2(); //2. Zeile
			lcd_string("A=");
			lcd_data(diodes[0].Anode + 49);
			lcd_string(";K1=");
			lcd_data(diodes[0].Cathode + 49);
			lcd_string(";K2=");
			lcd_data(diodes[1].Cathode + 49);
		} else if(diodes[0].Cathode == diodes[1].Cathode) {
			//Common Cathode
			lcd_eep_string(DualDiode);	//Doppeldiode
			lcd_string("CC");
			Line2(); //2. Zeile
			lcd_string("K=");
			lcd_data(diodes[0].Cathode + 49);
			lcd_eep_string(A1);		//";A1="
			lcd_data(diodes[0].Anode + 49);
			lcd_eep_string(A2);		//";A2="
			lcd_data(diodes[1].Anode + 49);
		} else if ((diodes[0].Cathode == diodes[1].Anode) && (diodes[1].Cathode == diodes[0].Anode)) {
			//Antiparallel
			lcd_eep_string(TwoDiodes);	//2 Dioden
			Line2(); //2. Zeile
			lcd_eep_string(Antiparallel);	//Antiparallel
		} else if ((diodes[0].Cathode == diodes[1].Anode) || (diodes[1].Cathode == diodes[0].Anode)) {
			//Serienschaltung
			lcd_eep_string(TwoDiodes);	//2 Dioden
			Line2(); //2. Zeile
			lcd_eep_string(InSeries); //"in Serie A="
			if(diodes[0].Cathode == diodes[1].Anode) {
				lcd_data(diodes[0].Anode + 49);
				lcd_string(";K=");
				lcd_data(diodes[1].Cathode + 49);
			} else {
				lcd_data(diodes[1].Anode + 49);
				lcd_string(";K=");
				lcd_data(diodes[0].Cathode + 49);
			}
		}
		goto end;
	} else if (PartFound == PART_TRANSISTOR) {
		if(PartReady == 0) {	//Wenn 2. Prüfung nie gemacht, z.B. bei Transistor mit Schutzdiode
			hfe2 = hfe1;
			uBE2 = uBE1;
		}
		if((hfe1>hfe2)) {	//Wenn der Verstärkungsfaktor beim ersten Test höher war: C und E vertauschen!
			hfe2 = hfe1;
			uBE2 = uBE1;
			tmp = c;
			c = e;
			e = tmp;
		}
		if(PartMode == PART_MODE_NPN) {
			lcd_eep_string(NPN);
		} else {
			lcd_eep_string(PNP);
		}
		lcd_eep_string(bstr);	//B=
		lcd_data(b + 49);
		lcd_eep_string(cstr);	//;C=
		lcd_data(c + 49);
		lcd_eep_string(estr);	//;E=
		lcd_data(e + 49);
		Line2(); //2. Zeile
		//Verstärkungsfaktor berechnen
		//hFE = Emitterstrom / Basisstrom
		hfe = hfe2;
		hfe *= 691;	//Verhältnis von High- zu Low-Widerstand
		if(uBE2<11) uBE2 = 11;
		hfe /= uBE2;
		hfe2 = (unsigned int) hfe;
		lcd_eep_string(hfestr);	//"hFE = "
		lcd_string(utoa(hfe2, outval, 10));
		if(NumOfDiodes == 3) {	//Transistor mit Schutzdiode
			lcd_command(0xCD); //Cursor auf Zeile 2, Zeichen 13
			lcd_data(0);	//Diode anzeigen
		}
		goto end;		
	} else if (PartFound == PART_FET) {	//JFET oder MOSFET
		if(PartMode&1) {	//N-Kanal
			lcd_data('N');
		} else {
			lcd_data('P');	//P-Kanal
		}
		if((PartMode==PART_MODE_N_D_MOS) || (PartMode==PART_MODE_P_D_MOS)) {
			lcd_eep_string(dmode);	//"-D"
			lcd_eep_string(mosfet);	//"-MOS"
		} else {
			if((PartMode==PART_MODE_N_JFET) || (PartMode==PART_MODE_P_JFET)) {
				lcd_eep_string(jfet);	//"-JFET"
			} else {
				lcd_eep_string(emode);	//"-E"
				lcd_eep_string(mosfet);	//"-MOS"
			}
		}
		Line2(); //2. Zeile
		lcd_eep_string(gds);	//"GDS="
		lcd_data(b + 49);
		lcd_data(c + 49);
		lcd_data(e + 49);
		if(NumOfDiodes == 1) {	//MOSFET mit Schutzdiode
			lcd_data(0);	//Diode anzeigen
		} else {
			lcd_data(' ');	//Leerzeichen
		}
		if(PartMode < 3) {	//Anreicherungs-MOSFET
			lcd_eep_string(vt);
			lcd_string(outval);	//Gate-Schwellspannung, wurde zuvor ermittelt
			lcd_data('m');
		}
		goto end;
	} else if (PartFound == PART_THYRISTOR) {
		lcd_eep_string(Thyristor);	//"Thyristor"
		Line2(); //2. Zeile
		lcd_string("G=");
		lcd_data(b + 49);
		lcd_string(";A=");
		lcd_data(c + 49);
		lcd_string(";K=");
		lcd_data(e + 49);
		goto end;
	} else if (PartFound == PART_TRIAC) {
		lcd_eep_string(Triac);	//"Triac"
		Line2(); //2. Zeile
		lcd_string("G=");
		lcd_data(b + 49);
		lcd_eep_string(A1);		//";A1="
		lcd_data(c + 49);
		lcd_eep_string(A2);		//";A2="
		lcd_data(e + 49);
		goto end;
	} else if(PartFound == PART_RESISTOR) {
		lcd_eep_string(Resistor); //"Widerstand: "
		lcd_data(ra + 49);	//Pin-Angaben
		lcd_data('-');
		lcd_data(rb + 49);
		Line2(); //2. Zeile
		lcd_string ("R = ");
		if(rv1>512) {		//Überprüfen, wie weit die an den Testwiderständen anliegenden Spannungen von 512 abweichen
			hfe1 = (rv1 - 512);
		} else {
			hfe1 = (512 - rv1);
		}
		if(rv2>512) {
			hfe2 = (rv2 - 512);
		} else {
			hfe2 = (512 - rv2);
		}
		if(hfe1 > hfe2)  {
			rv1 = rv2;	//Ergebnis verwenden, welches näher an 512 liegt (bessere Genauigkeit)
			rv2 = 4700;	//470k-Testwiderstand
		} else {
			rv2 = 680;	//680R-Testwiderstand
		}
		if(rv1==0) rv1 = 1;
		hfe = (long)((long)((long)rv2 * (long)rv1) / (long)(1023 - rv1));	//Widerstand berechnen
		hfe1 = (unsigned int)hfe;
		utoa(hfe1,outval,10);

		if(rv2==4700) {	//470k-Widerstand?
			ra = strlen(outval);	//Nötig, um Komma anzuzeigen
			for(rb=0;rb<ra;rb++) {
				lcd_data(outval[rb]);
				if(rb==(ra-2)) lcd_data('.');	//Komma
			}
			lcd_data ('k'); //Kilo-Ohm, falls 470k-Widerstand verwendet
		} else {
			lcd_string(outval);
		}
		lcd_data(244);	//Omega für Ohm
		goto end;
	}

	if(NumOfDiodes == 0) {
		//Keine Dioden gefunden
		lcd_eep_string(TestFailed1); //"Kein,unbek. oder"
		Line2(); //2. Zeile
		lcd_eep_string(TestFailed2); //"defektes "
		lcd_eep_string(Bauteil);
	} else {
		lcd_eep_string(Bauteil);
		lcd_eep_string(Unknown); //" unbek."
		Line2(); //2. Zeile
		lcd_eep_string(OrBroken); //"oder defekt"
		lcd_data(NumOfDiodes + 48);
		lcd_data('d');
	}
	
	end:
	while(!(ON_PIN_REG & (1<<RST_PIN)));		//warten ,bis Taster losgelassen
	_delay_ms(200);
	for(hfe1 = 0;hfe1<10000;hfe1++) {
		if(!(ON_PIN_REG & (1<<RST_PIN))) {
			/*Wenn der Taster wieder gedrückt wurde...
			wieder zum Anfang springen und neuen Test durchführen
			*/
			goto start;
		}
		wdt_reset();
		_delay_ms(1);
	}
	ON_PORT &= ~(1<<ON_PIN);	//Abschalten
	wdt_disable();	//Watchdog aus
	//Endlosschleife
	while(1) {
		if(!(ON_PIN_REG & (1<<RST_PIN))) {	
			/* wird nur erreicht,
		 	wenn die automatische Abschaltung nicht eingebaut wurde */
			goto start;
		}
	}
	return 0;
}

void CheckPins(uint8_t HighPin, uint8_t LowPin, uint8_t TristatePin) {
	/*
	Funktion zum Testen der Eigenschaften des Bauteils bei der angegebenen Pin-Belegung
	Parameter:
	HighPin: Pin, der anfangs auf positives Potenzial gelegt wird
	LowPin: Pin, der anfangs auf negatives Potenzial gelegt wird
	TristatePin: Pin, der anfangs offen gelassen wird

	Im Testverlauf wird TristatePin natürlich auch positiv oder negativ geschaltet.
	*/
	unsigned int adcv[4];
	uint8_t tmpval, tmpval2;
	/*
		HighPin wird fest auf Vcc gelegt
		LowPin wird über R_L auf GND gelegt
		TristatePin wird hochohmig geschaltet, dafür ist keine Aktion nötig
	*/
	wdt_reset();
	//Pins setzen
	tmpval = (LowPin * 2);			//nötig wegen der Anordnung der Widerstände
	R_DDR = (1<<tmpval);			//Low-Pin auf Ausgang und über R_L auf Masse
	R_PORT = 0;
	ADC_DDR = (1<<HighPin);			//High-Pin auf Ausgang
	ADC_PORT = (1<<HighPin);		//High-Pin fest auf Vcc
	_delay_ms(5);
	//Bei manchen MOSFETs muss das Gate (TristatePin) zuerst entladen werden
	//N-Kanal:
	DischargePin(TristatePin,0);
	//Spannung am Low-Pin ermitteln
	adcv[0] = ReadADC(LowPin);
	if(adcv[0] < 20) goto next;	//Sperrt das Bauteil jetzt?
	//sonst: Entladen für P-Kanal (Gate auf Plus)
	DischargePin(TristatePin,1);
	//Spannung am Low-Pin ermitteln
	adcv[0] = ReadADC(LowPin);

	next:
	if(adcv[0] < 20) {	//Wenn das Bauteil keinen Durchgang zwischen HighPin und LowPin hat
		tmpval = (TristatePin * 2);		//nötig wegen der Anordnung der Widerstände
		R_DDR |= (1<<tmpval);			//Tristate-Pin über R_L auf Masse
		_delay_ms(2);
		adcv[0] = ReadADC(LowPin);		//Spannung messen
		if(adcv[0] > 700) {
			//Bauteil leitet => pnp-Transistor o.ä.
			//Verstärkungsfaktor in beide Richtungen messen
			//if(PartReady==1) goto testend;
			R_DDR &= ~(1<<tmpval);		//Tristate-Pin (Basis) hochohmig
			tmpval++;
			R_DDR |= (1<<tmpval);		//Tristate-Pin (Basis) über R_H auf Masse

			_delay_ms(10);
			adcv[0] = ReadADC(LowPin);		//Spannung am Low-Pin (vermuteter Kollektor) messen
			adcv[2] = ReadADC(TristatePin);	//Basisspannung messen
			R_DDR &= ~(1<<tmpval);		//Tristate-Pin (Basis) hochohmig
			//Prüfen, ob Test schon mal gelaufen
			if((PartFound == PART_TRANSISTOR) || (PartFound == PART_FET)) {
				PartReady = 1;
				hfe2 = adcv[0];
				uBE2 = adcv[2];
			} else {
				hfe1 = adcv[0];
				uBE1 = adcv[2];
			}
			if(adcv[2] > 200) {
				if(PartFound != PART_THYRISTOR) {
					PartFound = PART_TRANSISTOR;	//PNP-Transistor gefunden (Basis wird "nach oben" gezogen)
					PartMode = PART_MODE_PNP;
				}
			} else {
				if(PartFound != PART_THYRISTOR) {
				 	PartFound = PART_FET;			//P-Kanal-MOSFET gefunden (Basis/Gate wird NICHT "nach oben" gezogen)
					PartMode = PART_MODE_P_E_MOS;
				}
			}
			if(PartFound != PART_THYRISTOR) {
				b = TristatePin;
				c = LowPin;
				e = HighPin;
			}
		}

		//Tristate (vermutete Basis) auf Plus, zum Test auf npn
		ADC_PORT = 0;					//Low-Pin fest auf Masse
		tmpval = (TristatePin * 2);		//nötig wegen der Anordnung der Widerstände
		tmpval2 = (HighPin * 2);		//nötig wegen der Anordnung der Widerstände
		R_DDR = (1<<tmpval) | (1<<tmpval2);			//High-Pin und Tristate-Pin auf Ausgang
		R_PORT = (1<<tmpval) | (1<<tmpval2);		//High-Pin und Tristate-Pin über R_L auf Vcc
		ADC_DDR = (1<<LowPin);			//Low-Pin auf Ausgang
		_delay_ms(10);
		adcv[0] = ReadADC(HighPin);		//Spannung am High-Pin messen
		if(adcv[0] < 500) {
			if(PartReady==1) goto testend;
			//Bauteil leitet => npn-Transistor o.ä.

			//Test auf Thyristor:
			//Gate entladen
			
			R_PORT &= ~(1<<tmpval);			//Tristate-Pin (Gate) über R_L auf Masse
			_delay_ms(10);
			R_DDR &= ~(1<<tmpval);			//Tristate-Pin (Gate) hochohmig
			//Test auf Thyristor
			_delay_ms(5);
			adcv[1] = ReadADC(HighPin);		//Spannung am High-Pin (vermutete Anode) erneut messen
			
			R_PORT &= ~(1<<tmpval2);	//High-Pin (vermutete Anode) auf Masse
			_delay_ms(10);
			R_PORT |= (1<<tmpval2);	//High-Pin (vermutete Anode) wieder auf Plus
			_delay_ms(5);
			adcv[2] = ReadADC(HighPin);		//Spannung am High-Pin (vermutete Anode) erneut messen
			if((adcv[1] < 500) && (adcv[2] > 900)) {	//Nach Abschalten des Haltestroms muss der Thyristor sperren
				//war vor Abschaltung des Triggerstroms geschaltet und ist immer noch geschaltet obwohl Gate aus => Thyristor
				if(PartFound == PART_THYRISTOR) {
					PartFound = PART_TRIAC;	//Dieses Ergebnis schon einmal dagewesen => Triac (Doppelthyristor)
					PartReady = 1;
					goto saveresult;
				} else {
					PartFound = PART_THYRISTOR;
					goto saveresult;
				}
			}
			//Test auf Transistor oder MOSFET
			tmpval++;
			R_DDR |= (1<<tmpval);		//Tristate-Pin (Basis) auf Ausgang
			R_PORT |= (1<<tmpval);		//Tristate-Pin (Basis) über R_H auf Plus
			_delay_ms(30);
			adcv[0] = ReadADC(HighPin);		//Spannung am High-Pin (vermuteter Kollektor) messen
			adcv[2] = ReadADC(TristatePin);	//Basisspannung messen
			R_PORT &= ~(1<<tmpval);			//Tristate-Pin (Basis) hochohmig
			R_DDR &= ~(1<<tmpval);			//Tristate-Pin (Basis) auf Eingang

			if((PartFound == PART_TRANSISTOR) || (PartFound == PART_FET)) {	//prüfen, ob Test schon mal gelaufen
				PartReady = 1;
				hfe2 = 1023 - adcv[0];
				uBE2 = 1023 - adcv[2];
			} else {
				hfe1 = 1023 - adcv[0];
				uBE1 = 1023 - adcv[2];
			}
			if(adcv[2] < 600) {
				PartFound = PART_TRANSISTOR;	//NPN-Transistor gefunden (Basis wird "nach unten" gezogen)
				PartMode = PART_MODE_NPN;
			} else {
				PartFound = PART_FET;			//N-Kanal-MOSFET gefunden (Basis/Gate wird NICHT "nach unten" gezogen)
				PartMode = PART_MODE_N_E_MOS;
			}
			saveresult:
			b = TristatePin;
			c = HighPin;
			e = LowPin;
		}
		ADC_DDR = 0;
		ADC_PORT = 0;
		//Fertig
	} else {	//Durchgang
		//Test auf N-JFET oder selbstleitenden N-MOSFET
		R_DDR |= (2<<(TristatePin*2));	//Tristate-Pin (vermutetes Gate) über R_H auf Masse
		_delay_ms(20);
		adcv[0] = ReadADC(LowPin);		//Spannung am vermuteten Source messen
		R_PORT |= (2<<(TristatePin*2));	//Tristate-Pin (vermutetes Gate) über R_H auf Plus
		_delay_ms(20);
		adcv[1] = ReadADC(LowPin);		//Spannung am vermuteten Source erneut messen
		//Wenn es sich um einen selbstleitenden MOSFET oder JFET handelt, müsste adcv[1] > adcv[0] sein
		if(adcv[1]>(adcv[0]+100)) {
			//Spannung am Gate messen, zur Unterscheidung zwischen MOSFET und JFET
			ADC_PORT = 0;
			ADC_DDR = (1<<LowPin);	//Low-Pin fest auf Masse
			tmpval = (HighPin * 2);		//nötig wegen der Anordnung der Widerstände
			R_DDR |= (1<<tmpval);			//High-Pin auf Ausgang
			R_PORT |= (1<<tmpval);			//High-Pin über R_L auf Vcc
			_delay_ms(20);
			adcv[2] = ReadADC(TristatePin);		//Spannung am vermuteten Gate messen
			if(adcv[2]>800) {	//MOSFET
				PartFound = PART_FET;			//N-Kanal-MOSFET
				PartMode = PART_MODE_N_D_MOS;	//Verarmungs-MOSFET
			} else {	//JFET (pn-Übergang zwischen G und S leitet)
				PartFound = PART_FET;			//N-Kanal-JFET
				PartMode = PART_MODE_N_JFET;
			}
			PartReady = 1;
			b = TristatePin;
			c = HighPin;
			e = LowPin;
		}
		ADC_PORT = 0;

		//Test auf P-JFET oder selbstleitenden P-MOSFET
		ADC_DDR = (1<<LowPin);	//Low-Pin (vermuteter Drain) fest auf Masse, Tristate-Pin (vermutetes Gate) ist noch über R_H auf Plus
		tmpval = (HighPin * 2);			//nötig wegen der Anordnung der Widerstände
		R_DDR |= (1<<tmpval);			//High-Pin auf Ausgang
		R_PORT |= (1<<tmpval);			//High-Pin über R_L auf Vcc
		_delay_ms(20);
		adcv[0] = ReadADC(HighPin);		//Spannung am vermuteten Source messen
		R_PORT &= ~(2<<(TristatePin*2));	//Tristate-Pin (vermutetes Gate) über R_H auf Masse
		_delay_ms(20);
		adcv[1] = ReadADC(HighPin);		//Spannung am vermuteten Source erneut messen
		//Wenn es sich um einen selbstleitenden P-MOSFET oder P-JFET handelt, müsste adcv[0] > adcv[1] sein
		if(adcv[0]>(adcv[1]+100)) {
			//Spannung am Gate messen, zur Unterscheidung zwischen MOSFET und JFET
			ADC_PORT = (1<<HighPin);	//High-Pin fest auf Plus
			ADC_DDR = (1<<HighPin);		//High-Pin auf Ausgang
			_delay_ms(20);
			adcv[2] = ReadADC(TristatePin);		//Spannung am vermuteten Gate messen
			if(adcv[2]<200) {	//MOSFET
				PartFound = PART_FET;			//P-Kanal-MOSFET
				PartMode = PART_MODE_P_D_MOS;	//Verarmungs-MOSFET
			} else {	//JFET (pn-Übergang zwischen G und S leitet)
				PartFound = PART_FET;			//P-Kanal-JFET
				PartMode = PART_MODE_P_JFET;
			}
			PartReady = 1;
			b = TristatePin;
			c = LowPin;
			e = HighPin;
		}

		tmpval2 = (2<<(2*HighPin));	//R_H
		tmpval = (1<<(2*HighPin));	//R_L
		ADC_PORT = 0;
		//Test auf Diode
		ADC_DDR = (1<<LowPin);	//Low-Pin fest auf Masse, High-Pin ist noch über R_L auf Vcc
		DischargePin(TristatePin,1);	//Entladen für P-Kanal-MOSFET
		adcv[0] = ReadADC(HighPin);
		R_DDR = tmpval2;	//High-Pin über R_H auf Plus
		R_PORT = tmpval2;
		adcv[2] = ReadADC(HighPin);
		R_DDR = tmpval;	//High-Pin über R_L auf Plus
		R_PORT = tmpval;
		DischargePin(TristatePin,0);	//Entladen für N-Kanal-MOSFET
		adcv[1] = ReadADC(HighPin);
		R_DDR = tmpval2;	//High-Pin über R_H  auf Plus
		R_PORT = tmpval2;
		adcv[3] = ReadADC(HighPin);
		/*Ohne das Entladen kann es zu Falscherkennungen kommen, da das Gate eines MOSFETs noch geladen sein kann.
			Die zusätzliche Messung mit dem "großen" Widerstand R_H wird durchgeführt, um antiparallele Dioden von
			Widerständen unterscheiden zu können.
			Eine Diode hat eine vom Durchlassstrom relativ unabhängige Durchlassspg.
			Bei einem Widerstand ändert sich der Spannungsabfall stark (linear) mit dem Strom.
		*/
		if(adcv[0] > adcv[1]) {
			adcv[1] = adcv[0];	//der höhere Wert gewinnt
			adcv[3] = adcv[2];
		}
		if(PartFound == PART_NONE) PartFound = PART_DIODE;	//Diode nur angeben, wenn noch kein anderes Bauteil gefunden wurde. Sonst gäbe es Probleme bei Transistoren mit Schutzdiode
		if(NumOfDiodes < 3) {
			diodes[NumOfDiodes].Anode = HighPin;
			diodes[NumOfDiodes].Cathode = LowPin;
			diodes[NumOfDiodes].Voltage = (adcv[1]*60/13);	// ca. mit 4,6 multiplizieren, um aus dem ADC-Wert die Spannung in Millivolt zu erhalten
			NumOfDiodes++;
			for(uint8_t i=0;i<NumOfDiodes;i++) {
				if((diodes[i].Anode == LowPin) && (diodes[i].Cathode == HighPin)) {	//zwei antiparallele Dioden: Defekt oder Duo-LED
					if((adcv[3]*2) < adcv[1]) {	//Durchlassspannung fällt bei geringerem Teststrom stark ab => Defekt
						if(i<NumOfDiodes) {
							for(uint8_t j=i;j<(NumOfDiodes-1);j++) {
								diodes[j].Anode = diodes[j+1].Anode;
								diodes[j].Cathode = diodes[j+1].Cathode;
								diodes[j].Voltage = diodes[j+1].Voltage;
							}
						}
						NumOfDiodes -= 2;
					}
				}
			}
		}
	}
	tmpval2 = (2<<(2*HighPin));	//R_H
	tmpval = (1<<(2*HighPin));	//R_L
	ADC_PORT = 0;
	///Test auf Widerstand
	ADC_DDR = (1<<LowPin);	//Low-Pin fest auf Masse
	R_DDR = tmpval;	//High-Pin über R_L auf Plus
	R_PORT = tmpval;
	adcv[0] = ReadADC(HighPin);
	R_DDR = tmpval2;	//High-Pin über R_H auf Plus
	R_PORT = tmpval2;
	adcv[1] = ReadADC(HighPin);
	if(((adcv[1] + 200) < adcv[0]) && (((adcv[1] * 3) / 2) < adcv[0]) && (adcv[0] > 45)) {	//Durchlassspannung fällt bei geringerem Teststrom stark ab und es besteht kein "Beinahe-Kurzschluss" => Widerstand
		if((PartFound == PART_DIODE) || (PartFound == PART_NONE)) PartFound = PART_RESISTOR;
		ra = HighPin;
		rb = LowPin;
		rv1 = adcv[0];
		rv2 = adcv[1];
	}
	testend:
	ADC_DDR = 0;
	ADC_PORT = 0;
	R_DDR = 0;
	R_PORT = 0;
}



unsigned int ReadADC(uint8_t mux) {
	//ADC-Wert des angegebenen Kanals auslesen und als unsigned int zurückgegen
	unsigned int adcx = 0;
	ADMUX = mux | (1<<REFS0);
	for(uint8_t j=0;j<20;j++) {	//20 Messungen; für bessere Genauigkeit
		ADCSRA |= (1<<ADSC);
		while (ADCSRA&(1<<ADSC));
		adcx += ADCW;
	}
	adcx /= 20;
	return adcx;
}

void GetGateThresholdVoltage(void) {
	uint8_t tmpval, tmpval2;
	unsigned int tmpint = 0;
	tmpval = (1<<(2*c) | (2<<(2*b)));
	tmpval2=(1<<(2*c));
	R_DDR = tmpval;        // Drain über R_L auf Ausgang, Gate über R_H auf Ausgang
	ADC_DDR=(1<<e)|(1<<b);	//Gate und Source auf Ausgang
	if((PartFound==PART_FET) && (PartMode == PART_MODE_N_E_MOS)) {
		ADC_PORT = 0;			//Gate und Source fest auf Masse
		R_PORT = tmpval;  	   // Drain über R_L auf Plus, Gate über R_H auf Plus
		_delay_ms(5);

		tmpval=(1<<c);
		ADC_DDR=(1<<e);          // Gate über R_H langsam laden
		while (ADC_PIN&tmpval) {  // Warten, bis der MOSFET schaltet und Drain auf low geht
			wdt_reset();
			tmpint++;
			if(tmpint==0) break;	//Timeout für Gate-Schwellspannungs-Messung
		}
		R_PORT=tmpval2;          // Gate hochohmig schalten
		R_DDR=tmpval2;          // Gate hochohmig schalten
	} else if((PartFound==PART_FET) && (PartMode == PART_MODE_P_E_MOS)) {
		ADC_PORT = (1<<e)|(1<<b);	//Gate und Source fest auf Plus
		R_PORT = 0;					//Drain über R_L auf Masse, Gate über R_H auf Masse
		_delay_ms(5);

		tmpval=(1<<c);
		ADC_DDR=(1<<e);          // Gate über R_H langsam laden (Gate auf Eingang)
		ADC_PORT=(1<<e);          // Gate über R_H langsam laden (Gate-Pullup aus)
		while (!(ADC_PIN&tmpval)) {  // Warten, bis der MOSFET schaltet und Drain auf high geht
			wdt_reset();
			tmpint++;
			if(tmpint==0) break;	//Timeout für Gate-Schwellspannungs-Messung
		}
		R_DDR=tmpval2;          // Gate hochohmig schalten
	}
	if(tmpint > 0) {
		tmpint=ReadADC(b);
		if(PartMode == PART_MODE_P_E_MOS) {
			tmpint = 1023-tmpint;
		}
		tmpint=(tmpint*39/8);
	}
	utoa(tmpint, outval, 10);
	R_DDR = 0;
	R_PORT = 0;
	ADC_DDR = 0;
	ADC_PORT = 0;
}

void DischargePin(uint8_t PinToDischarge, uint8_t DischargeDirection) {
	/*Anschluss eines Bauelementes kurz(10ms) auf ein bestimmtes Potenzial legen
		Diese Funktion ist zum Entladen von MOSFET-Gates vorgesehen, um Schutzdioden u.ä. in MOSFETs erkennen zu können
		Parameter:
		PinToDischarge: zu entladender Pin
		DischargeDirection: 0 = gegen Masse (N-Kanal-FET), 1= gegen Plus(P-Kanal-FET)
	*/
	uint8_t tmpval;
	tmpval = (PinToDischarge * 2);		//nötig wegen der Anordnung der Widerstände

	if(DischargeDirection) R_PORT |= (1<<tmpval);			//R_L aus
	R_DDR |= (1<<tmpval);			//Pin auf Ausgang und über R_L auf Masse
	_delay_ms(10);
	R_DDR &= ~(1<<tmpval);			//Pin wieder auf Eingang
	if(DischargeDirection) R_PORT &= ~(1<<tmpval);			//R_L aus
}















